#include "BoxUiLayout.h"
#include "contracts/boxui/private/BoxUiCodec.h"
#include <cmath>
namespace xfmd {
namespace {
struct Context {
  ITextMetrics& metrics;
  const std::function<bool()>& cancel;
};
uint32_t measure(void* opaque, const uint8_t* text, uint64_t n, double size, double* w, double* h,
                 double* baseline) noexcept {
  try {
    if (!text || n > 65536 || !std::isfinite(size) || size <= 0 || size > 512)
      return 1;
    FontSpec font;
    font.points = std::max(1, int(std::round(size * .75)));
    auto v = static_cast<Context*>(opaque)->metrics.measure(
        {reinterpret_cast<const char*>(text), std::size_t(n)}, font);
    *w = v.width * size / font.points;
    *h = v.height * size / font.points;
    *baseline = v.ascent * size / font.points;
    return 0;
  } catch (...) {
    return 1;
  }
}
uint32_t cancel(void* opaque) noexcept {
  try {
    auto& f = static_cast<Context*>(opaque)->cancel;
    return f && f();
  } catch (...) {
    return 1;
  }
}
} // namespace
std::shared_ptr<const BoxUiFrame> BoxUiLayout::prepare(const BoxUiModel& model,
                                                       const BoxUiLayoutRequest& request,
                                                       ITextMetrics& metrics) {
  using namespace boxUiCodec;
  Json children = Json::array();
  for (auto& c : request.children) {
    if (!c.error.empty())
      children.push_back({{"ref", c.ref}, {"error", c.error}});
    else
      children.push_back(
          {{"ref", c.ref}, {"width", c.width}, {"height", c.height}, {"svg", c.svg}});
  }
  Json input = {{"contract", contract},
                {"key", key(request.key)},
                {"model", boxUiCodec::model(model)},
                {"snapshot", snapshot(model, request.snapshot)},
                {"viewport", {{"width", request.width}, {"height", request.height}}},
                {"palette",
                 {{"background", "#ffffffff"},
                  {"foreground", "#0f172aff"},
                  {"surface", "#f8fafcff"},
                  {"border", "#94a3b8ff"},
                  {"accent", "#2563ebff"},
                  {"muted", "#64748bff"},
                  {"error", "#dc2626ff"}}},
                {"fontSignature", std::to_string(metrics.fontSetId())},
                {"budgetMs", request.budgetMs},
                {"childProfiles", {"XFMD Flowchart 1", "XFMD Sequence 2", "XFMD State 1"}},
                {"children", children}};
  auto bytes = input.dump();
  Context context{metrics, request.cancelled};
  auto j = result(xfmd_boxui_prepare_v1(1, reinterpret_cast<const uint8_t*>(bytes.data()),
                                        bytes.size(), &context, measure, cancel));
  if (j.at("key") != key(request.key))
    throw Error(ErrorCode::Conflict, "BoxUI key mismatch");
  auto frame = std::make_shared<BoxUiFrame>();
  frame->key = request.key;
  frame->snapshot = request.snapshot;
  auto w = j.at("width").get<double>(), h = j.at("height").get<double>();
  if (!std::isfinite(w) || !std::isfinite(h) || w <= 0 || h <= 0 || w > 8192 || h > 8192)
    throw Error(ErrorCode::Layout, "Invalid BoxUI extent");
  frame->width = w * .75;
  frame->height = h * .75;
  auto scene = [&](const char* name) {
    auto v = std::make_shared<DiagramScene>();
    v->width = frame->width;
    v->height = frame->height;
    v->fonts = metrics.fontSetId();
    v->svg = j.at(name);
    v->bytes = v->svg.size();
    frame->bytes += v->bytes;
    return v;
  };
  frame->staticScene = scene("staticSvg");
  frame->previewScene = scene("previewSvg");
  for (auto& c : j.at("controls")) {
    BoxUiControl control;
    control.id = c.at("id");
    control.kind = kind(c.at("kind").get<std::string>());
    control.version = c.at("version");
    control.rect = rect(c.at("rect"));
    control.clip = rect(c.at("clip"));
    control.enabled = c.at("enabled");
    control.role = c.at("role");
    control.accessibleName = c.at("accessibleName");
    control.commandBinding = c.at("commandBinding");
    control.valueBinding = c.value("valueBinding", std::string{});
    control.valueType = c.at("valueType");
    if (control.rect.x < 0 || control.rect.y < 0 || control.rect.x + control.rect.width > w + .01 ||
        control.rect.y + control.rect.height > h + .01)
      throw Error(ErrorCode::Layout, "BoxUI control outside frame");
    frame->controls.push_back(std::move(control));
  }
  for (auto& d : j.at("diagnostics"))
    frame->diagnostics.push_back(d.at("message"));
  return frame;
}
} // namespace xfmd
