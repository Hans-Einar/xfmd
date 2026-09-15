#include "DiagramTextLayout.h"
#include <algorithm>
namespace xfmd {
std::map<std::string, DiagramText> DiagramTextLayout::measure(const DiagramModel& model,
                                                              ITextMetrics& metrics) {
  std::map<std::string, DiagramText> result;
  auto add = [&](const std::string& text) {
    if (result.count(text))
      return;
    DiagramText value;
    std::size_t begin = 0;
    do {
      auto end = text.find('\n', begin);
      auto line = text.substr(begin, end == std::string::npos ? end : end - begin);
      auto extent = metrics.measure(line, {});
      if (!std::isfinite(extent.width) || !std::isfinite(extent.height) || extent.width < 0 ||
          extent.height <= 0)
        throw Error(ErrorCode::Layout, "Invalid diagram text metrics");
      value.lines.push_back({line, {0, value.height}, extent});
      value.width = std::max(value.width, extent.width);
      value.height += extent.height;
      if (end == std::string::npos)
        break;
      begin = end + 1;
    } while (begin <= text.size());
    result.emplace(text, std::move(value));
  };
  add("");
  for (const auto& n : model.nodes)
    add(n.label);
  for (const auto& e : model.edges)
    add(e.label);
  for (const auto& g : model.groups)
    add(g.label);
  return result;
}
} // namespace xfmd
