#include "LinkResolver.h"
#include "application/io/InputPolicy.h"
#include <cctype>
#include <filesystem>
namespace xfmd {
namespace {
void localOnly(const std::string& target) {
  if (target.empty() || target.compare(0, 2, "//") == 0)
    throw Error(ErrorCode::Unsupported, "Only local document links are supported.");
  auto colon = target.find(':'), slash = target.find('/');
  if (colon != std::string::npos && (slash == std::string::npos || colon < slash))
    throw Error(ErrorCode::Unsupported, "External URLs and URI schemes are not opened by xfmd.");
  for (unsigned char c : target)
    if (c < 32 || c == 127)
      throw Error(ErrorCode::InvalidInput, "Control character in link.");
}
int hex(unsigned char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return -1;
}
std::string decode(const std::string& target) {
  std::string decoded;
  for (std::size_t i = 0; i < target.size(); ++i) {
    if (target[i] != '%') {
      decoded += target[i];
      continue;
    }
    if (i + 2 >= target.size() || hex(target[i + 1]) < 0 || hex(target[i + 2]) < 0)
      throw Error(ErrorCode::InvalidInput, "Invalid percent escape in link.");
    decoded += char(hex(target[i + 1]) * 16 + hex(target[i + 2]));
    i += 2;
  }
  return decoded;
}
} // namespace
DocumentLink LinkResolver::documentLink(const std::string& document, const std::string& target) {
  const auto hash = target.find('#');
  if (hash == std::string::npos)
    return {resourcePath(document, target), {}};
  auto fragment = decode(target.substr(hash + 1));
  InputPolicy::validate(fragment);
  for (unsigned char c : fragment)
    if (c < 32 || c == 127)
      throw Error(ErrorCode::InvalidInput, "Control character in heading anchor.");
  if (hash == 0)
    return {document, std::move(fragment)};
  return {resourcePath(document, target.substr(0, hash)), std::move(fragment)};
}
std::string LinkResolver::resourcePath(const std::string& document, const std::string& target) {
  localOnly(target);
  if (target.find('#') != std::string::npos || target.find('?') != std::string::npos)
    throw Error(ErrorCode::Unsupported,
                "Fragment and query links are not supported in this version.");
  auto decoded = decode(target);
  localOnly(decoded);
  InputPolicy::validate(decoded);
  std::filesystem::path path(decoded);
  if (!path.is_absolute()) {
    if (document.empty())
      throw Error(ErrorCode::Unsupported, "Save this document before following relative links.");
    path = std::filesystem::path(document).parent_path() / path;
  }
  return path.lexically_normal().string();
}
std::string LinkResolver::localPath(const std::string& document, const std::string& target) {
  auto path = resourcePath(document, target);
  InputPolicy::validate({}, path);
  return path;
}
std::string LinkResolver::resolve(const std::string& document, const std::string& target) {
  return std::filesystem::weakly_canonical(localPath(document, target)).string();
}
} // namespace xfmd
