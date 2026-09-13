#pragma once
#include <fx.h>
#include <optional>
#include <vector>
namespace xfmd {
struct WindowRect {
  int x = 0, y = 0, width = 0, height = 0;
};
class FoxWindowMode : public FX::FXObject {
  FXDECLARE(FoxWindowMode)
  FX::FXTopWindow* window = nullptr;
  bool observed = false;
  std::optional<bool> pending;
  WindowRect before;

protected:
  FoxWindowMode() = default;

public:
  enum { ID_RESTORE = 1 };
  explicit FoxWindowMode(FX::FXTopWindow& value) : window(&value) {}
  ~FoxWindowMode() override;
  bool fullscreen() const;
  bool requested() const { return pending.value_or(fullscreen()); }
  bool requestFullscreen(bool);
  void observe();
  long settle(FX::FXObject*, FX::FXSelector, void*);
  static WindowRect ensureVisible(WindowRect, const std::vector<WindowRect>&);
};
} // namespace xfmd
