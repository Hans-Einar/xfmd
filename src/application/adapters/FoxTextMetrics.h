#pragma once
#include <fx.h>
#include "contracts/ITextMetrics.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
namespace xfmd {
class FoxTextMetrics final : public ITextMetrics {
  FX::FXApp& app;
  std::map<std::pair<FontSpec, std::string>, std::unique_ptr<FX::FXFont>> fonts;
public:
  explicit FoxTextMetrics(FX::FXApp& app) : app(app) {}
  FX::FXFont* font(FontSpec, std::string_view text = {});
  std::vector<std::pair<std::string_view, FX::FXFont*>> segments(std::string_view, FontSpec);
  TextExtent measure(std::string_view, FontSpec) override;
};
}
