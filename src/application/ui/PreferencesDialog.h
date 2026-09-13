#pragma once
#include "EditorWidget.h"
#include "application/preferences/PreferencesService.h"
#include <fx.h>
namespace xfmd {
class PreferencesDialog : public FX::FXDialogBox {
  FXDECLARE(PreferencesDialog)
  PreferencesService* service = nullptr;
  PreferencesDraft draft;
  FX::FXRealSpinner *speed = nullptr, *strength = nullptr, *maximum = nullptr, *margin = nullptr;
  FX::FXCheckButton* acceleration = nullptr;
  FX::FXLabel* error = nullptr;
  FX::FXComboBox* browser = nullptr;
  EditorWidget* sample = nullptr;

protected:
  PreferencesDialog() = default;

public:
  enum { Changed = FX::FXDialogBox::ID_LAST, BrowseBrowser };
  PreferencesDialog(FX::FXWindow*, PreferencesService&);
  long onChanged(FX::FXObject*, FX::FXSelector, void*);
  long onAccept(FX::FXObject*, FX::FXSelector, void*);
  long onBrowseBrowser(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
