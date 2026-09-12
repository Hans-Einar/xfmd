#include "EditController.h"
#include "TextProjection.h"
namespace xfmd {
void EditController::apply(const Edit& edit, bool record) {
  auto previous = session.view().token;
  auto removed = session.view().text.substr(edit.begin, edit.end - edit.begin);
  session.applyEdit(edit);
  if (previous == session.view().token) return;
  if (record) {
    undoStack.push_back({edit, {edit.begin, edit.begin + edit.replacement.size(), removed}});
    historyBytes += removed.size() + edit.replacement.size();
    redoStack.clear();
    while (historyBytes > 32 * 1024 * 1024 && undoStack.size() > 1) {
      historyBytes -= undoStack.front().forward.replacement.size() + undoStack.front().backward.replacement.size();
      undoStack.erase(undoStack.begin());
    }
  }
  if (changed) changed();
}
void EditController::applyEdit(const Edit& edit) { apply(edit, true); }
void EditController::applyProjectedText(const std::string& text) { applyEdit(TextProjection(session.view().text).difference(text)); }
void EditController::undo() {
  if (undoStack.empty()) return;
  auto change = undoStack.back();
  apply(change.backward, false);
  undoStack.pop_back(); redoStack.push_back(std::move(change));
}
void EditController::redo() {
  if (redoStack.empty()) return;
  auto change = redoStack.back();
  apply(change.forward, false);
  redoStack.pop_back(); undoStack.push_back(std::move(change));
}
void EditController::reset() { undoStack.clear(); redoStack.clear(); historyBytes = 0; }
std::size_t EditController::find(const std::string& needle, std::size_t from) const {
  auto text = TextProjection(session.view().text).text;
  auto result = text.find(needle, from);
  return result == std::string::npos ? text.find(needle) : result;
}
}
