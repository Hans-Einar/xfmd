#include "DocumentFixture.h"
#include "LabWindow.h"
#include "UiButton.h"
#include <cairo-xlib.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

class SpacedItem : public FXTreeItem {
public:
  explicit SpacedItem(const char* text) : FXTreeItem(text) {}
  FXint getHeight(const FXTreeList* list) const override { return FXTreeItem::getHeight(list) + 8; }
};
FXTreeItem* node(FXTreeList* tree, FXTreeItem* parent, const char* text) {
  auto* item = tree->appendItem(parent, new SpacedItem(text));
  tree->expandTree(item);
  return item;
}
FXHorizontalFrame* row(FXComposite* parent, const Style& s, FXuint extra = 0) {
  return new FXHorizontalFrame(parent, LAYOUT_FILL_X | PACK_UNIFORM_HEIGHT | extra, 0, 0, 0, 0,
                               s.inset, s.inset, 4, 4, s.gap, s.gap);
}
void divider(FXComposite* parent, const Style& s) {
  auto* sep = new FXVerticalSeparator(parent, SEPARATOR_GROOVE | LAYOUT_FILL_Y, 0, 0, 1, 0, s.gap,
                                      s.gap, 5, 5);
  sep->setShadowColor(s.border);
}
UiButton* button(FXComposite* p, const Style& s, const char* text, Glyph g = Glyph::NoIcon,
                 bool active = false) {
  return new UiButton(p, s, text, g, active);
}
void header(FXComposite* p, const char* label, const Style& s) {
  new FXLabel(p, label, nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X, 0, 0, 0, 0, s.inset, s.inset, 10, 8);
}
FXTreeList* tree(FXComposite* p) {
  return new FXTreeList(p, nullptr, 0,
                        TREELIST_SHOWS_BOXES | TREELIST_ROOT_BOXES | TREELIST_BROWSESELECT |
                            LAYOUT_FILL_X | LAYOUT_FILL_Y);
}
void filePage(FXComposite* p, const Style& s) {
  auto* bar = row(p, s);
  new FXLabel(bar, "xfmd / docs", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
  auto* refresh = button(bar, s, "", Glyph::Refresh);
  refresh->setTipText("Refresh files");
  auto* search = row(p, s);
  button(search, s, "", Glyph::Search);
  auto* field =
      new FXTextField(search, 16, nullptr, 0, TEXTFIELD_NORMAL | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
  field->setText("Filter files...");
  auto* filters = row(p, s);
  button(filters, s, ".md", Glyph::NoIcon, true);
  button(filters, s, ".txt");
  auto* split =
      new FXSplitter(p, SPLITTER_VERTICAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* upper = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 370, 8, 8, 0, 0);
  auto* t = tree(upper);
  auto* root = node(t, nullptr, "docs");
  node(t, root, "README.md");
  auto* active = node(t, root, "workspace.md");
  node(t, root, "keyboard.md");
  node(t, root, "architecture.md");
  auto* design = node(t, root, "design");
  node(t, design, "ui-proposal.md");
  node(t, design, "ux-proposal.md");
  t->setCurrentItem(active);
  t->selectItem(active);
  auto* lower = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 145, 8, 8, 0, 0);
  header(lower, "Recent folders", s);
  auto* recent = tree(lower);
  node(recent, nullptr, "~/git/xfmd/docs");
  node(recent, nullptr, "~/git/Linux");
}
void indexPage(FXComposite* p, const Style& s) {
  auto* split =
      new FXSplitter(p, SPLITTER_VERTICAL | SPLITTER_TRACKING | LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* upper = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 330, 8, 8, 0, 0);
  header(upper, "Document index", s);
  auto* outline = tree(upper);
  auto* root = node(outline, nullptr, "A quieter workspace");
  auto* active = node(outline, root, "1. Keep the document in focus");
  node(outline, root, "2. Make actions easy to find");
  node(outline, root, "3. Change the look");
  outline->setCurrentItem(active);
  outline->selectItem(active);
  auto* lower = new FXVerticalFrame(split, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 280, 8, 8, 0, 0);
  header(lower, "References", s);
  auto* refs = tree(lower);
  auto* md = node(refs, nullptr, "Markdown");
  auto* file = node(refs, md, "ui-proposal.md");
  node(refs, file, "1. A shared appearance");
  node(refs, file, "2. Buttons and spacing");
  auto* web = node(refs, nullptr, "Hyperlinks");
  node(refs, web, "FOX Toolkit");
}
void workspace(FXMainWindow* window, const Style& s, bool reading, std::vector<FXMenuPane*>& menus,
               std::vector<FXFont*>& fonts) {
  auto* menu =
      new FXMenuBar(window, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, s.inset, s.inset, 2, 2);
  for (auto label : {"File", "Edit", "View", "Go"}) {
    auto* pane = new FXMenuPane(window);
    menus.push_back(pane);
    new FXMenuCommand(pane, "Prototype only — no file operations");
    new FXMenuTitle(menu, label, nullptr, pane);
  }
  auto* tools = row(window, s, LAYOUT_SIDE_TOP);
  auto* open = button(tools, s, "", Glyph::Open);
  open->setTipText("Open file (Ctrl+O)");
  auto* save = button(tools, s, "", Glyph::Save);
  save->setTipText("Save (Ctrl+S)");
  divider(tools, s);
  button(tools, s, "", Glyph::Back)->setTipText("Back (Alt+Left)");
  auto* forward = button(tools, s, "", Glyph::Forward);
  forward->disable();
  divider(tools, s);
  button(tools, s, "", Glyph::Sidebar, true)->setTipText("Sidebar (F10)");
  new FXLabel(tools, "workspace.md", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y, 0, 0,
              0, 0, 12, 4, 0, 0);
  button(tools, s, "Preview", Glyph::NoIcon, reading);
  button(tools, s, "Split", Glyph::NoIcon, !reading);
  button(tools, s, "Editor");
  auto* status = row(window, s, LAYOUT_SIDE_BOTTOM);
  new FXLabel(status, "Saved   ·   UTF-8", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X);
  new FXLabel(status, "Markdown   ·   42 lines", nullptr, JUSTIFY_RIGHT);
  auto* outer = new FXSplitter(window, SPLITTER_HORIZONTAL | SPLITTER_TRACKING | LAYOUT_FILL_X |
                                           LAYOUT_FILL_Y);
  auto* side = new FXVerticalFrame(outer, LAYOUT_FILL_Y, 0, 0, reading ? 295 : 250, 0, 0, 0, 0, 0);
  auto* tabs = new FXTabBook(side, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
  auto* files = new FXTabItem(tabs, "Files");
  files->setPadLeft(14);
  files->setPadRight(14);
  files->setPadTop(7);
  files->setPadBottom(7);
  auto* fp = new FXVerticalFrame(tabs, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  filePage(fp, s);
  auto* index = new FXTabItem(tabs, "Index");
  index->setPadLeft(14);
  index->setPadRight(14);
  index->setPadTop(7);
  index->setPadBottom(7);
  auto* ip = new FXVerticalFrame(tabs, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  indexPage(ip, s);
  tabs->setCurrent(reading ? 1 : 0);
  auto* content = new FXSplitter(outer, SPLITTER_HORIZONTAL | SPLITTER_TRACKING | LAYOUT_FILL_X |
                                            LAYOUT_FILL_Y);
  if (!reading) {
    auto* editorArea = new FXVerticalFrame(content, LAYOUT_FILL_Y, 0, 0, 340, 0, 0, 0, 0, 0);
    header(editorArea, "MARKDOWN", s);
    auto* edit = new FXText(editorArea, nullptr, 0, TEXT_WORDWRAP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    auto* font = new FXFont(window->getApp(), "DejaVu Sans Mono", 9);
    fonts.push_back(font);
    edit->setFont(font);
    edit->setMarginLeft(18);
    edit->setMarginTop(18);
    edit->setText(
        "# A quieter workspace\n\nSmall changes. Familiar tools.\n\n## 1. Keep the document in "
        "focus\n\nGroup related actions and give the page room.\nThe editor, index and preview "
        "stay connected.\n\n## 2. Make actions easy to find\n\n| Action | Shortcut |\n| --- | --- "
        "|\n| Open file | Ctrl+O |\n| Save document | Ctrl+S |\n| Find text | Ctrl+F |\n| Toggle "
        "sidebar | F10 |\n\n## 3. Change the look, keep the workflow\n\nOne profile defines "
        "colors, spacing and icons.\n\n[Read the UI "
        "proposal](ui-proposal.md)\n\n```ini\nappearance = light\n```\n");
  }
  auto* preview =
      new FXVerticalFrame(content, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
  auto* previewHeader = row(preview, s);
  new FXLabel(previewHeader, "PREVIEW", nullptr, JUSTIFY_LEFT | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
  button(previewHeader, s, "Window wrap", Glyph::NoIcon, true);
  button(previewHeader, s, "A4 page");
  new DocumentFixture(preview);
}
void controls(FXMainWindow* window, const Style& s) {
  auto* content = new FXVerticalFrame(window, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 28, 28, 24,
                                      24, 12, 12);
  header(content, "BUTTON STUDY  /  same FOX input, different paint", s);
  auto* samples = row(content, s);
  button(samples, s, "Normal");
  new UiButton(samples, s, "Hover", Glyph::NoIcon, false, 1);
  new UiButton(samples, s, "Pressed", Glyph::NoIcon, false, 2);
  button(samples, s, "Selected", Glyph::NoIcon, true);
  button(samples, s, "Disabled")->disable();
  new UiButton(samples, s, "Focus", Glyph::NoIcon, false, 3);
  header(content, "Toolbar roles", s);
  auto* icons = row(content, s);
  button(icons, s, "", Glyph::Open);
  button(icons, s, "", Glyph::Save);
  divider(icons, s);
  button(icons, s, "", Glyph::Back);
  button(icons, s, "", Glyph::Forward)->disable();
  divider(icons, s);
  button(icons, s, "Open file", Glyph::Open);
  header(content, "Appearance preview — proposed Preferences section", s);
  auto* form =
      new FXMatrix(content, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X, 0, 0, 0, 0, 12, 12, 8, 8, 20, 12);
  new FXLabel(form, "Color profile", nullptr, JUSTIFY_LEFT);
  auto* profile = new FXComboBox(form, 24, nullptr, 0, COMBOBOX_STATIC | LAYOUT_FILL_X);
  profile->appendItem("Light");
  profile->appendItem("Graphite");
  profile->appendItem("Classic");
  profile->setCurrentItem(0);
  new FXLabel(form, "Density", nullptr, JUSTIFY_LEFT);
  auto* density = new FXComboBox(form, 24, nullptr, 0, COMBOBOX_STATIC | LAYOUT_FILL_X);
  density->appendItem("Comfortable");
  density->appendItem("Compact");
  density->setCurrentItem(0);
  header(content, "One profile: color · spacing · radius · button painter", s);
  auto* actions = row(content, s);
  new FXLabel(actions, "", nullptr, LAYOUT_FILL_X);
  button(actions, s, "Cancel");
  button(actions, s, "OK", Glyph::NoIcon, true);
}
int main(int argc, char** argv) {
  try {
    if (argc < 4) {
      std::cerr << "Usage: fox-ui-lab profiles.ini light|graphite|compact "
                   "workspace|reader|controls [output.png]\n";
      return 2;
    }
    std::string profiles = argv[1], name = argv[2], mode = argv[3],
                output = argc > 4 ? argv[4] : "";
    if (name != "light" && name != "graphite" && name != "compact")
      throw std::runtime_error("Unknown profile");
    if (mode != "workspace" && mode != "reader" && mode != "controls")
      throw std::runtime_error("Unknown view");
    argc = 1;
    FXApp app("fox-ui-lab", "xfmd-design");
    app.init(argc, argv);
    Style s = Style::load(profiles.c_str(), name.c_str());
    app.setBaseColor(s.surface);
    app.setForeColor(s.text);
    app.setBackColor(s.field);
    app.setBorderColor(s.border);
    app.setHiliteColor(s.panel);
    app.setShadowColor(s.border);
    app.setSelbackColor(s.selected);
    app.setSelforeColor(s.text);
    new FXToolTip(&app);
    auto* window =
        new LabWindow(&app, "XFMD design study — FOX prototype", nullptr, nullptr, DECOR_ALL, 40,
                      40, mode == "controls" ? 940 : 1200, mode == "controls" ? 520 : 800);
    std::vector<FXMenuPane*> menus;
    std::vector<FXFont*> fonts;
    if (mode == "controls")
      controls(window, s);
    else
      workspace(window, s, mode == "reader", menus, fonts);
    s.apply(window);
    app.create();
    for (auto* font : fonts)
      font->create();
    window->show(PLACEMENT_DEFAULT);
    if (output.empty() || output == "--check-close") {
      if (output == "--check-close")
        app.addTimeout(window, FXTopWindow::ID_CLOSE, 100);
      auto result = app.run();
      for (auto* pane : menus)
        delete pane;
      delete window;
      for (auto* font : fonts)
        delete font;
      return result;
    }
    for (int i = 0; i < 300; ++i) {
      app.runWhileEvents();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    app.repaint();
    app.flush(true);
    auto* display = static_cast<Display*>(app.getDisplay());
    auto* shot = cairo_xlib_surface_create(display, window->id(),
                                           DefaultVisual(display, DefaultScreen(display)),
                                           window->getWidth(), window->getHeight());
    const auto captured = cairo_surface_write_to_png(shot, output.c_str());
    cairo_surface_destroy(shot);
    if (captured != CAIRO_STATUS_SUCCESS)
      throw std::runtime_error("Screenshot failed");
    for (auto* pane : menus)
      delete pane;
    delete window;
    for (auto* font : fonts)
      delete font;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
