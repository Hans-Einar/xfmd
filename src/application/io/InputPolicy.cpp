#include "InputPolicy.h"
#include <algorithm>
#include <filesystem>
namespace xfmd {
namespace {
std::string extension(const std::string& path) {
  auto ext = std::filesystem::path(path).extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return char(std::tolower(c)); });
  return ext;
}
} // namespace
bool InputPolicy::supportedPath(const std::string& path) {
  auto ext = extension(path);
  return ext == ".md" || ext == ".txt";
}
bool InputPolicy::plainText(const std::string& path) { return extension(path) == ".txt"; }
void InputPolicy::validate(std::string_view text, const std::string& path) {
  if (!path.empty() && !supportedPath(path))
    throw Error(ErrorCode::Unsupported, "Only .md and .txt files are supported.");
  if (text.size() > maxDocumentBytes)
    throw Error(ErrorCode::TooLarge, "File exceeds the 8 MiB limit.");
  for (std::size_t i = 0; i < text.size();) {
    auto c = static_cast<unsigned char>(text[i++]);
    if (c == 0)
      throw Error(ErrorCode::InvalidInput, "NUL bytes cannot be edited.");
    if (c < 0x80)
      continue;
    unsigned value = 0, count = 0, minimum = 0;
    if (c >= 0xc2 && c <= 0xdf) {
      count = 1;
      value = c & 31;
      minimum = 0x80;
    } else if (c >= 0xe0 && c <= 0xef) {
      count = 2;
      value = c & 15;
      minimum = 0x800;
    } else if (c >= 0xf0 && c <= 0xf4) {
      count = 3;
      value = c & 7;
      minimum = 0x10000;
    } else
      throw Error(ErrorCode::InvalidInput, "Input is not valid UTF-8.");
    if (i + count > text.size())
      throw Error(ErrorCode::InvalidInput, "Incomplete UTF-8 sequence.");
    while (count--) {
      auto next = static_cast<unsigned char>(text[i++]);
      if ((next & 0xc0) != 0x80)
        throw Error(ErrorCode::InvalidInput, "Input is not valid UTF-8.");
      value = (value << 6) | (next & 63);
    }
    if (value < minimum || value > 0x10ffff || (value >= 0xd800 && value <= 0xdfff))
      throw Error(ErrorCode::InvalidInput, "Invalid UTF-8 code point.");
  }
}
} // namespace xfmd
