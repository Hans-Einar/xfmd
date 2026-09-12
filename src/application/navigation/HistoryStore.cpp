#include "HistoryStore.h"
namespace xfmd {
std::optional<std::size_t> HistoryStore::propose(bool back) const {
  if (entries.empty())
    return {};
  if (back && cursor)
    return cursor - 1;
  if (!back && cursor + 1 < entries.size())
    return cursor + 1;
  return {};
}
void HistoryStore::commit(const std::string& path, SourceAnchor previous,
                          std::optional<std::size_t> target) {
  if (target && *target >= entries.size())
    throw Error(ErrorCode::InvalidInput, "Invalid history cursor.");
  if (!entries.empty())
    entries[cursor].anchor = previous;
  if (target) {
    cursor = *target;
    return;
  }
  if (!entries.empty() && entries[cursor].path == path)
    return;
  if (!entries.empty())
    entries.resize(cursor + 1);
  entries.push_back({path, {}});
  if (entries.size() > 100)
    entries.erase(entries.begin());
  cursor = entries.size() - 1;
}
void HistoryStore::renameCurrent(const std::string& path) {
  if (!entries.empty())
    entries[cursor].path = path;
}
} // namespace xfmd
