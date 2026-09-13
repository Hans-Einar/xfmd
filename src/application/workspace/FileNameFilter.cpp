#include "FileNameFilter.h"
#include <vector>
namespace xfmd {
namespace {
std::vector<std::string> characters(const std::string& value) {
  std::vector<std::string> result;
  for (unsigned char c : value) {
    if ((c & 0xc0) == 0x80 && !result.empty())
      result.back() += char(c);
    else
      result.emplace_back(1, c >= 'A' && c <= 'Z' ? char(c + 'a' - 'A') : char(c));
  }
  return result;
}
bool wildcard(const std::string& pattern, const std::string& value) {
  const auto p = characters(pattern), v = characters(value);
  std::size_t i = 0, j = 0, star = std::string::npos, retry = 0;
  while (j < v.size()) {
    if (i < p.size() && p[i] == "*") {
      star = i++;
      retry = j;
    } else if (i < p.size() && (p[i] == "?" || p[i] == v[j])) {
      ++i;
      ++j;
    } else if (star != std::string::npos) {
      i = star + 1;
      j = ++retry;
    } else
      return false;
  }
  while (i < p.size() && p[i] == "*")
    ++i;
  return i == p.size();
}
} // namespace
bool FileNameFilter::matches(const std::string& name) const {
  if ((markdown || text) &&
      !((markdown && wildcard("*.md", name)) || (text && wildcard("*.txt", name))))
    return false;
  if (pattern.empty())
    return true;
  return wildcard(pattern.find_first_of("*?") == std::string::npos ? "*" + pattern + "*" : pattern,
                  name);
}
} // namespace xfmd
