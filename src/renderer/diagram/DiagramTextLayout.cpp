#include "DiagramTextLayout.h"
#include <algorithm>
#include <set>
#include <sstream>
namespace xfmd {
std::map<std::string, DiagramText> DiagramTextLayout::measure(const DiagramModel& model,
                                                              ITextMetrics& metrics) {
  std::map<std::string, DiagramText> result;
  std::set<std::string> edgeLabels;
  for (const auto& e : model.edges)
    edgeLabels.insert(e.label);
  // The library keys measurements by text, so node/group titles take priority.
  for (const auto& n : model.nodes)
    edgeLabels.erase(n.label);
  for (const auto& g : model.groups)
    edgeLabels.erase(g.label);
  auto add = [&](const std::string& text) {
    if (result.count(text))
      return;
    DiagramText value;
    auto append = [&](const std::string& line) {
      auto extent = metrics.measure(line, {});
      if (!std::isfinite(extent.width) || !std::isfinite(extent.height) || extent.width < 0 ||
          extent.height <= 0)
        throw Error(ErrorCode::Layout, "Invalid diagram text metrics");
      value.lines.push_back({line, {0, value.height}, extent});
      value.width = std::max(value.width, extent.width);
      value.height += extent.height;
    };
    std::size_t begin = 0;
    do {
      auto end = text.find('\n', begin);
      auto line = text.substr(begin, end == std::string::npos ? end : end - begin);
      if (edgeLabels.count(text) && metrics.measure(line, {}).width > 120.) {
        std::istringstream words(line);
        std::string word, current;
        while (words >> word) {
          auto candidate = current.empty() ? word : current + " " + word;
          if (!current.empty() && metrics.measure(candidate, {}).width > 120.) {
            append(current);
            current = word;
          } else {
            current = std::move(candidate);
          }
        }
        append(current);
      } else {
        append(line);
      }
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
