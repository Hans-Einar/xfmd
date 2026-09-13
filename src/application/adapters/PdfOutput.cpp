#include "PdfOutput.h"
#include <cairo-pdf.h>
#include <cerrno>
#include <memory>
#include <unistd.h>
namespace xfmd {
namespace {
struct Stream {
  int fd;
  ExportControl& control;
  std::size_t bytes = 0;
  static cairo_status_t write(void* closure, const unsigned char* bytes, unsigned length) {
    auto& self = *static_cast<Stream*>(closure);
    if (self.control.cancelled.load() || length > 128 * 1024 * 1024 - self.bytes)
      return CAIRO_STATUS_WRITE_ERROR;
    self.bytes += length;
    while (length) {
      if (self.control.cancelled.load())
        return CAIRO_STATUS_WRITE_ERROR;
      auto count = ::write(self.fd, bytes, length);
      if (count < 0 && errno == EINTR)
        continue;
      if (count <= 0)
        return CAIRO_STATUS_WRITE_ERROR;
      bytes += count;
      length -= count;
    }
    return CAIRO_STATUS_SUCCESS;
  }
};
} // namespace
void PdfOutput::write(const RenderFrame& frame, FontCatalog& fonts, int fd,
                      ExportControl& control) {
  const auto& paper = frame.pages.paper;
  paper.validate();
  if (frame.key.profile.mode != LayoutMode::Paged || frame.pages.slices.empty() ||
      frame.pages.slices.size() > 2000)
    throw Error(ErrorCode::Layout, "PDF requires a complete paged frame.");
  Stream stream{fd, control};
  std::unique_ptr<cairo_surface_t, decltype(&cairo_surface_destroy)> surface(
      cairo_pdf_surface_create_for_stream(Stream::write, &stream, paper.width, paper.height),
      cairo_surface_destroy);
  std::unique_ptr<cairo_t, decltype(&cairo_destroy)> cr(cairo_create(surface.get()), cairo_destroy);
  DisplayListPainter painter(fonts);
  control.total = frame.pages.slices.size();
  for (std::size_t page = 0; page < frame.pages.slices.size(); ++page) {
    control.checkpoint();
    cairo_save(cr.get());
    cairo_translate(cr.get(), 0, -double(page) * paper.height);
    painter.paint(frame, cr.get(), {0, page * paper.height, paper.width, paper.height});
    cairo_restore(cr.get());
    cairo_show_page(cr.get());
    control.page = page + 1;
    if (cairo_status(cr.get()) != CAIRO_STATUS_SUCCESS)
      throw Error(ErrorCode::Io, cairo_status_to_string(cairo_status(cr.get())));
  }
  cairo_surface_finish(surface.get());
  control.checkpoint();
  if (cairo_surface_status(surface.get()) != CAIRO_STATUS_SUCCESS)
    throw Error(ErrorCode::Io,
                std::string("Cannot write PDF (disk, output limit or font error): ") +
                    cairo_status_to_string(cairo_surface_status(surface.get())));
}
} // namespace xfmd
