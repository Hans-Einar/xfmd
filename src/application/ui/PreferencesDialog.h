#pragma once
#include "EditorWidget.h"
#include "application/preferences/PreferencesService.h"
#include "controls/UiLayout.h"
#include <functional>
namespace xfmd {
class PreferencesDialog : public FX::FXDialogBox {
  FXDECLARE(PreferencesDialog)
  PreferencesService* service = nullptr;
  UiContext* ui = nullptr;
  PreferencesDraft draft;
  std::function<void(const Appearance&)> preview;
  bool committed = false;
  ThemeProfiles originalProfiles, draftProfiles;
  FX::FXRealSpinner *speed = nullptr, *strength = nullptr, *maximum = nullptr, *margin = nullptr;
  FX::FXSpinner* fontSize = nullptr;
  FX::FXCheckButton* acceleration = nullptr;
  FX::FXLabel* error = nullptr;
  FX::FXComboBox *browser = nullptr, *theme = nullptr, *density = nullptr, *buttons = nullptr;
  EditorWidget* sample = nullptr;
  void buildAppearance(FX::FXComposite*);
  void buildScrolling(FX::FXComposite*);
  void buildDocument(FX::FXComposite*);
  void buildPrograms(FX::FXComposite*);
  void apply(const Appearance&);
  void restore();

protected:
  PreferencesDialog() = default;

public:
  enum {
    Changed = FX::FXDialogBox::ID_LAST,
    BrowseBrowser,
    BrowserChanged,
    ThemeChanged,
    DensityChanged,
    ButtonsChanged,
    FontChanged,
    ReloadStyle,
    SampleToggle
  };
  PreferencesDialog(FX::FXWindow*, PreferencesService&, UiContext&,
                    std::function<void(const Appearance&)> = {});
  ~PreferencesDialog() override;
  long onChanged(FX::FXObject*, FX::FXSelector, void*);
  long onAppearance(FX::FXObject*, FX::FXSelector, void*);
  long onSampleToggle(FX::FXObject*, FX::FXSelector, void*);
  long onReload(FX::FXObject*, FX::FXSelector, void*);
  long onAccept(FX::FXObject*, FX::FXSelector, void*);
  long onCancel(FX::FXObject*, FX::FXSelector, void*);
  long onBrowseBrowser(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
