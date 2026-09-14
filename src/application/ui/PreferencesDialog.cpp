#include "PreferencesDialog.h"
#include "application/adapters/FoxWheelScrollBar.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(PreferencesDialog)
preferencesMap[] = {
    FXMAPFUNC(SEL_COMMAND, FXDialogBox::ID_ACCEPT, PreferencesDialog::onAccept),
    FXMAPFUNC(SEL_COMMAND, FXDialogBox::ID_CANCEL, PreferencesDialog::onCancel),
    FXMAPFUNC(SEL_CLOSE, 0, PreferencesDialog::onCancel),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::BrowseBrowser, PreferencesDialog::onBrowseBrowser),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::ReloadStyle, PreferencesDialog::onReload),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::SampleToggle, PreferencesDialog::onSampleToggle),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::Changed, PreferencesDialog::onChanged),
    FXMAPFUNC(SEL_CHANGED, PreferencesDialog::Changed, PreferencesDialog::onChanged),
    FXMAPFUNC(SEL_COMMAND, PreferencesDialog::BrowserChanged, PreferencesDialog::onChanged),
    FXMAPFUNC(SEL_CHANGED, PreferencesDialog::BrowserChanged, PreferencesDialog::onChanged),
    FXMAPFUNCS(SEL_COMMAND, PreferencesDialog::ThemeChanged, PreferencesDialog::FontChanged,
               PreferencesDialog::onAppearance),
    FXMAPFUNCS(SEL_CHANGED, PreferencesDialog::ThemeChanged, PreferencesDialog::FontChanged,
               PreferencesDialog::onAppearance)};
FXIMPLEMENT(PreferencesDialog, FXDialogBox, preferencesMap, ARRAYNUMBER(preferencesMap))
PreferencesDialog::PreferencesDialog(FXWindow* owner, PreferencesService& preferences,
                                     UiContext& context,
                                     std::function<void(const Appearance&)> callback)
    : FXDialogBox(owner, "Preferences", DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE, 0, 0, 640, 490),
      service(&preferences), ui(&context), draft(preferences.begin()), preview(std::move(callback)),
      originalProfiles(context.profileSnapshot()), draftProfiles(originalProfiles) {
  auto* content = new FXVerticalFrame(this, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* tabs = new FXTabBook(content, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto page = [&](const char* title) {
    new FXTabItem(tabs, title);
    return new FXVerticalFrame(tabs, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 12, 12, 12, 12);
  };
  buildAppearance(page("Appearance"));
  buildScrolling(page("Scrolling"));
  buildDocument(page("Document"));
  buildPrograms(page("Programs"));
  error = new FXLabel(content, "", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
  new DialogActions(content, context, this, ID_ACCEPT, ID_CANCEL);
  context.apply(this);
  error->setTextColor(context.palette().danger);
  error->setText(context.error().c_str());
  onChanged(nullptr, 0, nullptr);
}
PreferencesDialog::~PreferencesDialog() {
  if (!committed)
    restore();
}
void PreferencesDialog::apply(const Appearance& value) {
  if (preview)
    preview(value);
  else {
    ui->setAppearance(value);
    ui->apply(getApp()->getRootWindow());
  }
  ui->apply(this);
  if (error)
    error->setTextColor(ui->palette().danger);
  resize(std::max(640, value.fontSize * 48), std::max(490, value.fontSize * 37));
  getApp()->refresh();
}
void PreferencesDialog::restore() {
  if (service && ui) {
    ui->restoreProfiles(originalProfiles);
    apply(service->active().appearance);
  }
}
long PreferencesDialog::onAppearance(FXObject*, FXSelector, void*) {
  draft.appearance = {theme->getCurrentItem() == 1 ? "dark" : "light",
                      density->getCurrentItem() == 1,
                      buttons->getCurrentItem() == 1 ? "classic" : "flat", fontSize->getValue()};
  ui->restoreProfiles(draftProfiles);
  apply(draft.appearance);
  return 1;
}
long PreferencesDialog::onSampleToggle(FXObject* sender, FXSelector, void*) {
  auto* button = dynamic_cast<UiButton*>(sender);
  if (button)
    button->handle(
        this, FXSEL(SEL_COMMAND, button->isChecked() ? FXWindow::ID_UNCHECK : FXWindow::ID_CHECK),
        nullptr);
  return 1;
}
long PreferencesDialog::onReload(FXObject*, FXSelector, void*) {
  std::string message;
  if (ui->reload(message)) {
    draftProfiles = ui->profileSnapshot();
    apply(draft.appearance);
  }
  error->setText(message.c_str());
  return 1;
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
  ui->restoreProfiles(draftProfiles);
  if (!service->commit(draft, message)) {
    restore();
    error->setText(message.c_str());
    return 1;
  }
  committed = true;
  apply(service->active().appearance);
  return FXDialogBox::onCmdAccept(this, FXSEL(SEL_COMMAND, ID_ACCEPT), nullptr);
}
long PreferencesDialog::onCancel(FXObject*, FXSelector, void*) {
  restore();
  return FXDialogBox::onCmdCancel(this, FXSEL(SEL_COMMAND, ID_CANCEL), nullptr);
}
} // namespace xfmd
