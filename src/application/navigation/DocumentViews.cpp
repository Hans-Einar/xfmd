#include "DocumentViews.h"
#include "LinkResolver.h"
#include "application/Application.h"
#include "application/io/FileOpenPolicy.h"
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>
namespace xfmd {
DocumentViews::DocumentViews(Application& a, DocumentViewConfig c)
    : app(a), config(std::move(c)), scheduler(a.app), endpoint(config.window) {
  std::string pattern = WindowEndpoint::runtimeDirectory() + "/views-XXXXXX";
  std::vector<char> bytes(pattern.begin(), pattern.end());
  bytes.push_back(0);
  char* dir = mkdtemp(bytes.data());
  if (!dir)
    throw std::runtime_error("Cannot create private view directory");
  temporary = dir;
  navigator = std::make_unique<NavigationPanel>(app.window->navigationArea, app.app);
  navigator->host->setReadingColors(app.host->readingColors());
  navigator->error = [this](const auto& e) { app.window->status->setText(e.c_str()); };
  navigator->host->linkActivated = [this](const auto& uri, bool systemDefault) {
    if (!systemDefault && uri.rfind("sdl-view:", 0) == 0)
      follow(uri);
    else if (systemDefault || ExternalBrowser::accepts(uri))
      app.followLink(navigator->path(), uri, systemDefault);
    else {
      try {
        if (uri.find('#') != std::string::npos ||
            FileOpenPolicy::classify(LinkResolver::resourcePath(navigator->path(), uri)) ==
                FileOpenKind::Markdown)
          navigator->follow(uri);
        else
          app.followLink(navigator->path(), uri);
      } catch (const std::exception& e) {
        navigator->error(e.what());
      }
    }
  };
  endpoint.open = [this](const auto& pane, const auto& path) { return open(pane, path); };
  navigator->changed = [this] { documentChanged("navigation"); };
  endpoint.leased = [this](const auto& pane, const auto& lease, const auto& broker) {
    leases[pane] = {lease, broker};
  };
  endpoint.info = [this] { return app.session.view().path + "\t" + navigator->path(); };
  app.window->navigationArea->show();
  app.window->navigationArea->setWidth(330);
  app.window->split->recalc();
  if (!navigator->open(config.navigator))
    throw std::runtime_error("Cannot open navigator");
  scheduler.restart(10, 40, [this] { poll(); });
}
DocumentViews::~DocumentViews() {
  scheduler.cancelAll();
  documentChanged("main");
  documentChanged("navigation");
  flushReleases();
  cancel();
  navigator.reset();
  std::filesystem::remove_all(temporary);
}
void DocumentViews::cancel() {
  if (child > 0) {
    kill(-child, SIGKILL);
    while (waitpid(child, nullptr, 0) < 0 && errno == EINTR) {
    };
    child = -1;
  }
}
bool DocumentViews::open(const std::string& pane, const std::string& path) {
  flushReleases();
  if (releases.size() >= 256)
    return false;
  if (pane == "navigation")
    return navigator->open(path);
  if (pane != "main")
    return false;
  // External delivery must not open a dirty-buffer modal dialog from IPC.
  if (app.session.dirty()) {
    app.window->status->setText("Save or discard edits before generated navigation");
    return false;
  }
  auto error = app.documents.error;
  app.documents.error = [this](auto e) { app.window->status->setText(e.c_str()); };
  bool result = false;
  try {
    result = app.open(path);
  } catch (...) {
    app.documents.error = error;
    throw;
  }
  app.documents.error = error;
  return result;
}
bool DocumentViews::follow(const std::string& uri) {
  auto reject = [this](const char* why) {
    app.window->status->setText(why);
    return false;
  };
  const auto prefix = "sdl-view://" + config.project + "/";
  if (config.tool.empty() || (config.source.empty() && config.broker.empty()) ||
      config.project.empty() || uri.rfind(prefix, 0) != 0 || uri.size() > 8192 ||
      uri.find_first_of("\n\r\t") != std::string::npos)
    return reject("Unregistered SDL project or invalid URI");
  cancel();
  ++sequence;
  pendingDirectory = temporary + "/" + std::to_string(sequence);
  pendingPane = "main"; // Delivery metadata comes from the registered tool after URI validation.
  std::vector<std::string> args{config.tool, "view",     config.source,   "--uri",
                                uri,         "--output", pendingDirectory};
  if (!config.renderer.empty()) {
    args.push_back("--renderer");
    args.push_back(config.renderer);
  }
  if (!config.broker.empty())
    args = {config.tool,
            "--socket",
            config.broker,
            "--uri",
            uri,
            "--window",
            config.window,
            "--client",
            config.window,
            "--sequence",
            std::to_string(sequence),
            "--open"};
  std::vector<char*> argv;
  for (auto& s : args)
    argv.push_back(s.data());
  argv.push_back(nullptr);
  std::string log = temporary + "/tool.log";
  child = fork();
  if (child < 0)
    return reject("Cannot start SDL tool");
  if (child == 0) {
    setpgid(0, 0);
    int fd = ::open(log.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd >= 0) {
      dup2(fd, 1);
      dup2(fd, 2);
      close(fd);
    }
    execv(argv[0], argv.data());
    _exit(127);
  }
  setpgid(child, child);
  deadline = std::chrono::steady_clock::now() + std::chrono::seconds(65);
  app.window->status->setText("Generating selected SDL view…");
  return true;
}
void DocumentViews::documentChanged(const std::string& pane) {
  auto it = leases.find(pane);
  if (it != leases.end()) {
    releases.push_back(it->second);
    leases.erase(it);
  }
  flushReleases();
}
void DocumentViews::flushReleases() {
  for (auto it = releases.begin(); it != releases.end();) {
    int fd = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    bool sent = false;
    if (fd >= 0) {
      sockaddr_un a{};
      a.sun_family = AF_UNIX;
      if (it->second.size() < sizeof(a.sun_path)) {
        std::memcpy(a.sun_path, it->second.c_str(), it->second.size() + 1);
        if (connect(fd, reinterpret_cast<sockaddr*>(&a), sizeof(a)) == 0) {
          auto packet = "SDLVIEW1\tRELEASE\t" + it->first + "\n";
          sent = send(fd, packet.data(), packet.size(), MSG_NOSIGNAL) ==
                 static_cast<ssize_t>(packet.size());
        }
      }
      close(fd);
    }
    if (sent)
      it = releases.erase(it);
    else
      ++it;
  }
}
void DocumentViews::poll() {
  endpoint.poll();
  flushReleases();
  if (child > 0) {
    int status = 0;
    auto result = waitpid(child, &status, WNOHANG);
    if (result == child) {
      child = -1;
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0 && config.broker.empty()) {
        std::ifstream delivery(pendingDirectory + "/delivery.txt");
        std::getline(delivery, pendingPane);
        open(pendingPane, pendingDirectory + "/entry.md");
      } else if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::ifstream in(temporary + "/tool.log");
        std::string message(4096, '\0');
        in.read(message.data(), message.size());
        message.resize(in.gcount());
        app.window->status->setText(("SDL: " + message).c_str());
      }
    } else if (std::chrono::steady_clock::now() > deadline) {
      cancel();
      app.window->status->setText("SDL generation timed out; current document retained");
    }
  }
  scheduler.restart(10, 40, [this] { poll(); });
}
} // namespace xfmd
