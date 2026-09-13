#pragma once
#include "ExportCoordinator.h"
#include "application/adapters/SharedTextMetrics.h"
#include "application/io/PdfFilePublisher.h"
#include "contracts/IInterpreter.h"
namespace xfmd {
class ExportPipeline {
public:
  static unsigned run(const ExportRequest&, const PdfTarget&, ExportControl&, IInterpreter&,
                      IRenderer&, SharedTextMetrics&);
};
} // namespace xfmd
