#include "BoxUiCodec.h"
#include <cmath>
#include <set>
namespace xfmd::boxUiCodec {
namespace {
const std::pair<const char*, std::optional<std::string> BoxUiNode::*> stringFields[] = {
    {"label", &BoxUiNode::label},
    {"text", &BoxUiNode::text},
    {"valueBinding", &BoxUiNode::valueBinding},
    {"commandBinding", &BoxUiNode::commandBinding},
    {"childRef", &BoxUiNode::childRef},
    {"family", &BoxUiNode::family}};
}

Json result(XfmdDiagramResult r) {
  struct Owner {
    XfmdDiagramResult& r;
    ~Owner() { xfmd_boxui_result_free_v1(&r); }
  } owner{r};
  if (r.abi_version != 1 || r.struct_size != sizeof(r) || !r.data || r.size > 8 * 1024 * 1024)
    throw Error(ErrorCode::Parse, "Invalid BoxUI ABI result");
  std::vector<std::set<std::string>> keys;
  auto callback = [&](int depth, Json::parse_event_t event, Json& value) {
    if (depth > 64)
      throw Error(ErrorCode::TooLarge, "BoxUI JSON depth");
    if (event == Json::parse_event_t::object_start)
      keys.emplace_back();
    if (event == Json::parse_event_t::key && !keys.back().insert(value.get<std::string>()).second)
      throw Error(ErrorCode::Parse, "Duplicate BoxUI result key");
    if (event == Json::parse_event_t::object_end)
      keys.pop_back();
    return true;
  };
  auto j = Json::parse(r.data, r.data + r.size, callback);
  if (j.at("contract") != contract)
    throw Error(ErrorCode::Unsupported, "BoxUI contract mismatch");
  if (r.status)
    throw Error(ErrorCode::Parse, j.at("diagnostics").at(0).at("message").get<std::string>());
  return j;
}
BoxUiKind kind(const std::string& s) {
  if (s == "row")
    return BoxUiKind::Row;
  if (s == "column")
    return BoxUiKind::Column;
  if (s == "text")
    return BoxUiKind::Text;
  if (s == "value")
    return BoxUiKind::Value;
  if (s == "button")
    return BoxUiKind::Button;
  if (s == "input")
    return BoxUiKind::Input;
  if (s == "diagram")
    return BoxUiKind::Diagram;
  throw Error(ErrorCode::Unsupported, "Unknown BoxUI kind");
}
std::string kind(BoxUiKind k) {
  static const char* names[] = {"row", "column", "text", "value", "button", "input", "diagram"};
  return names[static_cast<unsigned>(k)];
}
BoxUiNode node(const Json& j) {
  BoxUiNode n;
  n.id = j.at("id");
  n.kind = kind(j.at("kind").get<std::string>());
  n.version = j.at("version");
  if (j.contains("size")) {
    auto& z = j.at("size");
    if (z.contains("min"))
      n.min = z.at("min");
    if (z.contains("max"))
      n.max = z.at("max");
    if (z.contains("grow"))
      n.grow = z.at("grow");
  }
  for (auto pair : stringFields)
    if (j.contains(pair.first))
      n.*pair.second = j.at(pair.first).get<std::string>();
  if (j.contains("children"))
    for (auto& c : j.at("children"))
      n.children.push_back(node(c));
  return n;
}
Json node(const BoxUiNode& n) {
  Json j = {{"id", n.id}, {"kind", kind(n.kind)}, {"version", n.version}};
  Json z = Json::object();
  if (n.min)
    z["min"] = *n.min;
  if (n.max)
    z["max"] = *n.max;
  if (n.grow)
    z["grow"] = *n.grow;
  j["size"] = z;
  for (auto pair : stringFields)
    if (n.*pair.second)
      j[pair.first] = *(n.*pair.second);
  if (!n.children.empty()) {
    j["children"] = Json::array();
    for (auto& c : n.children)
      j["children"].push_back(node(c));
  }
  return j;
}
BoxUiModel model(const Json& j) {
  if (j.at("profile") != "boxui/0.1")
    throw Error(ErrorCode::Unsupported, "BoxUI profile");
  BoxUiModel m;
  m.documentId = j.at("documentId");
  m.root = node(j.at("root"));
  for (auto& b : j.at("bindings"))
    m.bindings.push_back({b.at("id"), b.at("role"), b.at("type")});
  return m;
}
Json model(const BoxUiModel& m) {
  Json j = {{"profile", "boxui/0.1"},
            {"documentId", m.documentId},
            {"root", node(m.root)},
            {"bindings", Json::array()}};
  for (auto& b : m.bindings)
    j["bindings"].push_back({{"id", b.id}, {"role", b.role}, {"type", b.type}});
  return j;
}
Json key(const BoxUiKey& k) {
  return {{"session", std::to_string(k.source.document)},
          {"epoch", std::to_string(k.epoch)},
          {"blockId", k.blockId},
          {"incarnation", std::to_string(k.source.revision)},
          {"sourceRevision", std::to_string(k.source.revision)},
          {"bindingRevision", std::to_string(k.bindingRevision)},
          {"stateRevision", std::to_string(k.stateRevision)},
          {"frameSequence", std::to_string(k.frameSequence)},
          {"viewportRevision", "0"},
          {"themeRevision", "0"}};
}
Json snapshot(const BoxUiModel& m, const BoxUiSnapshot& s) {
  Json j = {{"contextRevision", s.contextRevision},
            {"values", Json::array()},
            {"commands", Json::array()}};
  for (auto& b : m.bindings) {
    if (b.role == "value") {
      auto it = s.values.find(b.id);
      BoxUiValueState v = it == s.values.end() ? BoxUiValueState{} : it->second;
      Json value = nullptr;
      std::visit(
          [&](auto&& x) {
            using T = std::decay_t<decltype(x)>;
            if constexpr (!std::is_same_v<T, std::monostate>)
              value = x;
          },
          v.value);
      j["values"].push_back({{"id", b.id},
                             {"type", b.type},
                             {"value", value},
                             {"validity", v.validity},
                             {"revision", v.revision},
                             {"sourceSession", v.sourceSession}});
    } else {
      auto it = s.commands.find(b.id);
      BoxUiCommandState c = it == s.commands.end() ? BoxUiCommandState{} : it->second;
      j["commands"].push_back({{"id", b.id},
                               {"argumentType", b.type},
                               {"enabled", c.enabled},
                               {"reason", c.reason},
                               {"provenance", c.provenance}});
    }
  }
  return j;
}
Rect rect(const Json& j) {
  Rect r{j.at("x"), j.at("y"), j.at("width"), j.at("height")};
  if (!std::isfinite(r.x) || !std::isfinite(r.y) || !std::isfinite(r.width) ||
      !std::isfinite(r.height) || r.width <= 0 || r.height <= 0)
    throw Error(ErrorCode::Layout, "Invalid BoxUI rectangle");
  return r;
}
} // namespace xfmd::boxUiCodec
