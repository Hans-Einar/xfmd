#pragma once
#include <optional>
#include <string>
namespace xfmd {
struct DocumentLink {
  std::string path;
  std::optional<std::string> fragment;
};
class LinkResolver {
public:
  static DocumentLink documentLink(const std::string& document, const std::string& target);
  static std::string resourcePath(const std::string& document, const std::string& target);
  static std::string localPath(const std::string& document, const std::string& target);
  static std::string resolve(const std::string& document, const std::string& target);
};
} // namespace xfmd
