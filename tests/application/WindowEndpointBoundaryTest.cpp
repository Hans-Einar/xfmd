#include "application/navigation/WindowEndpoint.h"
#include "support/TestSupport.h"
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <memory>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
using namespace xfmd;
namespace {
bool foreignPeer = false;
struct Socket {
  int fd;
  explicit Socket(const std::string& path) {
    fd = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0);
    CHECK(fd >= 0);
    sockaddr_un a{};
    a.sun_family = AF_UNIX;
    CHECK(path.size() < sizeof(a.sun_path));
    std::memcpy(a.sun_path, path.c_str(), path.size() + 1);
    CHECK(connect(fd, reinterpret_cast<sockaddr*>(&a), sizeof(a)) == 0);
  }
  ~Socket() { close(fd); }
  std::string read() {
    char b[32769];
    auto n = recv(fd, b, sizeof(b), MSG_DONTWAIT);
    return n > 0 ? std::string(b, n) : std::string{};
  }
  void send(const std::string& packet) {
    CHECK(::send(fd, packet.data(), packet.size(), MSG_NOSIGNAL) == ssize_t(packet.size()));
  }
};
} // namespace
extern "C" int __real_getsockopt(int, int, int, void*, socklen_t*);
extern "C" int __wrap_getsockopt(int fd, int level, int option, void* value, socklen_t* size) {
  int result = __real_getsockopt(fd, level, option, value, size);
  if (result == 0 && foreignPeer && level == SOL_SOCKET && option == SO_PEERCRED)
    static_cast<ucred*>(value)->uid = getuid() + 1;
  return result;
}
void run() {
  char pattern[] = "/tmp/xfmd-endpoint-bounds-XXXXXX";
  auto* directory = mkdtemp(pattern);
  CHECK(directory);
  struct Cleanup {
    std::string path;
    ~Cleanup() { std::filesystem::remove_all(path); }
  } cleanup{directory};
  CHECK(setenv("XDG_RUNTIME_DIR", directory, 1) == 0);
  WindowEndpoint endpoint("bounds");
  int opens = 0, leases = 0;
  bool allow = true;
  endpoint.open = [&](const auto&, const auto&) {
    ++opens;
    return allow;
  };
  endpoint.leased = [&](const auto&, const auto&, const auto&) { ++leases; };
  auto request = [](const std::string& client, int seq, const std::string& tail = "end") {
    return "XFMD1\tOPEN\tbounds\t" + client + "\t" + std::to_string(seq) + "\tmain\t/example.md\t" +
           tail + "\n";
  };
  for (const auto& tail : {"bad!\t/tmp/broker\tend", "valid\trelative\tend", "\t/tmp/broker\tend"})
    CHECK(endpoint.handle(request("client", 1, tail)) == "ERROR\tinvalid lease metadata\n");
  CHECK(opens == 0 && leases == 0);
  allow = false;
  CHECK(endpoint.handle(request("client", 1, "valid\t/tmp/broker\tend")) ==
        "ERROR\tdocument not opened\n");
  CHECK(opens == 1 && leases == 0);
  allow = true;
  CHECK(endpoint.handle(request("client", 1)) == "ERROR\tstale request\n");
  CHECK(endpoint.handle(request("client", 2, "valid\t/tmp/broker\tend")) == "OK\t2\n");
  CHECK(leases == 1);
  for (int i = 1; i < 256; ++i)
    CHECK(endpoint.handle(request("c" + std::to_string(i), 1)) == "OK\t1\n");
  int before = opens;
  CHECK(endpoint.handle(request("overflow", 1)) == "ERROR\tclient limit\n");
  CHECK(opens == before);
  CHECK(endpoint.handle(request("client", 3)) == "OK\t3\n");
  std::cout << "PASS: lease metadata, failed-open sequence consumption, 256 keys and existing-key "
               "admission\n";

  auto path = WindowEndpoint::runtimeDirectory() + "/bounds.sock";
  before = opens;
  {
    Socket closed(path);
  }
  endpoint.poll();
  CHECK(opens == before);
  {
    Socket disconnected(path);
    disconnected.send(request("client", 4));
  }
  endpoint.poll();
  CHECK(opens == before + 1); // Delivery may commit even if its acknowledgment is lost.
  CHECK(endpoint.handle(request("client", 4)) == "ERROR\tstale request\n");
  std::cout << "PASS: disconnect before packet is inert; after packet commits once without reply\n";

  std::vector<std::unique_ptr<Socket>> pending;
  for (int i = 0; i < 16; ++i)
    pending.push_back(std::make_unique<Socket>(path));
  endpoint.poll();
  {
    Socket overflow(path);
    endpoint.poll();
    char c;
    CHECK(recv(overflow.fd, &c, 1, MSG_DONTWAIT) == 0);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(2100));
  endpoint.poll();
  for (auto& peer : pending)
    CHECK(peer->read() == "ERROR\tpacket size or timeout\n");
  pending.clear();
  {
    Socket recovered(path);
    recovered.send("XFMD1\tINFO\tbounds\n");
    endpoint.poll();
    CHECK(recovered.read() == "OK\t\n");
  }
  std::cout << "PASS: 16 pending peers, overflow close, idle expiry and recovered capacity\n";
  {
    Socket rejected(path);
    foreignPeer = true;
    endpoint.poll();
    foreignPeer = false;
    char c;
    CHECK(recv(rejected.fd, &c, 1, MSG_DONTWAIT) == 0);
  }
  std::cout
      << "PASS: foreign UID rejection with injected SO_PEERCRED result (not a cross-UID process)\n";
}
TEST_MAIN(run)
