#include "PreferencesDialog.h"
using namespace FX;
namespace xfmd {
void PreferencesDialog::buildAppearance(FXComposite* parent) {
  auto* form = new UiForm(parent, *ui);
  auto choice = [&](const char* label, FXSelector id, const char* first, const char* second,
                    bool selected) {
    new FXLabel(form, label, nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
    auto* value = new FXComboBox(form, 24, this, id,
                                 COMBOBOX_STATIC | FRAME_SUNKEN | FRAME_THICK | LAYOUT_FILL_X);
    value->appendItem(first);
    value->appendItem(second);
    value->setNumVisible(2);
    value->setCurrentItem(selected ? 1 : 0);
    return value;
  };
  theme = choice("Theme", ThemeChanged, "Light", "Dark", draft.appearance.theme == "dark");
  density = choice("Spacing", DensityChanged, "Comfortable", "Compact", draft.appearance.compact);
  buttons =
      choice("Buttons", ButtonsChanged, "Flat", "Classic", draft.appearance.buttons == "classic");
  new FXLabel(form, "UI font size", nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
  fontSize = new FXSpinner(form, 8, this, FontChanged, SPIN_NORMAL | LAYOUT_FILL_X);
  fontSize->setRange(8, 18);
  fontSize->setValue(draft.appearance.fontSize);
  UiFactory factory(*ui);
  auto* row = factory.row(parent);
  factory.button(row, "Reload style file", this, ReloadStyle, UiIcon::Refresh, ButtonRole::Normal)
      ->setTipText(ui->profilePath().c_str());
  auto* examples = factory.row(parent);
  factory.button(examples, "Normal", this, SampleToggle, UiIcon::Open, ButtonRole::Normal);
  auto* selected =
      factory.button(examples, "Selected", this, SampleToggle, UiIcon::Sidebar, ButtonRole::Normal);
  selected->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_CHECK), nullptr);
  factory.button(examples, "Disabled", nullptr, 0, UiIcon::Save, ButtonRole::Normal)->disable();
  new FXLabel(parent, "Preview changes here. Cancel restores your appearance.", nullptr,
              JUSTIFY_LEFT);
  new FXLabel(parent, "Document typography and PDF output keep their own settings.", nullptr,
              JUSTIFY_LEFT);
}
void PreferencesDialog::buildScrolling(FXComposite* parent) {
  auto* form = new UiForm(parent, *ui);
  auto number = [&](const char* label, double value, double min, double max, double step) {
    new FXLabel(form, label, nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
    auto* input = new FXRealSpinner(form, 8, this, Changed, REALSPIN_NORMAL | LAYOUT_FILL_X);
    input->setRange(min, max);
    input->setIncrement(step);
    input->setValue(value);
    return input;
  };
  speed = number("Scroll speed", draft.scroll.speed, .25, 4, .25);
  strength = number("Acceleration strength", draft.scroll.strength, 0, 2, .1);
  maximum = number("Maximum acceleration", draft.scroll.maxGain, 1, 5, .25);
  acceleration = new FXCheckButton(parent, "Enable scroll acceleration", this, Changed);
  acceleration->setCheck(draft.scroll.acceleration);
  new FXLabel(parent, "Try scrolling here — the document stays unchanged.", nullptr, JUSTIFY_LEFT);
  sample = new EditorWidget(parent);
  sample->setEditable(false);
  sample->setLayoutHints(LAYOUT_FILL_X | LAYOUT_FILL_Y);
  FXString text;
  for (int i = 1; i <= 100; ++i) {
    FXString row;
    row.format("Scroll sample %03d — slow and fast movement\n", i);
    text += row;
  }
  sample->setText(text);
}
void PreferencesDialog::buildDocument(FXComposite* parent) {
  auto* form = new UiForm(parent, *ui);
  new FXLabel(form, "A4 margin (mm)", nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
  margin = new FXRealSpinner(form, 8, this, Changed, REALSPIN_NORMAL | LAYOUT_FILL_X);
  margin->setRange(5, 50);
  margin->setIncrement(1);
  margin->setValue(draft.marginMm);
  new FXLabel(parent, "Paper margins apply to A4 preview and PDF export.", nullptr, JUSTIFY_LEFT);
}
void PreferencesDialog::buildPrograms(FXComposite* parent) {
  auto* form = new UiForm(parent, *ui);
  new FXLabel(form, "Browser program", nullptr, JUSTIFY_LEFT | LAYOUT_CENTER_Y);
  browser = new FXComboBox(form, 24, this, BrowserChanged, COMBOBOX_NORMAL | LAYOUT_FILL_X);
  for (auto name : {"xdg-open", "google-chrome-stable", "firefox"})
    browser->appendItem(name);
  browser->setNumVisible(3);
  browser->setText(draft.browserProgram.c_str());
  browser->setTipText("Executable name or full path; the link is passed automatically.");
  UiFactory(*ui).button(parent, "Browse…", this, BrowseBrowser, UiIcon::Open, ButtonRole::Normal);
  new FXLabel(parent, "xdg-open uses the system default browser.", nullptr, JUSTIFY_LEFT);
}
} // namespace xfmd
