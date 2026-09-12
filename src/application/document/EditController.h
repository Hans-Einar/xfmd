#pragma once
#include "DocumentSession.h"
#include <functional>
#include <vector>
namespace xfmd {
class EditController {
  DocumentSession& session;
  struct Change {
    Edit forward, backward;
  };
  std::vector<Change> undoStack, redoStack;
  std::size_t historyBytes = 0;
  void apply(const Edit&, bool record);

public:
  std::function<void()> changed;
  explicit EditController(DocumentSession& session) : session(session) {}
  void applyEdit(const Edit&);
  void applyProjectedText(const std::string&);
  void undo();
  void redo();
  void reset();
  bool canUndo() const { return !undoStack.empty(); }
  bool canRedo() const { return !redoStack.empty(); }
  std::size_t find(const std::string&, std::size_t from) const;
};
} // namespace xfmd
