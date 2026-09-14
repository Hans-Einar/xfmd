#include "DocumentFixture.h"
#include <algorithm>
FXDEFMAP(DocumentFixture) documentMap[] = {FXMAPFUNC(SEL_PAINT, 0, DocumentFixture::onPaint)};
FXIMPLEMENT(DocumentFixture, FXCanvas, documentMap, ARRAYNUMBER(documentMap))
DocumentFixture::DocumentFixture(FXComposite* parent)
    : FXCanvas(parent, nullptr, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y) {
  title = new FXFont(getApp(), "DejaVu Sans", 23, FXFont::Bold);
  heading = new FXFont(getApp(), "DejaVu Sans", 13, FXFont::Bold);
  body = new FXFont(getApp(), "DejaVu Sans", 11);
  mono = new FXFont(getApp(), "DejaVu Sans Mono", 10);
}
DocumentFixture::~DocumentFixture() {
  delete title;
  delete heading;
  delete body;
  delete mono;
}
void DocumentFixture::create() {
  FXCanvas::create();
  title->create();
  heading->create();
  body->create();
  mono->create();
}
long DocumentFixture::onPaint(FXObject*, FXSelector, void* ptr) {
  FXDCWindow dc(this, static_cast<FXEvent*>(ptr));
  dc.setForeground(FXRGB(255, 255, 255));
  dc.fillRectangle(0, 0, getWidth(), getHeight());
  int cw = std::min(690, getWidth() - 64), x = (getWidth() - cw) / 2, y = 46;
  auto line = [&](const char* s, FXFont* font, int after) {
    dc.setFont(font);
    dc.setForeground(FXRGB(38, 48, 62));
    dc.drawText(x, y, s);
    y += after;
  };
  line("A quieter workspace", title, 36);
  line("Small changes. Familiar tools.", body, 42);
  line("1. Keep the document in focus", heading, 29);
  line("Group related actions and give the page room.", body, 25);
  line("The editor, index and preview stay connected.", body, 43);
  line("2. Make actions easy to find", heading, 27);
  const char* cells[][2] = {{"Action", "Shortcut"},
                            {"Open file", "Ctrl+O"},
                            {"Save document", "Ctrl+S"},
                            {"Find text", "Ctrl+F"},
                            {"Toggle sidebar", "F10"}};
  int row = 34, split = cw * 3 / 5;
  for (int i = 0; i < 5; ++i) {
    dc.setForeground(i == 0 ? FXRGB(237, 242, 248) : FXRGB(255, 255, 255));
    dc.fillRectangle(x, y, cw, row);
    dc.setForeground(FXRGB(207, 216, 226));
    dc.drawRectangle(x, y, cw, row);
    dc.drawLine(x + split, y, x + split, y + row);
    dc.setFont(i == 0 ? heading : body);
    dc.setForeground(FXRGB(38, 48, 62));
    dc.drawText(x + 12, y + 23, cells[i][0]);
    dc.drawText(x + split + 12, y + 23, cells[i][1]);
    y += row;
  }
  y += 40;
  line("3. Change the look, keep the workflow", heading, 27);
  line("One profile defines colors, spacing and icons.", body, 25);
  dc.setForeground(FXRGB(36, 92, 190));
  dc.setFont(body);
  dc.drawText(x, y, "Read the UI proposal");
  y += 32;
  dc.setForeground(FXRGB(244, 246, 249));
  dc.fillRectangle(x, y, cw, 48);
  dc.setForeground(FXRGB(38, 48, 62));
  dc.setFont(mono);
  dc.drawText(x + 12, y + 29, "appearance = light");
  return 1;
}
