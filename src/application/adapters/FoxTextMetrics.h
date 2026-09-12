#pragma once
#include "contracts/ITextMetrics.h"
#include <fx.h>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace xfmd {
class FoxTextMetrics final : public ITextMetrics {
  FX::FXApp& app;
  std::map<FontSpec, std::unordered_map<std::string, TextExtent>> measurements;
  std::size_t cachedCount = 0;
  std::map<std::pair<FontSpec, std::string>, std::unique_ptr<FX::FXFont>> fonts;

public:
  explicit FoxTextMetrics(FX::FXApp& app) : app(app) {}
  FX::FXFont* font(FontSpec, std::string_view text = {});
  std::vector<std::pair<std::string_view, FX::FXFont*>> segments(std::string_view, FontSpec);
  TextExtent measure(std::string_view, FontSpec) override;
};
} // namespace xfmd
