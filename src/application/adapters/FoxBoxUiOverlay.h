#pragma once
#include "FoxBoxUiInput.h"
#include "FoxRenderHost.h"
#include "application/boxui/BoxUiSession.h"
#include <map>
namespace xfmd {
class FoxBoxUiOverlay : public FX::FXObject {
  FXDECLARE(FoxBoxUiOverlay)
  struct Item {
    FX::FXComposite* clip = nullptr;
    FoxBoxUiField* field = nullptr;
    FoxBoxUiButton* button = nullptr;
    std::unique_ptr<FX::FXFont> font;
    std::shared_ptr<const BoxUiFrame> frame;
    BoxUiControl control;
    Rect bounds;
    std::string compatibility, accepted, expectedRevision;
    bool dirty = false;
    int fontSize = 0;
    ~Item() { delete clip; }
  };
  FoxRenderHost* host = nullptr;
  BoxUiSession* session = nullptr;
  std::map<std::string, std::unique_ptr<Item>> items;
  LayoutResult published;
  std::uint64_t events = 0, publication = 0;
  void submit(Item&);
  void restore(Item&);

protected:
  FoxBoxUiOverlay() = default;

public:
  std::function<void(const BoxUiCommandResult&)> completed;
  FoxBoxUiOverlay(FoxRenderHost&, BoxUiSession&);
  ~FoxBoxUiOverlay() override;
  void reconcile();
  void invalidate();
  void position();
  long onChanged(FX::FXObject*, FX::FXSelector, void*);
  long onCommand(FX::FXObject*, FX::FXSelector, void*);
};
} // namespace xfmd
