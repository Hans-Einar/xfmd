#include "SduiWorkflow.h"
#include "SdpRuntime.h"
#include "SduiTools.h"
#include "application/Application.h"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
namespace xfmd {
namespace {
constexpr unsigned timer = 11;
}
SduiWorkflow::SduiWorkflow(Application& owner)
    : app(owner),
      reason("Open a generated SDUI frame preview; regenerate it after source changes") {
  app.scheduler->restart(timer, 100, [this] { poll(); });
}
SduiWorkflow::~SduiWorkflow() {
  app.scheduler->cancel(timer);
  probe.cancel();
  // Fyne windows deliberately outlive document navigation and XFMD itself.
  for (const auto& child : children)
    waitpid(child.pid, nullptr, WNOHANG);
}
std::string SduiWorkflow::contextKey() const {
  auto current = app.sdpNavigation->currentSdui();
  if (!current || app.session.dirty())
    return {};
  return app.session.view().path + "\n" + current->path + "\n" + current->entry + "\n" +
         current->revision + "\n" + sduiExecutable("sdui-fyne") + "\n" +
         sduiExecutable("sdui-preview");
}
bool SduiWorkflow::canLaunch() const {
  return app.window->workspacePanel->tabs->getCurrent() == 2 && ready && !key.empty() &&
         key == contextKey();
}
void SduiWorkflow::poll() {
  for (auto it = children.begin(); it != children.end();) {
    int status = 0;
    const auto done = waitpid(it->pid, &status, WNOHANG);
    if (done == it->pid || (done < 0 && errno == ECHILD)) {
      if (done > 0 && (!WIFEXITED(status) || WEXITSTATUS(status))) {
        std::ifstream input(it->log);
        std::string diagnostic(1000, '\0');
        input.read(diagnostic.data(), diagnostic.size());
        diagnostic.resize(input.gcount());
        for (char& c : diagnostic)
          if (static_cast<unsigned char>(c) < 32)
            c = ' ';
        app.window->status->setText(
            ("SDUI prototype failed: " + diagnostic + " Log: " + it->log.string()).c_str());
      } else if (done > 0) {
        std::error_code ec;
        std::filesystem::remove_all(it->log.parent_path(), ec);
        app.window->status->setText("SDUI prototype window closed");
      }
      it = children.erase(it);
    } else
      ++it;
  }
  auto next = contextKey();
  if (next != key) {
    probe.cancel();
    pending = ready = false;
    key = next;
    target = app.sdpNavigation->currentSdui();
    host = sduiExecutable("sdui-fyne");
    auto helper = sduiExecutable("sdui-preview");
    reason = "Open a generated SDUI frame preview to launch its prototype";
    if (!key.empty()) {
      if (host.empty() || helper.empty())
        reason = "SDUI preview/Fyne helper unavailable; install the bundled SDUI tools";
      else {
        reason = "Checking SDUI prototype readiness…";
        pending = true;
        probe.start({helper, "-check", "-source", target->path, "-entry", target->entry,
                     "-revision", target->revision});
      }
    }
  }
  if (pending) {
    if (auto result = probe.take()) {
      pending = false;
      reason = result->error;
      if (result->reply) {
        const auto& r = *result->reply;
        ready = r.operation == "sdui-check" && r.revision == target->revision &&
                r.source == target->path && r.entry == target->entry;
        reason = ready ? r.diagnostic : "SDUI preflight returned a different source or entry";
      }
    }
  }
  const bool mode = app.window->workspacePanel->tabs->getCurrent() == 2;
  auto* toolbar = app.window->sdpTools;
  if (mode != toolbar->shown()) {
    if (mode)
      toolbar->show();
    else
      toolbar->hide();
    app.window->recalc();
  }
  app.window->sduiLaunch->setTipText(reason.c_str());
  app.window->sduiLaunch->setHelpText(reason.c_str());
  if (canLaunch())
    app.window->sduiLaunch->enable();
  else
    app.window->sduiLaunch->disable();
  app.scheduler->restart(timer, 200, [this] { poll(); });
}
void SduiWorkflow::launch() {
  if (!canLaunch())
    return;
  try {
    auto directory = sdpRuntimeWorkspace();
    auto log = directory / "prototype.log";
    std::vector<std::string> args{
        host, "-entry", target->entry, "-revision", target->revision, "-watch=true", target->path};
    std::vector<char*> argv;
    for (auto& arg : args)
      argv.push_back(arg.data());
    argv.push_back(nullptr);
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_addopen(&actions, STDIN_FILENO, "/dev/null", O_RDONLY, 0);
    posix_spawn_file_actions_addopen(&actions, STDOUT_FILENO, log.c_str(),
                                     O_WRONLY | O_CREAT | O_TRUNC, 0600);
    posix_spawn_file_actions_adddup2(&actions, STDOUT_FILENO, STDERR_FILENO);
    posix_spawn_file_actions_addclosefrom_np(&actions, 3);
    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);
    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID);
    pid_t pid;
    const int error = posix_spawn(&pid, argv[0], &actions, &attr, argv.data(), environ);
    posix_spawn_file_actions_destroy(&actions);
    posix_spawnattr_destroy(&attr);
    if (error) {
      std::filesystem::remove_all(directory);
      throw std::runtime_error(std::string("Cannot launch Fyne: ") + std::strerror(error));
    }
    children.push_back({pid, log});
    app.window->status->setText(
        ("SDUI: " + reason + " — separate window; log " + log.string()).c_str());
  } catch (const std::exception& e) {
    app.window->status->setText(e.what());
  }
}
} // namespace xfmd
