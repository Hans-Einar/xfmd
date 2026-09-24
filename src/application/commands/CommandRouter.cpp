#include "CommandRouter.h"
using namespace FX;
namespace xfmd {
FXDEFMAP(CommandRouter)
commandMap[] = {
    FXMAPFUNCS(SEL_COMMAND, CommandRouter::Open, CommandRouter::Last - 1, CommandRouter::dispatch),
    FXMAPFUNCS(SEL_UPDATE, CommandRouter::Open, CommandRouter::Last - 1, CommandRouter::update)};
FXIMPLEMENT(CommandRouter, FXObject, commandMap, ARRAYNUMBER(commandMap))
double CommandRouter::presetPercent(Command command) {
  switch (command) {
  case Zoom25:
    return 25;
  case Zoom50:
    return 50;
  case ActualSize:
    return 100;
  case Zoom200:
    return 200;
  case Zoom300:
    return 300;
  default:
    return 0;
  }
}
long CommandRouter::dispatch(FXObject*, FXSelector selector, void*) {
  auto command = static_cast<Command>(FXSELID(selector));
  if (action && (!enabled || enabled(command)))
    action(command);
  return 1;
}
long CommandRouter::update(FXObject* sender, FXSelector selector, void*) {
  if (checked)
    sender->handle(this,
                   FXSEL(SEL_COMMAND, checked(static_cast<Command>(FXSELID(selector)))
                                          ? FXWindow::ID_CHECK
                                          : FXWindow::ID_UNCHECK),
                   nullptr);
  bool state = !enabled || enabled(static_cast<Command>(FXSELID(selector)));
  sender->handle(this, FXSEL(SEL_COMMAND, state ? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE),
                 nullptr);
  return 1;
}
} // namespace xfmd
