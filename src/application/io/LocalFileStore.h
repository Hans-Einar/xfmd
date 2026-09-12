#pragma once
#include "contracts/DocumentTypes.h"
#include <functional>
#include <optional>
namespace xfmd {
struct FileIdentity {
  std::uint64_t device = 0, inode = 0, hash = 0;
  std::int64_t modified = 0, changed = 0;
  std::size_t size = 0;
  bool operator==(const FileIdentity&) const;
};
struct LoadedDocument {
  std::string path, text;
  FileIdentity identity;
  bool plainText = false;
};
struct SavedDocument {
  FileIdentity identity;
  bool durable = true;
  std::string path;
};
class LocalFileStore {
public:
  std::function<void(const char*)> checkpoint; // Optional fault injection; empty in production.
  virtual ~LocalFileStore() = default;
  virtual LoadedDocument read(const std::string&) const;
  virtual SavedDocument writeAtomic(const SourceSnapshot&, const std::string&,
                                    const std::optional<FileIdentity>& expected) const;
};
} // namespace xfmd
