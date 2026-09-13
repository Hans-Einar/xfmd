#include "SidebarWidget.h"
#include "application/io/InputPolicy.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(SidebarWidget)
sidebarMap[] = {FXMAPFUNC(SEL_DOUBLECLICKED, SidebarWidget::ID_TREE_EVENT, SidebarWidget::onOpen)};
FXIMPLEMENT(SidebarWidget, FXDirList, sidebarMap, ARRAYNUMBER(sidebarMap))
SidebarWidget::SidebarWidget(FXComposite* parent)
    : FXDirList(parent, this, ID_TREE_EVENT,
                DIRLIST_SHOWFILES | TREELIST_SHOWS_LINES | TREELIST_SHOWS_BOXES | LAYOUT_FILL_Y, 0,
                0, 220, 0) {
  setPattern("*.md,*.txt");
  setMatchMode(FILEMATCH_FILE_NAME | FILEMATCH_NOESCAPE | FILEMATCH_CASEFOLD);
}
long SidebarWidget::onOpen(FXObject*, FXSelector, void* data) {
  auto* item = static_cast<FXTreeItem*>(data);
  if (!item)
    item = getCurrentItem();
  if (!item || !isItemFile(item))
    return 1;
  std::string path = getItemPathname(item).text();
  if (InputPolicy::supportedPath(path) && open)
    open(path);
  return 1;
}
} // namespace xfmd
