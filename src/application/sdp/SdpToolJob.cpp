#include "SdpToolJob.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <poll.h>
#include <signal.h>
#include <spawn.h>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
namespace xfmd {
namespace {
struct Pipe {
  int fd[2]{-1, -1};
  Pipe() {
    if (pipe2(fd, O_CLOEXEC))
      throw std::runtime_error("Cannot create SDP tool pipe");
  }
  ~Pipe() {
    for (int f : fd)
      if (f >= 0)
        close(f);
  }
  void reader() {
    close(fd[1]);
    fd[1] = -1;
    fcntl(fd[0], F_SETFL, O_NONBLOCK);
  }
};
std::string execute(std::vector<std::string>& args, std::atomic<bool>& canceled,
                    std::chrono::milliseconds timeout) {
  if (args.empty())
    throw std::runtime_error("No SDP executable configured");
  Pipe out, err;
  std::vector<char*> argv;
  for (auto& arg : args)
    argv.push_back(arg.data());
  argv.push_back(nullptr);
  posix_spawn_file_actions_t actions;
  posix_spawn_file_actions_init(&actions);
  posix_spawn_file_actions_adddup2(&actions, out.fd[1], STDOUT_FILENO);
  posix_spawn_file_actions_adddup2(&actions, err.fd[1], STDERR_FILENO);
  posix_spawn_file_actions_addopen(&actions, STDIN_FILENO, "/dev/null", O_RDONLY, 0);
  posix_spawn_file_actions_addclosefrom_np(&actions, 3);
  posix_spawnattr_t attr;
  posix_spawnattr_init(&attr);
  posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETPGROUP);
  posix_spawnattr_setpgroup(&attr, 0);
  pid_t pid = -1;
  const int failure = posix_spawnp(&pid, argv[0], &actions, &attr, argv.data(), environ);
  posix_spawn_file_actions_destroy(&actions);
  posix_spawnattr_destroy(&attr);
  if (failure)
    throw std::runtime_error("SDP runtime unavailable; run gh sdp setup. Cannot start SDP tool " + args.front() + ": " +
                             std::string(std::strerror(failure)) +
                             ". Install/update the gh sdp extension and run gh sdp setup; "
                             "development override: XFMD_SDP_TOOL");
  struct Child {
    pid_t pid;
    ~Child() {
      // Reap leader and terminate remaining descendants on every exit path.
      kill(-pid, SIGKILL);
      while (waitpid(pid, nullptr, 0) < 0 && errno == EINTR) {
      }
    }
  } child{pid};
  out.reader();
  err.reader();
  std::string output, errors;
  const auto end = std::chrono::steady_clock::now() + timeout;
  bool outEnd = false, errEnd = false, exited = false;
  siginfo_t status{};
  auto readPipe = [](int fd, std::string& bytes, bool& eof, std::size_t limit) {
    if (eof)
      return;
    char buffer[8192];
    // Bounded drain per cycle: cancellation stays responsive for a noisy child.
    for (int i = 0; i < 32; ++i) {
      auto count = read(fd, buffer, sizeof(buffer));
      if (count == 0) {
        eof = true;
        break;
      }
      if (count < 0) {
        if (errno != EAGAIN && errno != EINTR)
          throw std::runtime_error("Cannot read SDP tool output");
        break;
      }
      if (bytes.size() + count > limit)
        throw std::runtime_error("SDP tool output limit exceeded");
      bytes.append(buffer, count);
    }
  };
  while (!exited || !outEnd || !errEnd) {
    if (canceled)
      throw std::runtime_error("SDP request canceled");
    if (std::chrono::steady_clock::now() >= end)
      throw std::runtime_error("SDP tool timed out");
    pollfd fds[]{{outEnd ? -1 : out.fd[0], POLLIN, 0}, {errEnd ? -1 : err.fd[0], POLLIN, 0}};
    poll(fds, 2, 25);
    readPipe(out.fd[0], output, outEnd, 32 * 1024 * 1024);
    readPipe(err.fd[0], errors, errEnd, 64 * 1024);
    if (!exited) {
      // Keep the zombie until group cleanup so its PID cannot be recycled.
      auto done = waitid(P_PID, pid, &status, WEXITED | WNOHANG | WNOWAIT);
      if (done == 0 && status.si_pid == pid)
        exited = true;
      else if (done < 0 && errno != EINTR)
        throw std::runtime_error("Cannot wait for SDP tool");
    }
  }
  if (status.si_code != CLD_EXITED || status.si_status != 0)
    throw std::runtime_error(sdpError(errors));
  return output;
}
} // namespace
SdpToolJob::~SdpToolJob() { cancel(); }
void SdpToolJob::cancel() {
  canceled = true;
  if (worker.joinable())
    worker.join();
  std::lock_guard<std::mutex> lock(mutex);
  completed.reset();
}
void SdpToolJob::start(std::vector<std::string> argv, std::chrono::milliseconds deadline) {
  cancel();
  canceled = false;
  worker = std::thread([this, argv = std::move(argv), deadline]() mutable {
    SdpJobResult result;
    try {
      result.reply = decodeSdpReply(execute(argv, canceled, deadline));
    } catch (const std::exception& e) {
      result.error = e.what();
    }
    if (!canceled) {
      std::lock_guard<std::mutex> lock(mutex);
      completed = std::move(result);
    }
  });
}
std::optional<SdpJobResult> SdpToolJob::take() {
  std::lock_guard<std::mutex> lock(mutex);
  auto result = std::move(completed);
  completed.reset();
  return result;
}
std::vector<std::string> sdpCommand() {
  const char* tool = std::getenv("XFMD_SDP_TOOL");
  if (tool && *tool)
    return {tool};
  const char* home = std::getenv("HOME");
  // The registered public entrypoint is independent of desktop shell PATH.
  // A missing HOME deliberately selects an unusable absolute path, never PATH.
  return {(std::filesystem::path(home && *home ? home : "/nonexistent") /
           ".local/bin/sdptool").string()};
}
} // namespace xfmd
