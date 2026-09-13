#include <pango/pangocairo.h>
#include <cairo-pdf.h>
#include <chrono>
#include <iostream>
int main() {
  auto* map=pango_cairo_font_map_new();
  pango_cairo_font_map_set_resolution(PANGO_CAIRO_FONT_MAP(map),72);
  auto* context=pango_font_map_create_context(map);
  auto* opts=cairo_font_options_create();
  cairo_font_options_set_hint_metrics(opts,CAIRO_HINT_METRICS_OFF);
  pango_cairo_context_set_font_options(context,opts);
  auto* layout=pango_layout_new(context);
  auto* font=pango_font_description_from_string("DejaVu Sans 12");
  pango_layout_set_font_description(layout,font);
  const char* text="XFMD æøå — office é 日本語";
  pango_layout_set_text(layout,text,-1);
  int w,h;pango_layout_get_size(layout,&w,&h);
  auto* pdf=cairo_pdf_surface_create("/tmp/xfmd-typography-probe.pdf",210*72/25.4,297*72/25.4);
  auto* image=cairo_image_surface_create(CAIRO_FORMAT_ARGB32,600,850);
  for(auto* surface:{pdf,image}) {
    auto* cr=cairo_create(surface);
    cairo_set_source_rgb(cr,1,1,1);cairo_paint(cr);
    cairo_set_source_rgb(cr,0,0,0);cairo_move_to(cr,56.7,56.7);
    pango_cairo_show_layout(cr,layout);
    if(cairo_status(cr)!=CAIRO_STATUS_SUCCESS)return 1;
    cairo_destroy(cr);
  }
  cairo_surface_write_to_png(image,"/tmp/xfmd-typography-probe.png");
  cairo_surface_finish(pdf);
  if(cairo_surface_status(pdf)!=CAIRO_STATUS_SUCCESS)return 2;
  int w2,h2;pango_layout_get_size(layout,&w2,&h2);
  if(w!=w2||h!=h2)return 3;
  const auto start=std::chrono::steady_clock::now();
  for(int i=0;i<10000;++i) {pango_layout_set_text(layout,text,-1);pango_layout_get_size(layout,&w2,&h2);}
  std::cout<<"Shared layout: "<<w/double(PANGO_SCALE)<<" x "<<h/double(PANGO_SCALE)
           <<" pt; 10000 shape iterations: "<<std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count()<<" s\n";
  cairo_surface_destroy(pdf);cairo_surface_destroy(image);g_object_unref(layout);
  pango_font_description_free(font);cairo_font_options_destroy(opts);g_object_unref(context);g_object_unref(map);
}
