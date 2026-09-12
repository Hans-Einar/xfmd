#include "DocumentSession.h"
#include "application/io/InputPolicy.h"
namespace xfmd {
void DocumentSession::replace(LoadedDocument loaded) {
  current = {{current.token.document + 1, 0}, std::move(loaded.text), std::move(loaded.path), loaded.plainText};
  baseline = current.text;
  file = loaded.identity;
}
Revision DocumentSession::applyEdit(const Edit& edit) {
  if (edit.begin > edit.end || edit.end > current.text.size()) throw Error(ErrorCode::InvalidInput, "Invalid edit range.");
  auto next = current.text;
  next.replace(edit.begin, edit.end - edit.begin, edit.replacement);
  InputPolicy::validate(next);
  if (next != current.text) { current.text = std::move(next); ++current.token.revision; }
  return current.token.revision;
}
void DocumentSession::markSaved(const SourceSnapshot& saved, const SavedDocument& result) {
  if (saved.token.document != current.token.document) return;
  baseline = saved.text;
  if (current.plainText != InputPolicy::plainText(result.path)) ++current.token.revision;
  current.path = result.path;
  current.plainText = InputPolicy::plainText(result.path);
  file = result.identity;
}
}
