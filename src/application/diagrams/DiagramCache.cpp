#include "DiagramCache.h"
namespace xfmd {
std::shared_ptr<const DiagramScene> DiagramCache::find(const std::string& key) {
  for (auto i = entries.begin(); i != entries.end(); ++i)
    if (i->key == key) {
      auto result = i->scene;
      entries.splice(entries.begin(), entries, i);
      return result;
    }
  return {};
}
void DiagramCache::insert(std::string key, std::shared_ptr<const DiagramScene> scene) {
  constexpr std::size_t limit = 32 * 1024 * 1024;
  auto cost = key.size() + scene->bytes;
  if (cost > limit)
    return;
  while (!entries.empty() && size + cost > limit) {
    size -= entries.back().key.size() + entries.back().scene->bytes;
    entries.pop_back();
  }
  size += cost;
  entries.push_front({std::move(key), std::move(scene)});
}
} // namespace xfmd
