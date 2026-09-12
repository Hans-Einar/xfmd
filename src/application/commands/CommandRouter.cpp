#include "CommandRouter.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(CommandRouter) commandMap[] = {
  FXMAPFUNCS(SEL_COMMAND, CommandRouter::Open, CommandRouter::Last - 1, CommandRouter::dispatch),
  FXMAPFUNCS(SEL_UPDATE, CommandRouter::Open, CommandRouter::Last - 1, CommandRouter::update)};
FXIMPLEMENT(CommandRouter, FXObject, commandMap, ARRAYNUMBER(commandMap))
long CommandRouter::dispatch(FXObject*, FXSelector selector, void*) {
  auto command = static_cast<Command>(FXSELID(selector));
  if (action && (!enabled || enabled(command))) action(command);
  return 1;
}
long CommandRouter::update(FXObject* sender, FXSelector selector, void*) {
  bool state = !enabled || enabled(static_cast<Command>(FXSELID(selector)));
  sender->handle(this, FXSEL(SEL_COMMAND, state ? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE), nullptr);
  return 1;
}
}
