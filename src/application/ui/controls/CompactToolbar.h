#pragma once
#include <fx.h>
namespace xfmd {
// Packs intact control groups into as few rows as the available width permits.
class CompactToolbar : public FX::FXHorizontalFrame {
  FX::FXint rowsHeight(FX::FXint width) const;

public:
  explicit CompactToolbar(FX::FXComposite*);
  FX::FXint getDefaultHeight() override;
  void layout() override;
};
} // namespace xfmd
