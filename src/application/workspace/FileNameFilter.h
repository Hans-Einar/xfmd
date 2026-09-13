#pragma once
#include <string>
namespace xfmd {
struct FileNameFilter {
  bool markdown = false, text = false;
  std::string pattern;
  bool active() const { return markdown || text || !pattern.empty(); }
  bool matches(const std::string& name) const;
};
} // namespace xfmd
