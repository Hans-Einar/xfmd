#include "SdpNavigation.h"
#include "application/Application.h"
#include <charconv>
namespace xfmd {
bool SdpNavigation::followSdlSource(const std::string& document, const std::string& uri) {
  try {
    const auto it = contexts.find(document);
    if (it == contexts.end() || it->second.root != root ||
        it->second.target.project != project || it->second.target.operation != "select" ||
        it->second.sourceLinks.empty())
      throw std::runtime_error("SDL source link has no retained project context");
    // Opening and history pruning may mutate contexts: retain the checked value.
    const auto context = it->second;
    const auto range = uri.substr(13);
    const auto first = range.find('/'), last = range.find('/', first == std::string::npos ? 0 : first + 1);
    std::size_t start = 0, end = 0;
    auto number = [](const std::string& text, std::size_t& value) {
      auto result = std::from_chars(text.data(), text.data() + text.size(), value);
      return !text.empty() && result.ec == std::errc() && result.ptr == text.data() + text.size();
    };
    if (first == std::string::npos || last == std::string::npos ||
        !number(range.substr(first + 1, last - first - 1), start) ||
        !number(range.substr(last + 1), end) || start >= end)
      throw std::runtime_error("Invalid SDL source span");
    const auto id = range.substr(0, first);
    std::string path, snapshot;
    auto noSymlinks = [](const std::filesystem::path& path) {
      std::filesystem::path part;
      for (const auto& component : path) {
        part /= component;
        if (std::filesystem::is_symlink(std::filesystem::symlink_status(part))) return false;
      }
      return true;
    };
    auto check = [&] {
      for (const auto& link : context.sourceLinks) {
        const auto saved = context.directory / link.snapshot;
        if (!sdpContained(area, link.path) || !sdpContained(context.directory.string(), saved.string()) ||
            !noSymlinks(link.path) || !noSymlinks(saved))
          throw std::runtime_error("SDL source is missing or outside its retained context");
        const auto bytes = app.files.read(saved.string()).text;
        if (app.files.read(link.path).text != bytes)
          throw std::runtime_error("SDL source changed; regenerate the view before following source links");
        if (link.id == id) { path = link.path; snapshot = bytes; }
      }
    };
    check();
    if (path.empty() || end > snapshot.size() ||
        (static_cast<unsigned char>(snapshot[start]) & 0xc0) == 0x80 ||
        (end < snapshot.size() && (static_cast<unsigned char>(snapshot[end]) & 0xc0) == 0x80))
      throw std::runtime_error("Invalid SDL source identity or byte range");
    if (!app.open(path)) return true;
    check();
    if (app.session.view().text != snapshot)
      throw std::runtime_error("SDL source changed while opening; highlighting canceled");
    app.views->setMode(ViewMode::Editor);
    app.window->editor->setSelection(int(start), int(end - start));
    app.window->editor->setCursorPos(int(end));
    app.window->editor->makePositionVisible(int(start));
    message("SDL source selected — exact byte span from the generated snapshot");
  } catch (const std::exception& e) { message(e.what()); }
  return true;
}
} // namespace xfmd
