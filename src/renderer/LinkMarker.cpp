#include "LinkMarker.h"
#include <algorithm>
#include <cctype>
namespace xfmd {
std::optional<DrawRun> LinkMarker::make(const InlineRun& link, FontSpec font,
                                        ITextMetrics& metrics) {
  std::string target = link.link;
  std::transform(target.begin(), target.end(), target.begin(),
                 [](unsigned char c) { return char(std::tolower(c)); });
  const bool web = target.rfind("https://", 0) == 0 || target.rfind("http://", 0) == 0 ||
                   target.rfind("//", 0) == 0;
  auto end = target.find_first_of("?#");
  auto path = target.substr(0, end);
  bool markdown = path.size() >= 3 && path.compare(path.size() - 3, 3, ".md") == 0;
  if (!web && !markdown)
    return {};
  font.bold = false;
  font.italic = false;
  font.mono = false;
  DrawRun marker;
  marker.font = font;
  marker.link = link.link;
  marker.source = {link.source.begin, link.source.begin, MappingQuality::Approximate};
  marker.text = web ? "" : (target.front() == '/' ? "/# " : "# ");
  marker.icon = web ? InlineIcon::Globe : InlineIcon::None;
  auto extent = metrics.measure(web ? "M " : marker.text, font);
  if (web)
    extent.width = extent.height + metrics.measure(" ", font).width;
  marker.bounds = {0, 0, extent.width, extent.height};
  marker.ascent = extent.ascent;
  if (!web)
    marker.shaped = extent.shaped;
  return marker;
}
} // namespace xfmd
