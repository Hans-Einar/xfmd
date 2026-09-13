#pragma once
#include "DisplayListPainter.h"
#include "application/export/ExportCoordinator.h"
namespace xfmd {
class PdfOutput {
public:
  static void write(const RenderFrame&, FontCatalog&, int fd, ExportControl&);
};
} // namespace xfmd
