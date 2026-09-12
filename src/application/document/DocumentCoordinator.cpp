#include "DocumentCoordinator.h"
namespace xfmd {
bool DocumentCoordinator::resolveUnsaved() {
  if (!session.dirty()) return true;
  auto choice = chooseUnsaved ? chooseUnsaved() : UnsavedChoice::Cancel;
  if (choice == UnsavedChoice::Save) return save();
  return choice == UnsavedChoice::Discard;
}
bool DocumentCoordinator::requestOpen(const std::string& path) {
  if (path.empty() || !resolveUnsaved()) return false;
  try { auto candidate = files.read(path); session.replace(std::move(candidate)); }
  catch (const std::exception& e) { if (error) error(e.what()); return false; }
  if (opened) opened();
  return true;
}
bool DocumentCoordinator::save(const std::string& target, bool overwrite) {
  auto source = session.snapshot();
  auto path = target.empty() ? source.path : target;
  if (path.empty() && chooseSavePath) path = chooseSavePath();
  if (path.empty()) return false;
  try {
    auto expected = path == source.path ? session.identity() : std::optional<FileIdentity>{};
    if (overwrite && path != source.path) expected = files.read(path).identity;
    auto result = files.writeAtomic(source, path, expected);
    session.markSaved(source, result);
    if (!result.durable && error) error("File saved, but directory flush failed; durability is uncertain.");
  } catch (const std::exception& e) { if (error) error(e.what()); return false; }
  if (saved) saved();
  return true;
}
}
