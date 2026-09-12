#pragma once
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace xfmd {
using DocumentId = std::uint64_t;
using Revision = std::uint64_t;
struct DocumentToken {
  DocumentId document = 0;
  Revision revision = 0;
  bool operator==(const DocumentToken& other) const {
    return document == other.document && revision == other.revision;
  }
  bool operator!=(const DocumentToken& other) const { return !(*this == other); }
};
struct SourceSnapshot {
  DocumentToken token;
  std::string text;
  std::string path;
  bool plainText = false;
};
enum class MappingQuality { Exact, Approximate, Unavailable };
struct SourceRange {
  std::size_t begin = 0, end = 0;
  MappingQuality quality = MappingQuality::Exact;
};
struct SourceAnchor {
  std::size_t byte = 0;
  double fraction = 0;
  MappingQuality quality = MappingQuality::Exact;
};
struct Edit {
  std::size_t begin = 0, end = 0;
  std::string replacement;
};
enum class ErrorCode { Io, InvalidInput, TooLarge, Conflict, Unsupported, Parse, Layout };
class Error : public std::runtime_error {
public:
  ErrorCode code;
  Error(ErrorCode code, const std::string& message) : std::runtime_error(message), code(code) {}
};
constexpr std::size_t maxDocumentBytes = 8 * 1024 * 1024;
} // namespace xfmd
