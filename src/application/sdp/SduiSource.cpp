#include "SdpNavigation.h"
#include "application/Application.h"
#include <charconv>
namespace xfmd {
std::optional<SdpTarget> SdpNavigation::currentSdui() const {
  auto context = contexts.find(app.session.view().path);
  if (context == contexts.end() || context->second.root != root)
    return {};
  const auto& target = context->second.target;
  auto source = sources.find(target.model);
  if (target.operation != "sdui-preview" || target.entry.empty() || target.project != project ||
      source == sources.end() || source->second.kind != "sdui" ||
      source->second.revision != target.revision || !sdpContained(area, target.path))
    return {};
  return target;
}
bool SdpNavigation::followSduiSource(const std::string& document, const std::string& uri) {
  try {
    const auto it = contexts.find(document);
    if (it == contexts.end() || it->second.root != root ||
        it->second.target.operation != "sdui-preview")
      throw std::runtime_error("SDUI source link has no current project context");
    const auto& context = it->second;
    const auto source = sources.find(context.target.model);
    if (source == sources.end() || source->second.revision != context.target.revision ||
        !sdpContained(area, context.target.path))
      throw std::runtime_error(
          "SDUI source changed; regenerate the preview before following source links");
    const auto snapshot = app.files.read((context.directory / "source.sdui").string()).text;
    const auto current = app.files.read(context.target.path).text;
    if (snapshot != current)
      throw std::runtime_error(
          "SDUI source changed; regenerate the preview before following source links");
    const auto range = uri.substr(14);
    const auto slash = range.find('/');
    std::size_t start = 0, end = 0;
    auto number = [](const std::string& s, std::size_t& value) {
      const auto result = std::from_chars(s.data(), s.data() + s.size(), value);
      return !s.empty() && result.ec == std::errc() && result.ptr == s.data() + s.size();
    };
    if (slash == std::string::npos || !number(range.substr(0, slash), start) ||
        !number(range.substr(slash + 1), end) || start >= end || end > current.size() ||
        (static_cast<unsigned char>(current[start]) & 0xc0) == 0x80 ||
        (end < current.size() && (static_cast<unsigned char>(current[end]) & 0xc0) == 0x80))
      throw std::runtime_error("Invalid SDUI source span");
    if (!app.open(context.target.path))
      return true;
    if (app.session.view().text != snapshot)
      throw std::runtime_error("SDUI source changed while opening; highlighting canceled");
    app.views->setMode(ViewMode::Editor);
    app.window->editor->setSelection(int(start), int(end - start));
    app.window->editor->setCursorPos(int(end));
    app.window->editor->makePositionVisible(int(start));
    message("SDUI source selected — exact byte span from preview revision");
  } catch (const std::exception& e) {
    message(e.what());
  }
  return true;
}
} // namespace xfmd
