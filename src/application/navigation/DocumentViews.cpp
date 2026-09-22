#include "DocumentViews.h"
#include "application/Application.h"
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <signal.h>
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
  navigator->host->linkActivated = [this](const auto& uri) {
    if (uri.rfind("sdl-view:", 0) == 0)
      follow(uri);
    else
      navigator->follow(uri);
  };
  endpoint.open = [this](const auto& pane, const auto& path) { return open(pane, path); };
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
  if (config.tool.empty() || config.source.empty() || config.project.empty() ||
      uri.rfind(prefix, 0) != 0 || uri.size() > 8192 ||
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
void DocumentViews::poll() {
  endpoint.poll();
  if (child > 0) {
    int status = 0;
    auto result = waitpid(child, &status, WNOHANG);
    if (result == child) {
      child = -1;
      if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        std::ifstream delivery(pendingDirectory + "/delivery.txt");
        std::getline(delivery, pendingPane);
        open(pendingPane, pendingDirectory + "/entry.md");
      } else {
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
