#pragma once
#include <fx.h>
#include <string>
namespace xfmd {
class OpenPathDialog : public FX::FXFileDialog {
  FXDECLARE(OpenPathDialog)
  FX::FXString selected;
  FX::FXLabel* feedback = nullptr;

protected:
  OpenPathDialog() = default;

public:
  enum { ID_CURRENT_FOLDER = FX::FXFileDialog::ID_LAST, ID_LAST };
  OpenPathDialog(FX::FXWindow*, const std::string& directory,
                 const char* title = "Open file or folder",
                 const char* folderAction = "Open current &folder");
  const FX::FXString& selectedPath() const { return selected; }
  long onAccept(FX::FXObject*, FX::FXSelector, void*);
  long onCurrentFolder(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
