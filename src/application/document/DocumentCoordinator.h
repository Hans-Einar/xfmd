#pragma once
#include "DocumentSession.h"
#include <functional>
namespace xfmd {
enum class UnsavedChoice { Save, Discard, Cancel };
class DocumentCoordinator {
  DocumentSession& session;
  LocalFileStore& files;
public:
  std::function<UnsavedChoice()> chooseUnsaved;
  std::function<std::string()> chooseSavePath;
  std::function<void(const std::string&)> error;
  std::function<void()> opened, saved;
  DocumentCoordinator(DocumentSession& session, LocalFileStore& files) : session(session), files(files) {}
  bool resolveUnsaved();
  bool requestOpen(const std::string&);
  bool save(const std::string& target = {}, bool overwrite = false);
  bool requestClose() { return resolveUnsaved(); }
};
}
