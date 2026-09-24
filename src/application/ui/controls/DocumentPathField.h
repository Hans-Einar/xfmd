#pragma once
#include <functional>
#include <fx.h>
#include <string>
namespace xfmd {
// Editing and clipboard adapter; Application owns target resolution/opening.
class DocumentPathField : public FX::FXTextField {
  FXDECLARE(DocumentPathField)
  std::string committed, base, pattern, previousPattern, copied;
  bool editing = false, submitting = false, selectOnRelease = false, pathClipboard = false;
  void finish(bool restoreFilter);
  void hint();

protected:
  DocumentPathField() = default;

public:
  enum { ID_INPUT = FX::FXTextField::ID_LAST, ID_CLEAR, ID_LAST };
  std::function<void(const std::string&)> filterChanged;
  std::function<bool(const std::string&)> submitted;
  std::function<void(const std::string&)> feedback;
  explicit DocumentPathField(FX::FXComposite*);
  void setDocument(const std::string&);
  void setWorkPath(const std::string&);
  void beginEditing();
  void clearFilter();
  bool isEditing() const { return editing; }
  const std::string& filter() const { return pattern; }
  long onLeftPress(FX::FXObject*, FX::FXSelector, void*);
  long onLeftRelease(FX::FXObject*, FX::FXSelector, void*);
  long onRight(FX::FXObject*, FX::FXSelector, void*);
  long onKey(FX::FXObject*, FX::FXSelector, void*);
  long onBlur(FX::FXObject*, FX::FXSelector, void*);
  long onInput(FX::FXObject*, FX::FXSelector, void*);
  long onClipboard(FX::FXObject*, FX::FXSelector, void*);
  long onClear(FX::FXObject*, FX::FXSelector, void*) {
    clearFilter();
    return 1;
  }
  long onCopy(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
