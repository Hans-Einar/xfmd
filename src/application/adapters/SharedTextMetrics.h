#pragma once
#include "FontCatalog.h"
#include <unordered_map>
namespace xfmd {
class SharedTextMetrics : public ITextMetrics {
  std::unordered_map<std::string,std::shared_ptr<const ShapedText>> cache;
public:
  FontCatalog catalog;
  TextExtent measure(std::string_view,FontSpec) override;
  std::shared_ptr<const ShapedText> shape(std::string_view,FontSpec) override;
  FontSetId fontSetId() const override {return catalog.id();}
};
} // namespace xfmd
