#pragma once
#include <X11/Xlib.h>
#include <fx.h>
#include <stdexcept>
inline void drainEvents(FX::FXApp& app) {
  auto* display = static_cast<Display*>(app.getDisplay());
  for (int round = 0; round < 10; ++round) {
    app.runWhileEvents();
    XSync(display, False);
    int remaining = 10000;
    // A deferred expose can end runWhileEvents while later input is still queued.
    while (XPending(display) && --remaining)
      app.runOneEvent(false);
    if (!remaining)
      throw std::runtime_error("Native event queue did not settle");
  }
  app.forceRefresh();
  app.repaint();
  app.flush(true);
}
