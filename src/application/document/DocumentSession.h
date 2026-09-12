#pragma once
#include "application/io/LocalFileStore.h"
namespace xfmd {
class DocumentSession {
  SourceSnapshot current{{1, 0}, {}, {}, false};
  std::string baseline;
  std::optional<FileIdentity> file;
public:
  const SourceSnapshot& view() const { return current; }
  SourceSnapshot snapshot() const { return current; }
  bool dirty() const { return current.text != baseline; }
  const std::optional<FileIdentity>& identity() const { return file; }
  void replace(LoadedDocument);
  Revision applyEdit(const Edit&);
  void markSaved(const SourceSnapshot&, const SavedDocument&);
};
}
