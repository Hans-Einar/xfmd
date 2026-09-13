#include "PreferencesDialog.h"
#include "application/adapters/FoxWheelScrollBar.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(PreferencesDialog)
preferencesMap[] = {
    FXMAPFUNC(SEL_COMMAND, FXDialogBox::ID_ACCEPT, PreferencesDialog::onAccept),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::BrowseBrowser, PreferencesDialog::onBrowseBrowser),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::Changed, PreferencesDialog::onChanged),
    FXMAPFUNC(SEL_CHANGED, PreferencesDialog::Changed, PreferencesDialog::onChanged)};
FXIMPLEMENT(PreferencesDialog, FXDialogBox, preferencesMap, ARRAYNUMBER(preferencesMap))
PreferencesDialog::PreferencesDialog(FXWindow* owner, PreferencesService& preferences)
    : FXDialogBox(owner, "Preferences", DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE, 0, 0, 520, 480),
      service(&preferences), draft(preferences.begin()) {
  auto* content = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* grid = new FXMatrix(content, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
  auto number = [&](const char* label, double value, double min, double max, double step) {
    new FXLabel(grid, label, nullptr, JUSTIFY_LEFT);
    auto* input = new FXRealSpinner(grid, 8, this, Changed, REALSPIN_NORMAL | LAYOUT_FILL_X);
    input->setRange(min, max);
    input->setIncrement(step);
    input->setValue(value);
    return input;
  };
  speed = number("Scroll speed", draft.scroll.speed, .25, 4, .25);
  strength = number("Acceleration strength", draft.scroll.strength, 0, 2, .1);
  maximum = number("Maximum acceleration", draft.scroll.maxGain, 1, 5, .25);
  acceleration = new FXCheckButton(content, "Enable scroll acceleration", this, Changed);
  acceleration->setCheck(draft.scroll.acceleration);
  margin = number("A4 margin (mm)", draft.marginMm, 5, 50, 1);
  auto* programs = new FXGroupBox(content, "Hyperlinks", GROUPBOX_NORMAL | LAYOUT_FILL_X);
  auto* browserRow = new FXHorizontalFrame(programs, LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);
  new FXLabel(browserRow, "Browser program", nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
  browser = new FXComboBox(browserRow, 24, this, Changed, COMBOBOX_NORMAL | LAYOUT_FILL_X);
  browser->appendItem("xdg-open");
  browser->appendItem("google-chrome-stable");
  browser->appendItem("firefox");
  browser->setNumVisible(3);
  browser->setText(draft.browserProgram.c_str());
  browser->setTipText("Executable name or full path; the link is passed automatically.");
  new FXButton(browserRow, "Browse…", nullptr, this, BrowseBrowser, BUTTON_NORMAL);
  new FXLabel(programs, "xdg-open uses the system default browser.", nullptr, JUSTIFY_LEFT);
  new FXLabel(content, "Try scrolling here — the document stays unchanged.");
  sample = new EditorWidget(content);
  sample->setEditable(false);
  sample->setHeight(150);
  sample->setLayoutHints(LAYOUT_FILL_X | LAYOUT_FILL_Y);
  FXString text;
  for (int i = 1; i <= 100; ++i) {
    FXString row;
    row.format("Scroll sample %03d — slow and fast movement\n", i);
    text += row;
  }
  sample->setText(text);
  error = new FXLabel(content, "", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
  error->setTextColor(FXRGB(170, 40, 40));
  auto* buttons = new FXHorizontalFrame(content, LAYOUT_FILL_X | PACK_UNIFORM_WIDTH);
  new FXButton(buttons, "&Cancel", nullptr, this, ID_CANCEL, BUTTON_NORMAL | LAYOUT_RIGHT);
  new FXButton(buttons, "&OK", nullptr, this, ID_ACCEPT,
               BUTTON_INITIAL | BUTTON_DEFAULT | BUTTON_NORMAL | LAYOUT_RIGHT);
  onChanged(nullptr, 0, nullptr);
}
long PreferencesDialog::onChanged(FXObject*, FXSelector, void*) {
  draft.scroll = {speed->getValue(), bool(acceleration->getCheck()), strength->getValue(),
                  maximum->getValue()};
  draft.marginMm = margin->getValue();
  draft.browserProgram = browser->getText().text();
  FoxWheelScrollBar::configureTree(sample, draft.scroll);
  return 1;
}
long PreferencesDialog::onBrowseBrowser(FXObject*, FXSelector, void*) {
  auto path =
      FXFileDialog::getOpenFilename(this, "Select browser program", "/usr/bin/", "All files (*)");
  if (!path.empty()) {
    browser->setText(path);
    onChanged(nullptr, 0, nullptr);
  }
  return 1;
}
long PreferencesDialog::onAccept(FXObject*, FXSelector, void*) {
  onChanged(nullptr, 0, nullptr);
  std::string message;
  if (!service->commit(draft, message)) {
    error->setText(message.c_str());
    return 1;
  }
  return FXDialogBox::onCmdAccept(this, FXSEL(SEL_COMMAND, ID_ACCEPT), nullptr);
}
} // namespace xfmd
