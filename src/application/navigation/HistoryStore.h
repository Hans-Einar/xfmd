#pragma once
#include "contracts/DocumentTypes.h"
#include <optional>
#include <vector>
namespace xfmd {
struct HistoryEntry { std::string path; SourceAnchor anchor; };
class HistoryStore {
  std::vector<HistoryEntry> entries;
  std::size_t cursor = 0;
public:
  std::optional<std::size_t> propose(bool back) const;
  void commit(const std::string&, SourceAnchor previous, std::optional<std::size_t> target = {});
  void renameCurrent(const std::string&);
  const HistoryEntry& at(std::size_t index) const { return entries.at(index); }
  std::size_t size() const { return entries.size(); }
  std::size_t position() const { return cursor; }
};
}
