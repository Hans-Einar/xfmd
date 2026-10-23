#include "SdpNavigation.h"
#include "SdpRuntime.h"
#include "SdpSources.h"
#include "SduiTools.h"
#include "application/Application.h"
#include <cstdlib>
#include <unistd.h>
namespace xfmd {
namespace fs = std::filesystem;
namespace {
constexpr unsigned scheduleKey = 9, watchKey = 10;
}
SdpNavigation::SdpNavigation(Application& app) : app(app) {}
SdpNavigation::~SdpNavigation() {
  stop();
  std::error_code ec;
  if (!temporary.empty())
    fs::remove_all(temporary, ec);
}
void SdpNavigation::discardPending() {
  std::error_code ec;
  if (!pendingBundle.empty())
    fs::remove_all(pendingBundle, ec);
  pendingBundle.clear();
}
void SdpNavigation::stop() {
  app.scheduler->cancel(watchKey);
  watch.stop();
  refreshQueued = false;
  cancelRequest();
}
void SdpNavigation::cancelRequest() {
  app.scheduler->cancel(scheduleKey);
  job.cancel();
  expansions.clear();
  expanding = {};
  discardPending();
  pendingOperation.clear();
}
void SdpNavigation::message(const std::string& text) {
  app.window->workspacePanel->sdp->message(text);
  app.window->status->setText(text.c_str());
}
void SdpNavigation::start(const std::string& operation, std::vector<std::string> args) {
  pendingOperation = operation;
  auto command = sdpCommand();
  command.insert(command.end(), args.begin(), args.end());
  command.push_back("--json");
  job.start(std::move(command));
  app.scheduler->restart(scheduleKey, 40, [this] { poll(); });
}
void SdpNavigation::refresh(const std::string& selected) {
  const bool changedRoot = root != selected;
  stop();
  root = selected;
  watchError.clear();
  project.clear();
  sources.clear();
  if (changedRoot)
    app.window->workspacePanel->sdp->clear();
  watch.start(root);
  app.scheduler->restart(watchKey, 250, [this] { pollWatch(); });
  discover();
}
void SdpNavigation::openSource(const SdpTarget& target) {
  const auto source = sources.find(target.model);
  if (project.empty() || target.project != project || source == sources.end() ||
      !sdpTextSource(source->second) || source->second.revision != target.revision) {
    message("SDP source selection changed; Refresh and choose View source again");
    return;
  }
  const auto expected = (fs::path(area).parent_path() / source->second.path).lexically_normal();
  std::error_code ec;
  if (expected != fs::path(target.path) ||
      !sdpContained(area, target.path) || !fs::is_regular_file(target.path, ec)) {
    message("SDP source is missing or outside the selected area; Refresh");
    return;
  }
  if ((pendingOperation == "discover" || pendingOperation == "actions"))
    refreshQueued = true;
  cancelRequest();
  // Source editing uses the normal document transaction, including dirty cancel.
  if (app.open(target.path)) {
    app.views->setMode(ViewMode::Editor);
    message("SDP source opened — " + target.path);
  }
}
void SdpNavigation::activate(const SdpTarget& target) {
  if (project.empty() || target.project != project) {
    message("SDP selection belongs to an unavailable project; Refresh");
    return;
  }
  if (target.operation != "open" && target.operation != "select" &&
      target.operation != "sdui-preview") {
    message("Unsupported SDP action: " + target.operation);
    return;
  }
  if (target.operation == "open") {
    std::error_code ec;
    if (!sdpContained(area, target.path) || !fs::is_regular_file(target.path, ec)) {
      message("SDP file is missing or outside the selected area; Refresh");
      return;
    }
    if ((pendingOperation == "discover" || pendingOperation == "actions"))
      refreshQueued = true;
    cancelRequest();
    app.open(target.path);
    return;
  }
  if (target.model.empty() || target.revision.empty() ||
      (target.operation == "select" &&
       (target.uri.rfind("sdl-view://" + project + "/", 0) != 0 || target.uri.size() > 8192 ||
        target.uri.find_first_of("\t\r\n") != std::string::npos)) ||
      (target.operation == "sdui-preview" && target.entry.empty())) {
    message("Incomplete SDP selection; expand the model and choose a view/frame");
    return;
  }
  if (app.session.dirty()) {
    message("Save or discard edits before opening a generated SDP view");
    return;
  }
  if ((pendingOperation == "discover" || pendingOperation == "actions"))
    refreshQueued = true;
  cancelRequest();
  try {
    if (temporary.empty())
      temporary = sdpRuntimeWorkspace();
    pendingBundle = temporary / std::to_string(++request);
    fs::create_directory(pendingBundle);
    selection = target;
    token = app.session.view().token;
    std::vector<std::string> args{root,         target.operation,      "--model",
                                  target.model, "--revision",          target.revision,
                                  "--output",   pendingBundle.string()};
    if (target.operation == "select") {
      args.push_back("--uri");
      args.push_back(target.uri);
    } else {
      args.push_back("--entry");
      args.push_back(target.entry);
    }
    message("Generating selected SDP view…");
    auto helper = sduiExecutable("sdui-preview");
    if (target.operation == "sdui-preview" && !helper.empty()) {
      const auto source = sources.find(target.model);
      if (source == sources.end() || source->second.kind != "sdui" ||
          source->second.revision != target.revision || !sdpContained(area, target.path))
        throw std::runtime_error("SDUI source changed; Refresh");
      const auto expected = (fs::path(area).parent_path() / source->second.path).lexically_normal();
      if (expected != fs::path(target.path))
        throw std::runtime_error("SDUI source path does not match catalog");
      pendingOperation = target.operation;
      job.start({helper, "-source", target.path, "-entry", target.entry,
                 "-revision", target.revision, "-output", pendingBundle.string()});
      app.scheduler->restart(scheduleKey, 40, [this] { poll(); });
    } else {
      start(target.operation, std::move(args));
    }
  } catch (const std::exception& e) {
    discardPending();
    message(e.what());
  }
}
bool SdpNavigation::follow(const std::string& document, const std::string& uri) {
  if (uri.rfind("sdl-source://", 0) == 0)
    return followSdlSource(document, uri);
  if (uri.rfind("sdui-source://", 0) == 0)
    return followSduiSource(document, uri);
  if (uri.rfind("sdl-view:", 0) != 0)
    return false;
  auto context = contexts.find(document);
  if (context == contexts.end())
    return false;
  if (root != context->second.root) {
    message("Select the originating SDP project before following this generated link");
    return true;
  }
  auto target = context->second.target;
  target.operation = "select";
  target.uri = uri;
  activate(target);
  return true;
}
bool SdpNavigation::deliver(const SdpReply& reply) {
  if (app.session.dirty() || app.session.view().token != token) {
    message("SDP result ignored: the current document changed");
    return false;
  }
  if (reply.revision != selection.revision || !sdpContained(pendingBundle.string(), reply.entry) ||
      fs::canonical(reply.directory) != fs::canonical(pendingBundle)) {
    message("SDP result has the wrong revision or output location");
    return false;
  }
  std::size_t bytes = 0, files = 0;
  for (const auto& entry : fs::recursive_directory_iterator(pendingBundle)) {
    if (entry.is_symlink())
      throw std::runtime_error("Unexpected symlink in SDP bundle");
    if (++files > 128)
      throw std::runtime_error("SDP bundle file limit exceeded");
    if (entry.is_regular_file())
      bytes += entry.file_size();
    if (bytes > 32 * 1024 * 1024)
      throw std::runtime_error("SDP bundle byte limit exceeded");
  }
  // Generated delivery must not open a late modal error/unsaved dialog.
  auto error = app.documents.error;
  struct Restore {
    Application& app;
    std::function<void(const std::string&)> error;
    ~Restore() { app.documents.error = std::move(error); }
  } restore{app, std::move(error)};
  app.documents.error = [this](const std::string& text) { message(text); };
  if (!app.open(reply.entry))
    return false;
  contexts[app.session.view().path] = {root, selection, pendingBundle, reply.sourceLinks};
  pendingBundle
      .clear(); // Retained while history/current document or asynchronous work can refer to it.
  app.views->setMode(ViewMode::Preview);
  message("SDP snapshot opened — select again to regenerate");
  return true;
}
void SdpNavigation::prune() {
  if (!pendingOperation.empty() || app.preview->busy() || app.references->busy() ||
      (app.exporter && app.exporter->busy()))
    return;
  for (auto it = contexts.begin(); it != contexts.end();) {
    const auto& bundle = it->second.directory;
    // Session/history paths are normalized at document admission. Keep their
    // ownership even if an external operation has removed an entry file.
    const auto prefix = bundle.string() + "/";
    bool live = app.session.view().path.rfind(prefix, 0) == 0;
    const auto& history = app.navigation->history;
    for (std::size_t i = 0; !live && i < history.size(); ++i)
      live = history.at(i).path.rfind(prefix, 0) == 0;
    if (live) {
      ++it;
      continue;
    }
    std::error_code ec;
    fs::remove_all(bundle, ec);
    if (ec)
      ++it; // Retry later; do not discard context before its files are released.
    else
      it = contexts.erase(it);
  }
}
void SdpNavigation::pollWatch() {
  prune();
  std::string diagnostic;
  if (watch.take(diagnostic))
    refreshQueued = true;
  if (!diagnostic.empty()) {
    watchError = diagnostic;
    message("SDP automatic refresh unavailable: " + watchError);
  }
  if (refreshQueued && pendingOperation.empty()) {
    refreshQueued = false;
    discover();
  }
  app.scheduler->restart(watchKey, 250, [this] { pollWatch(); });
}
} // namespace xfmd
