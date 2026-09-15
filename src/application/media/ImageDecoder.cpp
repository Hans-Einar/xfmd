#include "ImageDecoder.h"
#include "GifBudget.h"
#include "contracts/DocumentTypes.h"
#include <fcntl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
namespace xfmd {
namespace {
struct File {
  int fd;
  ~File() {
    if (fd >= 0)
      close(fd);
  }
};
void sizePrepared(GdkPixbufLoader* loader, int width, int height, void* data) {
  if (width <= 0 || height <= 0 || std::uint64_t(width) * height > 16000000) {
    *static_cast<bool*>(data) = true;
    gdk_pixbuf_loader_set_size(loader, 1, 1);
  }
}
} // namespace
std::shared_ptr<const VisualResource> ImageDecoder::load(const std::string& path) {
  File file{open(path.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC)};
  struct stat info{};
  if (file.fd < 0 || fstat(file.fd, &info) || !S_ISREG(info.st_mode))
    throw Error(ErrorCode::Io, "Image is not a readable regular file.");
  if (info.st_size <= 0 || info.st_size > 16 * 1024 * 1024)
    throw Error(ErrorCode::TooLarge, "Image exceeds 16 MiB or is empty.");
  std::vector<unsigned char> bytes(info.st_size);
  std::size_t offset = 0;
  while (offset < bytes.size()) {
    auto size = read(file.fd, bytes.data() + offset, bytes.size() - offset);
    if (size <= 0)
      throw Error(ErrorCode::Io, "Unable to read image.");
    offset += size;
  }
  checkGifBudget(bytes);
  std::unique_ptr<GdkPixbufLoader, decltype(&g_object_unref)> loader(gdk_pixbuf_loader_new(),
                                                                     g_object_unref);
  bool oversized = false;
  g_signal_connect(loader.get(), "size-prepared", G_CALLBACK(sizePrepared), &oversized);
  GError* error = nullptr;
  bool ok = gdk_pixbuf_loader_write(loader.get(), bytes.data(), bytes.size(), &error);
  if (error) {
    g_error_free(error);
    error = nullptr;
  }
  ok = gdk_pixbuf_loader_close(loader.get(), &error) && ok;
  if (error)
    g_error_free(error);
  auto* pixbuf = gdk_pixbuf_loader_get_pixbuf(loader.get());
  if (!ok || !pixbuf || oversized)
    throw Error(ErrorCode::InvalidInput, "Invalid image or image exceeds 16 million pixels.");
  auto* format = gdk_pixbuf_loader_get_format(loader.get());
  char* name = gdk_pixbuf_format_get_name(format);
  const std::string type = name ? name : "";
  g_free(name);
  if (type != "png" && type != "jpeg" && type != "gif" && type != "svg")
    throw Error(ErrorCode::Unsupported, "Supported images: PNG, JPEG, GIF and SVG.");
  int width = gdk_pixbuf_get_width(pixbuf), height = gdk_pixbuf_get_height(pixbuf);
  auto* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
    cairo_surface_destroy(surface);
    throw Error(ErrorCode::TooLarge, "Unable to allocate image.");
  }
  const auto* source = gdk_pixbuf_read_pixels(pixbuf);
  int channels = gdk_pixbuf_get_n_channels(pixbuf), stride = gdk_pixbuf_get_rowstride(pixbuf);
  auto* target = cairo_image_surface_get_data(surface);
  int targetStride = cairo_image_surface_get_stride(surface);
  for (int y = 0; y < height; ++y) {
    auto* row = reinterpret_cast<std::uint32_t*>(target + y * targetStride);
    for (int x = 0; x < width; ++x) {
      auto* pixel = source + y * stride + x * channels;
      unsigned alpha = channels == 4 ? pixel[3] : 255;
      row[x] = (alpha << 24) | (((pixel[0] * alpha + 127) / 255) << 16) |
               (((pixel[1] * alpha + 127) / 255) << 8) | ((pixel[2] * alpha + 127) / 255);
    }
  }
  cairo_surface_mark_dirty(surface);
  // Markdown image pixels use CSS's 96 dpi convention in the point-based document.
  return std::make_shared<CairoVisual>(surface, width * .75, height * .75, height * .75, false);
}
} // namespace xfmd
