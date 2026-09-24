#pragma once
#include <string>
namespace xfmd {
enum class FileOpenKind { Markdown, Text, Browser, Desktop };
class FileOpenPolicy {
public:
  static FileOpenKind classify(const std::string&);
};
} // namespace xfmd
