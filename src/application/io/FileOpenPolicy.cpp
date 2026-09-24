#include "FileOpenPolicy.h"
#include "InputPolicy.h"
#include "LocalFileStore.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
namespace xfmd {
FileOpenKind FileOpenPolicy::classify(const std::string& path) {
  if (!std::filesystem::is_regular_file(path))
    throw Error(ErrorCode::Unsupported, "Only regular files can be opened.");
  auto extension = std::filesystem::path(path).extension().string();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 [](unsigned char c) { return char(std::tolower(c)); });
  if (extension == ".html" || extension == ".htm")
    return FileOpenKind::Browser;
  if (InputPolicy::supportedPath(path))
    return InputPolicy::plainText(path) ? FileOpenKind::Text : FileOpenKind::Markdown;
  try {
    LocalFileStore().read(path);
    return FileOpenKind::Text;
  } catch (const Error& e) {
    if (e.code == ErrorCode::InvalidInput || e.code == ErrorCode::TooLarge)
      return FileOpenKind::Desktop;
    throw;
  }
}
} // namespace xfmd
