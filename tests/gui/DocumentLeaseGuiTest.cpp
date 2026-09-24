#include "application/Application.h"
#include "support/TestSupport.h"
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <set>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
using namespace xfmd;
namespace {
bool failReleaseSend = false;
int injectedFailures = 0;
sockaddr_un address(const std::string& path) {
  sockaddr_un a{};
  a.sun_family = AF_UNIX;
  CHECK(path.size() < sizeof(a.sun_path));
  std::memcpy(a.sun_path, path.c_str(), path.size() + 1);
  return a;
}
struct Broker {
  int fd;
  std::string path;
  std::vector<std::string> received;
  explicit Broker(std::string p) : path(std::move(p)) {
    fd = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    CHECK(fd >= 0);
    auto a = address(path);
    CHECK(bind(fd, reinterpret_cast<sockaddr*>(&a), sizeof(a)) == 0);
    CHECK(listen(fd, 300) == 0);
  }
  ~Broker() {
    close(fd);
    unlink(path.c_str());
  }
  void drain() {
    for (;;) {
      int peer = accept4(fd, nullptr, nullptr, SOCK_NONBLOCK | SOCK_CLOEXEC);
      if (peer < 0)
        break;
      char data[1024];
      auto n = recv(peer, data, sizeof(data), MSG_DONTWAIT);
      if (n > 0)
        received.emplace_back(data, n);
      close(peer); // Deliberately no acknowledgment or durable broker operation.
    }
  }
};
void pump(Application& app, int milliseconds, Broker* broker = nullptr) {
  auto until = std::chrono::steady_clock::now() + std::chrono::milliseconds(milliseconds);
  do {
    app.app.runWhileEvents();
    if (broker)
      broker->drain();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  } while (std::chrono::steady_clock::now() < until);
}
std::string packet(Application& app, const std::string& message) {
  int fd = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0);
  CHECK(fd >= 0);
  struct Close {
    int fd;
    ~Close() { close(fd); }
  } guard{fd};
  auto a = address(WindowEndpoint::runtimeDirectory() + "/leases.sock");
  CHECK(connect(fd, reinterpret_cast<sockaddr*>(&a), sizeof(a)) == 0);
  CHECK(send(fd, message.data(), message.size(), MSG_NOSIGNAL) == ssize_t(message.size()));
  for (int i = 0; i < 100; ++i) {
    pump(app, 5);
    char b[32769];
    auto n = recv(fd, b, sizeof(b), MSG_DONTWAIT);
    if (n > 0)
      return std::string(b, n);
    CHECK(n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK));
  }
  throw std::runtime_error("Endpoint response timed out");
}
} // namespace
extern "C" ssize_t __real_send(int, const void*, size_t, int);
extern "C" ssize_t __wrap_send(int fd, const void* data, size_t length, int flags) {
  constexpr char prefix[] = "SDLVIEW1\tRELEASE\t";
  if (failReleaseSend && length >= sizeof(prefix) - 1 &&
      std::memcmp(data, prefix, sizeof(prefix) - 1) == 0) {
    ++injectedFailures;
    errno = EAGAIN;
    return -1;
  }
  return __real_send(fd, data, length, flags);
}
void run() {
  char pattern[] = "/tmp/xfmd-lease-gui-XXXXXX";
  auto* directory = mkdtemp(pattern);
  CHECK(directory);
  std::filesystem::path dir = directory;
  struct Cleanup {
    std::filesystem::path dir;
    ~Cleanup() { std::filesystem::remove_all(dir); }
  } cleanup{dir};
  CHECK(setenv("XDG_RUNTIME_DIR", directory, 1) == 0);
  const auto first = (dir / "first.md").string(), second = (dir / "second.md").string();
  const auto nav = (dir / "nav.md").string(), brokerPath = (dir / "broker.sock").string();
  std::ofstream(first) << "# First\n";
  std::ofstream(second) << "# Second\n";
  std::ofstream(nav) << "# Navigator\n";
  int argc = 1;
  char name[] = "xfmd-lease-test";
  char* argv[] = {name, nullptr};
  Application app;
  app.initialize(argc, argv);
  CHECK(app.open(first));
  DocumentViewConfig config{nav, "", "", "", "", "leases", ""};
  app.documentViews = std::make_unique<DocumentViews>(app, config);
  auto broker = std::make_unique<Broker>(brokerPath);
  int sequence = 0;
  auto open = [&](const std::string& path, const std::string& lease,
                  const std::string& pane = "main") {
    ++sequence;
    auto fields = lease.empty() ? std::string{} : lease + "\t" + brokerPath + "\t";
    return packet(app, "XFMD1\tOPEN\tleases\ttest\t" + std::to_string(sequence) + "\t" + pane +
                           "\t" + path + "\t" + fields + "end\n");
  };
  CHECK(open(first, "retained").rfind("OK\t", 0) == 0);
  int modalCalls = 0;
  app.documents.chooseUnsaved = [&] {
    ++modalCalls;
    return UnsavedChoice::Cancel;
  };
  app.edits.applyEdit({0, 0, "unsaved "});
  auto dirty = app.session.snapshot();
  CHECK(open(second, "rejected-dirty") == "ERROR\tdocument not opened\n");
  CHECK(app.session.view().token == dirty.token && app.session.view().text == dirty.text);
  CHECK(app.session.view().path == first && app.session.dirty() && modalCalls == 0);
  broker->drain();
  CHECK(broker->received.empty());
  app.edits.applyEdit({0, 8, ""});
  CHECK(!app.session.dirty());
  CHECK(open((dir / "missing.md").string(), "rejected-missing") == "ERROR\tdocument not opened\n");
  CHECK(app.session.view().path == first);
  broker->drain();
  CHECK(broker->received.empty());
  CHECK(open(second, "active").rfind("OK\t", 0) == 0);
  pump(app, 100, broker.get());
  CHECK(broker->received == std::vector<std::string>{"SDLVIEW1\tRELEASE\tretained\n"});
  std::cout << "PASS: dirty main rejected without modal; failed open preserves active lease\n";

  failReleaseSend = true;
  CHECK(open(first, "retry").rfind("OK\t", 0) == 0);
  pump(app, 100, broker.get());
  CHECK(injectedFailures > 0 && broker->received.size() == 1);
  failReleaseSend = false;
  pump(app, 150, broker.get());
  CHECK(broker->received.size() == 2 && broker->received.back() == "SDLVIEW1\tRELEASE\tactive\n");
  pump(app, 200, broker.get());
  CHECK(broker->received.size() == 2);
  std::cout << "PASS: injected EAGAIN send retries; successful send is not retried without "
               "acknowledgment\n";

  broker.reset(); // Actual connect failures while the broker is absent.
  CHECK(open(second, "queued-0").rfind("OK\t", 0) == 0);
  for (int i = 1; i < 256; ++i)
    CHECK(open(i % 2 ? first : second, "queued-" + std::to_string(i)).rfind("OK\t", 0) == 0);
  auto retained = app.session.snapshot();
  CHECK(open(second, "overflow") == "ERROR\tdocument not opened\n");
  CHECK(app.session.view().token == retained.token && app.session.view().path == retained.path);
  broker = std::make_unique<Broker>(brokerPath);
  pump(app, 600, broker.get());
  CHECK(broker->received.size() == 256);
  std::set<std::string> unique(broker->received.begin(), broker->received.end());
  CHECK(unique.size() == 256 && unique.count("SDLVIEW1\tRELEASE\tretry\n") == 1);
  for (int i = 0; i < 255; ++i)
    CHECK(unique.count("SDLVIEW1\tRELEASE\tqueued-" + std::to_string(i) + "\n") == 1);
  CHECK(open(second, "final-main").rfind("OK\t", 0) == 0);
  CHECK(open(nav, "final-navigation", "navigation").rfind("OK\t", 0) == 0);
  app.documentViews.reset();
  broker->drain();
  unique = std::set<std::string>(broker->received.begin(), broker->received.end());
  CHECK(broker->received.size() == 259 && unique.size() == 259);
  CHECK(unique.count("SDLVIEW1\tRELEASE\tqueued-255\n") == 1);
  CHECK(unique.count("SDLVIEW1\tRELEASE\tfinal-main\n") == 1);
  CHECK(unique.count("SDLVIEW1\tRELEASE\tfinal-navigation\n") == 1);
  std::cout
      << "PASS: 256-release admission bound, actual connect retry, recovery and both-panel close\n";

  broker.reset();
  app.documentViews = std::make_unique<DocumentViews>(app, config);
  CHECK(open(first, "unpersisted").rfind("OK\t", 0) == 0);
  app.documentViews.reset(); // Broker remains unavailable throughout destruction.
  broker = std::make_unique<Broker>(brokerPath);
  app.documentViews = std::make_unique<DocumentViews>(app, config);
  pump(app, 200, broker.get());
  CHECK(broker->received.empty());
  app.documentViews.reset();
  std::cout << "PASS: unavailable-broker shutdown does not replay releases in a new owner\n";
}
TEST_MAIN(run)
