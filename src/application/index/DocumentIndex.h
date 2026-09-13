#pragma once
#include "contracts/SemanticDocument.h"
namespace xfmd {
struct IndexHeading {
  std::string title;
  int level = 0;
  SourceAnchor anchor;
};
struct IndexLink {
  std::string label, target, path, error;
  SourceAnchor anchor;
};
struct DocumentIndex {
  DocumentToken token;
  std::vector<IndexHeading> headings;
  std::vector<IndexLink> markdown, hyperlinks;
  static DocumentIndex build(const SemanticDocument&, const std::string& path);
  static std::vector<IndexHeading> topLevel(const SemanticDocument&);
};
} // namespace xfmd
