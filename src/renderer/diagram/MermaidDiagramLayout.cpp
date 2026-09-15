#include "MermaidDiagramLayout.h"
#include "DiagramTextLayout.h"
#include "contracts/diagram/DiagramLimits.h"
#include "contracts/diagram/DiagramWire.h"
#include <algorithm>
namespace xfmd {
std::shared_ptr<const DiagramScene>
MermaidDiagramLayout::layout(const DiagramModel& model, const DiagramLayoutRequest& request,
                             ITextMetrics& metrics) {
  auto checkpoint = [&] {
    if (request.cancelled && request.cancelled())
      throw Error(ErrorCode::Layout, "Diagram preparation cancelled");
  };
  checkpoint();
  auto texts = DiagramTextLayout::measure(model, metrics);
  diagramWire::Writer w;
  w.model(model);
  w.integer(diagramLayoutBudgetMilliseconds);
  w.integer(texts.size());
  for (const auto& pair : texts) {
    w.text(pair.first);
    w.number(pair.second.width / .75);
    w.number(pair.second.height / .75);
  }
  diagramWire::ResultOwner result(xfmd_diagram_layout_v1(1, w.data.data(), w.data.size()),
                                  xfmd_diagram_layout_free_v1);
  checkpoint();
  diagramWire::Reader r(result.result);
  auto scene = std::make_shared<DiagramScene>();
  scene->fonts = metrics.fontSetId();
  scene->width = r.number() * .75;
  scene->height = r.number() * .75;
  if (scene->width <= 0 || scene->height <= 0)
    throw Error(ErrorCode::Layout, "Empty diagram layout");
  auto rect = [&] {
    double x = r.number() * .75, y = r.number() * .75, w = r.number() * .75, h = r.number() * .75;
    if (w <= 0 || h <= 0)
      throw Error(ErrorCode::Layout, "Invalid diagram box");
    return Rect{x, y, w, h};
  };
  auto label = [&](const std::string& text, double x, double y, bool background) {
    const auto& measured = texts.at(text);
    for (auto line : measured.lines) {
      line.origin.x = x - line.extent.width / 2;
      line.origin.y += y - measured.height / 2;
      line.background = background;
      scene->labels.push_back(std::move(line));
    }
  };
  const auto count = r.count(128);
  if (count != model.nodes.size())
    throw Error(ErrorCode::Layout, "Diagram node mismatch");
  for (unsigned i = 0; i < count; ++i)
    scene->nodes.push_back({rect(), model.nodes[i].shape});
  const auto groups = r.count(32);
  if (groups != model.groups.size())
    throw Error(ErrorCode::Layout, "Diagram group mismatch");
  for (unsigned i = 0; i < groups; ++i) {
    auto id = r.text();
    auto box = rect();
    auto height = r.number() * .75;
    if (id != model.groups[i].id)
      throw Error(ErrorCode::Layout, "Diagram group order mismatch");
    scene->groups.push_back(box);
    label(model.groups[i].label, box.x + box.width / 2, box.y + height / 2 + 6, false);
  }
  for (unsigned i = 0; i < count; ++i) {
    const auto& box = scene->nodes[i].bounds;
    label(model.nodes[i].label, box.x + box.width / 2, box.y + box.height / 2, false);
  }
  const auto edges = r.count(512);
  if (edges != model.edges.size())
    throw Error(ErrorCode::Layout, "Diagram edge mismatch");
  for (unsigned i = 0; i < edges; ++i) {
    DiagramEdgePath path;
    path.start = model.edges[i].arrowStart;
    path.end = model.edges[i].arrowEnd;
    path.style = model.edges[i].style;
    auto points = r.count(65536);
    if (points < 2)
      throw Error(ErrorCode::Layout, "Empty diagram edge");
    for (unsigned j = 0; j < points; ++j) {
      double x = r.number() * .75, y = r.number() * .75;
      path.points.push_back({x, y});
    }
    scene->edges.push_back(std::move(path));
    double x = r.number() * .75, y = r.number() * .75;
    if (!model.edges[i].label.empty())
      label(model.edges[i].label, x, y, true);
  }
  r.finish();
  scene->bytes = result.result.size;
  for (const auto& l : scene->labels)
    scene->bytes += sizeof(l) + l.text.size() * 128;
  if (scene->bytes > 8 * 1024 * 1024)
    throw Error(ErrorCode::TooLarge, "Diagram scene exceeds budget");
  return scene;
}
} // namespace xfmd
