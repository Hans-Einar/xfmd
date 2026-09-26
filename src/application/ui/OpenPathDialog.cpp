#include "OpenPathDialog.h"
#include <memory>
using namespace FX;
namespace xfmd {
FXDEFMAP(OpenPathDialog)
openPathMap[] = {
    FXMAPFUNC(SEL_COMMAND, FXDialogBox::ID_ACCEPT, OpenPathDialog::onAccept),
    FXMAPFUNC(SEL_COMMAND, OpenPathDialog::ID_CURRENT_FOLDER, OpenPathDialog::onCurrentFolder)};
FXIMPLEMENT(OpenPathDialog, FXFileDialog, openPathMap, ARRAYNUMBER(openPathMap))
OpenPathDialog::OpenPathDialog(FXWindow* owner, const std::string& directory, const char* title,
                               const char* folderAction)
    : FXFileDialog(owner, title) {
  setSelectMode(SELECTFILE_MULTIPLE_ALL);
  setPatternList("All files (*)\nMarkdown and text (*.md,*.MD,*.txt,*.TXT)");
  setDirectory(directory.c_str());
  // FXFileSelector's acceptButton is a hidden forwarding button. Keep its native
  // visible OK button and put the selector and our footer in one vertical packer.
  auto* body = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  filebox->reparent(body);
  filebox->setLayoutHints(LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* row = new FXHorizontalFrame(body, LAYOUT_FILL_X);
  resize(600, 400);
  new FXButton(row, folderAction, nullptr, this, ID_CURRENT_FOLDER,
               BUTTON_NORMAL | LAYOUT_CENTER_Y);
  feedback = new FXLabel(row, "Select one file or folder.", nullptr,
                         JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
}
long OpenPathDialog::onAccept(FXObject* sender, FXSelector selector, void* data) {
  std::unique_ptr<FXString[]> paths(getFilenames());
  if (!paths || paths[0].empty() || !paths[1].empty()) {
    feedback->setText("Select one file or folder, or choose the current folder.");
    return 1;
  }
  selected = paths[0];
  return FXFileDialog::onCmdAccept(sender, selector, data);
}
long OpenPathDialog::onCurrentFolder(FXObject* sender, FXSelector selector, void* data) {
  selected = getDirectory();
  return FXFileDialog::onCmdAccept(sender, selector, data);
}
} // namespace xfmd
