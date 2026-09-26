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
  FX::FXLabel* empty = nullptr;
  void refresh();
  void save();
  int selected() const;
  void report(const std::string&);

protected:
  FavoritesPanel() = default;

public:
  enum { ID_ADD = FX::FXVerticalFrame::ID_LAST, ID_REMOVE, ID_SELECT, ID_ACTIVATE, ID_LAST };
  FX::FXList* list = nullptr;
  UiButton *addButton = nullptr, *removeButton = nullptr;
  std::function<std::string()> currentDirectory;
  std::function<void(const Entry&)> open;
  std::function<void(const std::string&)> feedback;
  FavoritesPanel(FX::FXComposite*, UiContext&);
  ~FavoritesPanel() override;
  bool add(const std::string&);
  const std::vector<Entry>& entries() const { return paths; }
  long onAdd(FX::FXObject*, FX::FXSelector, void*);
  long onRemove(FX::FXObject*, FX::FXSelector, void*);
  long onChanged(FX::FXObject*, FX::FXSelector, void*);
  long onSelect(FX::FXObject*, FX::FXSelector, void*);
  long onActivate(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
