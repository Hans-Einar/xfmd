#include "HeadingAnchor.h"
#include <set>
namespace xfmd {
namespace {
std::string lower(std::string value) {
  for (char& c : value)
    if (c >= 'A' && c <= 'Z')
      c += 'a' - 'A';
  return value;
}
std::string slug(const std::string& title) {
  std::string result;
  for (unsigned char c : lower(title)) {
    if (c == ' ' || c == '\n' || c == '\t')
      result += '-';
    else if (c >= 128 || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_')
      result += char(c);
  }
  return result;
}
} // namespace
std::optional<SourceAnchor> HeadingAnchor::find(const SemanticDocument& model,
                                                const std::string& fragment) {
  if (fragment.empty())
    return SourceAnchor{};
  std::set<std::string> used;
  const auto wanted = lower(fragment);
  for (const auto& block : model.blocks) {
    if (block.kind != BlockKind::Heading)
      continue;
    std::string title;
    for (const auto& run : block.runs)
      title += run.text;
    const auto base = slug(title);
    auto id = base;
    unsigned duplicate = 0;
    while (!used.insert(id).second)
      id = base + "-" + std::to_string(++duplicate);
    if (id == wanted || lower(title) == wanted)
      return SourceAnchor{block.source.begin};
  }
  return {};
}
} // namespace xfmd
