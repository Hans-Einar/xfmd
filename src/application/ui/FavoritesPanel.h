#pragma once
#include "controls/UiLayout.h"
#include <functional>
#include <string>
#include <vector>
namespace xfmd {
class FavoritesPanel : public FX::FXVerticalFrame {
  FXDECLARE(FavoritesPanel)
public:
  struct Entry {
    std::string path;
    bool folder = false;
  };

private:
  std::vector<Entry> paths;
  std::vector<int> rows;
  Entry pending;
  void refresh();
  void save();
  int selected() const;
  void report(const std::string&);

protected:
  FavoritesPanel() = default;

public:
  enum { ID_SELECT = FX::FXVerticalFrame::ID_LAST, ID_ACTIVATE, ID_LAST };
  FX::FXList* list = nullptr;
  std::function<void(const Entry&)> open;
  std::function<void(const std::string&)> feedback;
  FavoritesPanel(FX::FXComposite*, UiContext&);
  ~FavoritesPanel() override;
  bool add(const std::string&);
  void remove(const std::string&);
  const std::vector<Entry>& entries() const { return paths; }
  long onSelect(FX::FXObject*, FX::FXSelector, void*);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
