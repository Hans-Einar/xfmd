#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace xfmd {
class WorkPathHistory {
  std::filesystem::path homePath, current;
  std::vector<std::string> recent;

public:
  explicit WorkPathHistory(std::filesystem::path home);
  void activate(const std::filesystem::path& path);
  void restore(const std::vector<std::string>& paths);
  const std::filesystem::path& root() const { return current; }
  const std::vector<std::string>& entries() const { return recent; }
  std::string displayPath(const std::filesystem::path&) const;
  std::filesystem::path broaderRoot() const;
  static bool contains(const std::filesystem::path& root, const std::filesystem::path& path);
};
} // namespace xfmd
