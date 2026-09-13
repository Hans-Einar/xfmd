#include "WorkPathHistory.h"
#include <algorithm>
#include <stdexcept>
namespace xfmd {
WorkPathHistory::WorkPathHistory(std::filesystem::path home)
    : homePath(std::filesystem::canonical(home)), current(homePath) {}
void WorkPathHistory::activate(const std::filesystem::path& path) {
  auto resolved = std::filesystem::canonical(path);
  if (!std::filesystem::is_directory(resolved))
    throw std::runtime_error("Work path is not a directory: " + path.string());
  // Verify that the root can actually be listed before changing state.
  std::filesystem::directory_iterator probe(resolved);
  current = resolved;
  auto text = current.string();
  recent.erase(std::remove(recent.begin(), recent.end(), text), recent.end());
  recent.insert(recent.begin(), text);
  if (recent.size() > 32)
    recent.resize(32);
}
void WorkPathHistory::restore(const std::vector<std::string>& paths) {
  recent.clear();
  for (const auto& path : paths) {
    if (recent.size() == 32)
      break;
    if (std::filesystem::path(path).is_absolute() &&
        std::find(recent.begin(), recent.end(), path) == recent.end())
      recent.push_back(path);
  }
}
std::filesystem::path WorkPathHistory::broaderRoot() const {
  return current == homePath || current == "/" ? std::filesystem::path("/") : homePath;
}
std::string WorkPathHistory::displayPath(const std::filesystem::path& path) const {
  if (path == homePath)
    return "~";
  if (contains(homePath, path))
    return "~/" + path.lexically_relative(homePath).string();
  return path.string();
}
bool WorkPathHistory::contains(const std::filesystem::path& root,
                               const std::filesystem::path& path) {
  auto r = root.begin(), p = path.begin();
  for (; r != root.end(); ++r, ++p)
    if (p == path.end() || *r != *p)
      return false;
  return true;
}
} // namespace xfmd
