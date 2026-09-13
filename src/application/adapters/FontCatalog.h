#pragma once
#include "contracts/ITextMetrics.h"
#include <pango/pangocairo.h>
#include <map>
namespace xfmd {
// Native font resources are thread-owned. Frames carry only identity strings and glyph values.
class FontCatalog {
  PangoFontMap* map=nullptr;
  PangoContext* contextValue=nullptr;
  std::map<std::string,PangoFont*> fonts;
  FontSetId identity=0;
  static std::string key(PangoFont*);
public:
  FontCatalog();
  ~FontCatalog();
  FontCatalog(const FontCatalog&)=delete;
  FontCatalog& operator=(const FontCatalog&)=delete;
  PangoContext* context() const {return contextValue;}
  FontSetId id() const {return identity;}
  std::string remember(PangoFont*);
  PangoFont* font(const std::string&);
};
} // namespace xfmd
