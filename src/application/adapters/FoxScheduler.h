#pragma once
#include <fx.h>
#include "IScheduler.h"
#include <map>
namespace xfmd {
class FoxScheduler final : public FX::FXObject, public IScheduler {
  FXDECLARE(FoxScheduler)
  FX::FXApp* app = nullptr;
  std::map<unsigned, std::function<void()>> callbacks;
protected:
  FoxScheduler() = default;
public:
  explicit FoxScheduler(FX::FXApp& app) : app(&app) {}
  ~FoxScheduler() override;
  void restart(unsigned key, unsigned milliseconds, std::function<void()>) override;
  void cancel(unsigned key) override;
  void cancelAll();
  long onTimeout(FX::FXObject*, FX::FXSelector, void*);
};
}
