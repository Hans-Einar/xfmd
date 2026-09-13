#include "SharedTextMetrics.h"
#include "contracts/DocumentTypes.h"
namespace xfmd {
std::shared_ptr<const ShapedText> SharedTextMetrics::shape(std::string_view text,FontSpec spec) {
  if(text.size()>65536)throw Error(ErrorCode::TooLarge,"A text token is too large to shape safely.");
  std::string key;key.reserve(text.size()+4);key.append(text);
  key.push_back(char(spec.points&255));key.push_back(char((spec.points>>8)&255));
  key.push_back(char(spec.bold | (spec.italic<<1) | (spec.mono<<2)));
  auto found=cache.find(key);if(found!=cache.end())return found->second;
  std::unique_ptr<PangoLayout,decltype(&g_object_unref)> owner(pango_layout_new(catalog.context()),g_object_unref);
  auto* layout=owner.get();
  auto* desc=pango_font_description_new();
  pango_font_description_set_family(desc,spec.mono?"DejaVu Sans Mono":"DejaVu Sans");
  pango_font_description_set_size(desc,spec.points*PANGO_SCALE);
  pango_font_description_set_weight(desc,spec.bold?PANGO_WEIGHT_BOLD:PANGO_WEIGHT_NORMAL);
  pango_font_description_set_style(desc,spec.italic?PANGO_STYLE_ITALIC:PANGO_STYLE_NORMAL);
  pango_layout_set_font_description(layout,desc);pango_font_description_free(desc);
  pango_layout_set_single_paragraph_mode(layout,true);pango_layout_set_text(layout,text.data(),int(text.size()));
  auto result=std::make_shared<ShapedText>();
  int width,height;pango_layout_get_size(layout,&width,&height);
  result->width=double(width)/PANGO_SCALE;result->height=double(height)/PANGO_SCALE;
  result->ascent=double(pango_layout_get_baseline(layout))/PANGO_SCALE;
  auto* line=pango_layout_get_line_readonly(layout,0);
  if(line)for(auto* node=line->runs;node;node=node->next) {
    auto* run=static_cast<PangoGlyphItem*>(node->data);GlyphSegment segment;
    segment.text=std::string(text.substr(run->item->offset,run->item->length));
    segment.fontFace=catalog.remember(run->item->analysis.font);segment.level=run->item->analysis.level;segment.byteOffset=run->item->offset;
    for(int i=0;i<run->glyphs->num_glyphs;++i) {
      auto& g=run->glyphs->glyphs[i];
      segment.glyphs.push_back({g.glyph,std::uint32_t(run->glyphs->log_clusters[i]),double(g.geometry.width)/PANGO_SCALE,
                               double(g.geometry.x_offset)/PANGO_SCALE,double(g.geometry.y_offset)/PANGO_SCALE});
    }
    result->segments.push_back(std::move(segment));
  }

  if(text.size()<=256) {if(cache.size()>=8192)cache.clear();cache.emplace(std::move(key),result);}
  return result;
}
TextExtent SharedTextMetrics::measure(std::string_view text,FontSpec font) {
  auto shaped=shape(text,font);TextExtent result{shaped->width,shaped->height,shaped->ascent};
  result.shaped=std::move(shaped);return result;
}
} // namespace xfmd
