#include "FontCatalog.h"
#include "contracts/DocumentTypes.h"
#include <pango/pangofc-font.h>
#include <fontconfig/fontconfig.h>
#include <sys/stat.h>
#include <algorithm>
namespace xfmd {
namespace {
std::string fileIdentity(FcPattern* pattern) {
  FcChar8* file=nullptr;int index=0;
  FcPatternGetString(pattern,FC_FILE,0,&file);FcPatternGetInteger(pattern,FC_INDEX,0,&index);
  if(!file)return "unknown";
  struct stat st{};if(stat(reinterpret_cast<const char*>(file),&st)!=0)throw Error(ErrorCode::Layout,"Font file is unavailable.");
  return std::string(reinterpret_cast<const char*>(file))+":"+std::to_string(index)+":"+
    std::to_string(st.st_size)+":"+std::to_string(st.st_mtim.tv_sec)+":"+std::to_string(st.st_mtim.tv_nsec);
}
}
std::string FontCatalog::key(PangoFont* font) {
  auto* desc=pango_font_describe_with_absolute_size(font);
  char* name=pango_font_description_to_string(desc);
  std::string result(name);g_free(name);pango_font_description_free(desc);
  if(PANGO_IS_FC_FONT(font))result+='\n'+fileIdentity(pango_fc_font_get_pattern(PANGO_FC_FONT(font)));
  return result;
}
FontCatalog::FontCatalog() {
  map=pango_cairo_font_map_new();pango_cairo_font_map_set_resolution(PANGO_CAIRO_FONT_MAP(map),72);
  contextValue=pango_font_map_create_context(map);
  auto* opts=cairo_font_options_create();cairo_font_options_set_hint_metrics(opts,CAIRO_HINT_METRICS_OFF);
  pango_cairo_context_set_font_options(contextValue,opts);cairo_font_options_destroy(opts);
  try {
  std::vector<std::string> keys;
  auto* set=FcConfigGetFonts(nullptr,FcSetSystem);
  if(set)for(int i=0;i<set->nfont;++i)keys.push_back(fileIdentity(set->fonts[i]));
  std::sort(keys.begin(),keys.end());identity=1469598103934665603ULL;
  for(const auto& k:keys)for(unsigned char c:k){identity^=c;identity*=1099511628211ULL;}
  } catch(...) {g_object_unref(contextValue);g_object_unref(map);throw;}
}
FontCatalog::~FontCatalog(){for(auto& entry:fonts)g_object_unref(entry.second);g_object_unref(contextValue);g_object_unref(map);}
std::string FontCatalog::remember(PangoFont* value) {
  auto name=key(value);
  if(!fonts.count(name))fonts.emplace(name,PANGO_FONT(g_object_ref(value)));
  return name;
}
PangoFont* FontCatalog::font(const std::string& name) {
  auto it=fonts.find(name);if(it!=fonts.end())return it->second;
  auto* desc=pango_font_description_from_string(name.substr(0,name.find('\n')).c_str());
  std::unique_ptr<PangoFont,decltype(&g_object_unref)> owner(pango_font_map_load_font(map,contextValue,desc),g_object_unref);
  auto* value=owner.get();pango_font_description_free(desc);
  if(!value)throw Error(ErrorCode::Layout,"Unable to load document font.");
  if(key(value)!=name){throw Error(ErrorCode::Layout,"Document fonts changed. Refresh the preview before exporting.");}
  fonts.emplace(name,value);owner.release();return value;
}
} // namespace xfmd
