#pragma once
#include "controls/UiLayout.h"
#include <functional>
#include <string>
#include <vector>
namespace xfmd {
class RecentFilesPanel : public FX::FXVerticalFrame {
  FXDECLARE(RecentFilesPanel)
  std::vector<std::string> paths;
  std::string pending;
  void refresh();

protected:
  RecentFilesPanel() = default;

public:
  enum { ID_SELECT = FX::FXVerticalFrame::ID_LAST, ID_ACTIVATE, ID_LAST };
  FX::FXList* list = nullptr;
  std::function<void(const std::string&)> open;
  RecentFilesPanel(FX::FXComposite*, UiContext&);
  ~RecentFilesPanel() override;
  void remember(const std::string&);
  const std::vector<std::string>& entries() const { return paths; }
  long onSelect(FX::FXObject*, FX::FXSelector, void*);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
