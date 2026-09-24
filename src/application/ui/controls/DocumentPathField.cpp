#include "DocumentPathField.h"
#include <FX88591Codec.h>
#include <filesystem>
#include <fxkeys.h>
using namespace FX;
namespace xfmd {
FXDEFMAP(DocumentPathField)
pathMap[] = {FXMAPFUNC(SEL_COMMAND, DocumentPathField::ID_CLEAR, DocumentPathField::onClear),
             FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, DocumentPathField::onLeftPress),
             FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, DocumentPathField::onLeftRelease),
             FXMAPFUNC(SEL_RIGHTBUTTONPRESS, 0, DocumentPathField::onRight),
             FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0, DocumentPathField::onRight),
             FXMAPFUNC(SEL_KEYPRESS, 0, DocumentPathField::onKey),
             FXMAPFUNC(SEL_FOCUSOUT, 0, DocumentPathField::onBlur),
             FXMAPFUNC(SEL_CHANGED, DocumentPathField::ID_INPUT, DocumentPathField::onInput),
             FXMAPFUNC(SEL_COMMAND, DocumentPathField::ID_INPUT, DocumentPathField::onInput),
             FXMAPFUNC(SEL_CLIPBOARD_REQUEST, 0, DocumentPathField::onClipboard),
             FXMAPFUNC(SEL_COMMAND, FXTextField::ID_COPY_SEL, DocumentPathField::onCopy)};
FXIMPLEMENT(DocumentPathField, FXTextField, pathMap, ARRAYNUMBER(pathMap))
DocumentPathField::DocumentPathField(FXComposite* parent)
    : FXTextField(parent, 1, this, ID_INPUT,
                  TEXTFIELD_NORMAL | TEXTFIELD_ENTER_ONLY | LAYOUT_FILL_X | LAYOUT_CENTER_Y) {
  setEditable(false);
  setDocument("");
}
void DocumentPathField::hint() {
  setTipText(("Document: " + (committed.empty() ? "Untitled (not saved)" : committed) +
              "\nWork folder: " + base + "\nName filter: " + (pattern.empty() ? "all" : pattern) +
              "\nType to filter; Enter opens an exact path; Escape restores. Right-click copies "
              "document path.")
                 .c_str());
}
void DocumentPathField::setDocument(const std::string& path) {
  committed = path;
  if (!editing) {
    setText(committed.empty() ? "Untitled (not saved)" : committed.c_str());
    setCursorPos(0);
  }
  hint();
}
void DocumentPathField::setWorkPath(const std::string& path) {
  base = path;
  hint();
}
void DocumentPathField::beginEditing() {
  if (editing)
    return;
  editing = true;
  previousPattern = pattern;
  setEditable(true);
  setText(committed.c_str());
  setFocus();
  selectAll();
}
void DocumentPathField::finish(bool restoreFilter) {
  if (restoreFilter) {
    pattern = previousPattern;
    if (filterChanged)
      filterChanged(pattern);
  }
  editing = false;
  setEditable(false);
  setDocument(committed);
}
void DocumentPathField::clearFilter() {
  pattern.clear();
  previousPattern.clear();
  if (filterChanged)
    filterChanged(pattern);
  finish(false);
}
long DocumentPathField::onLeftPress(FXObject* sender, FXSelector sel, void* data) {
  selectOnRelease = !editing;
  beginEditing();
  return FXTextField::onLeftBtnPress(sender, sel, data);
}
long DocumentPathField::onLeftRelease(FXObject* sender, FXSelector sel, void* data) {
  auto result = FXTextField::onLeftBtnRelease(sender, sel, data);
  if (selectOnRelease) {
    selectAll();
    selectOnRelease = false;
  }
  return result;
}
long DocumentPathField::onKey(FXObject* sender, FXSelector sel, void* data) {
  auto* e = static_cast<FXEvent*>(data);
  if (e->code == KEY_Escape && editing) {
    finish(true);
    return 1;
  }
  if (!editing && (e->code == KEY_Return || e->code == KEY_KP_Enter || !e->text.empty()))
    beginEditing();
  return FXTextField::onKeyPress(sender, sel, data);
}
long DocumentPathField::onBlur(FXObject* sender, FXSelector sel, void* data) {
  auto result = FXTextField::onFocusOut(sender, sel, data);
  if (editing && !submitting)
    finish(false);
  return result;
}
long DocumentPathField::onInput(FXObject*, FXSelector sel, void*) {
  if (!editing)
    return 1;
  if (FXSELTYPE(sel) == SEL_CHANGED) {
    pattern = std::filesystem::path(getText().text()).filename().string();
    if (filterChanged)
      filterChanged(pattern);
    hint();
  } else if (submitted) {
    const std::string target = getText().text();
    submitting = true;
    bool accepted = submitted(target);
    submitting = false;
    if (accepted)
      finish(true);
    else
      setFocus();
  }
  return 1;
}
long DocumentPathField::onRight(FXObject*, FXSelector sel, void*) {
  if (FXSELTYPE(sel) != SEL_RIGHTBUTTONRELEASE)
    return 1;
  if (committed.empty()) {
    if (feedback)
      feedback("This document has no saved path.");
    return 1;
  }
  const FXDragType types[] = {getApp()->registerDragType("UTF8_STRING"), stringType};
  if (acquireClipboard(types, 2)) {
    copied = committed;
    pathClipboard = true;
    if (feedback)
      feedback("Document path copied.");
  }
  return 1;
}
long DocumentPathField::onClipboard(FXObject* sender, FXSelector sel, void* data) {
  if (!pathClipboard)
    return FXTextField::onClipboardRequest(sender, sel, data);
  const auto target = static_cast<FXEvent*>(data)->target;
  if (target != stringType && target != getApp()->registerDragType("UTF8_STRING"))
    return 0;
  FXString text(copied.c_str());
  if (target == stringType)
    text = FX88591Codec().utf2mb(text);
  return setDNDData(FROM_CLIPBOARD, target, text);
}
long DocumentPathField::onCopy(FXObject* sender, FXSelector sel, void* data) {
  pathClipboard = false;
  return FXTextField::onCmdCopySel(sender, sel, data);
}
} // namespace xfmd
