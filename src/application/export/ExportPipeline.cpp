#include "ExportPipeline.h"
#include "application/adapters/PdfOutput.h"
#include "application/composition/DiagramServices.h"
namespace xfmd {
unsigned ExportPipeline::run(const ExportRequest& request, const PdfTarget& target,
                             ExportControl& control, IInterpreter& parser, IRenderer& renderer,
                             SharedTextMetrics& metrics) {
  control.checkpoint();
  request.paper.validate();
  if (metrics.fontSetId() != request.fonts)
    throw Error(ErrorCode::Conflict, "Font set changed; refresh preview and export again.");
  auto frame = request.frame;
  LayoutProfile profile{LayoutMode::Paged, request.paper};
  if (!frame || frame->key.token != request.source.token || !(frame->key.profile == profile) ||
      frame->key.fonts != request.fonts) {
    auto model = DiagramServices::prepare(parser.parse(request.source), request.source, metrics, [&]{return control.cancelled.load();});
    control.checkpoint();
    frame = renderer.layout(
        *model, {request.paper.width, 0, profile, [&] { return control.cancelled.load(); }},
        metrics);
  }
  control.checkpoint();
  PdfFilePublisher publisher(target);
  PdfOutput::write(*frame, metrics.catalog, publisher.fd(), control);
  publisher.commit(control);
  return frame->pages.slices.size();
}
} // namespace xfmd
