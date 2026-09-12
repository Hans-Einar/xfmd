#include "FoxScheduler.h"
#include <stdexcept>
using namespace FX;
namespace xfmd {
FXDEFMAP(FoxScheduler) schedulerMap[] = {FXMAPFUNCS(SEL_TIMEOUT, 1, 65535, FoxScheduler::onTimeout)};
FXIMPLEMENT(FoxScheduler, FXObject, schedulerMap, ARRAYNUMBER(schedulerMap))
FoxScheduler::~FoxScheduler() { cancelAll(); }
void FoxScheduler::restart(unsigned key, unsigned milliseconds, std::function<void()> callback) {
  if (!key || key > 65535) throw std::invalid_argument("Timer key out of range");
  cancel(key); callbacks[key] = std::move(callback); app->addTimeout(this, key, milliseconds);
}
void FoxScheduler::cancel(unsigned key) { if (app) app->removeTimeout(this, key); callbacks.erase(key); }
void FoxScheduler::cancelAll() { while (!callbacks.empty()) cancel(callbacks.begin()->first); }
long FoxScheduler::onTimeout(FXObject*, FXSelector selector, void*) {
  auto it = callbacks.find(FXSELID(selector));
  if (it == callbacks.end()) return 1;
  auto callback = std::move(it->second); callbacks.erase(it);
  callback(); return 1;
}
}
