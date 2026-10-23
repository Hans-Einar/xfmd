#pragma once
#include <string>
namespace xfmd {
enum class FileOpenKind { Markdown, Text, Browser, Desktop, Mermaid };
class FileOpenPolicy {
public:
  static FileOpenKind classify(const std::string&);
};
} // namespace xfmd
