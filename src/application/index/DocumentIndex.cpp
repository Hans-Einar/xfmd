#include "DocumentIndex.h"
#include "application/navigation/LinkResolver.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <set>
namespace xfmd {
namespace {
std::string title(const std::vector<InlineRun>& runs) {
  std::string text;
  for (const auto& run : runs)
    text += run.text;
  std::replace(text.begin(), text.end(), '\n', ' ');
  return text.empty() ? "(Untitled)" : text;
}
void collect(const std::vector<InlineRun>& runs, const std::string& document,
             DocumentIndex& index, std::set<std::string>& seenFiles,
             std::set<std::string>& seenLinks) {
  for (std::size_t i = 0; i < runs.size(); ++i) {
    const auto& run = runs[i];
    if (run.link.empty())
      continue;
    IndexLink link{run.text, run.link, {}, {}, {run.source.begin}};
    while (i + 1 < runs.size() && runs[i + 1].link == link.target &&
           runs[i + 1].linkId == run.linkId)
      link.label += runs[++i].text;
    const auto base = link.target.substr(0, link.target.find_first_of("#?"));
    // Decode and normalize without touching the filesystem on the GUI thread.
    try {
      link.path = LinkResolver::localPath(document, base);
      auto extension = std::filesystem::path(link.path).extension().string();
      std::transform(extension.begin(), extension.end(), extension.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      if (extension != ".md")
        link.path.clear();
    } catch (const std::exception& e) {
      auto extension = std::filesystem::path(base).extension().string();
      if (extension == ".md" && base.find(':') == std::string::npos &&
          base.compare(0, 2, "//") != 0)
        link.error = e.what();
    }
    if (!link.path.empty() || !link.error.empty()) {
      if (seenFiles.insert(link.path.empty() ? link.target : link.path).second)
        index.markdown.push_back(std::move(link));
    } else if (seenLinks.insert(link.target).second)
      index.hyperlinks.push_back(std::move(link));
  }
}
} // namespace
DocumentIndex DocumentIndex::build(const SemanticDocument& model, const std::string& path) {
  DocumentIndex index;
  index.token = model.token;
  std::set<std::string> files, links;
  for (const auto& block : model.blocks) {
    if (block.kind == BlockKind::Heading)
      index.headings.push_back({title(block.runs), block.level, {block.source.begin}});
    collect(block.runs, path, index, files, links);
    if (block.table)
      for (const auto& row : block.table->rows)
        for (const auto& cell : row.cells)
          collect(cell.runs, path, index, files, links);
  }
  return index;
}
std::vector<IndexHeading> DocumentIndex::topLevel(const SemanticDocument& model) {
  std::vector<IndexHeading> headings;
  int level = 7;
  for (const auto& block : model.blocks) {
    if (block.kind != BlockKind::Heading || block.level > level)
      continue;
    if (block.level < level) {
      headings.clear();
      level = block.level;
    }
    headings.push_back({title(block.runs), block.level, {block.source.begin}});
  }
  return headings;
}
} // namespace xfmd
