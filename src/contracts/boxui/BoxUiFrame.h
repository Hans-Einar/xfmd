#pragma once
#include "BoxUiModel.h"
#include "contracts/diagram/DiagramScene.h"
#include <map>
#include <variant>
namespace xfmd {
using BoxUiValue = std::variant<std::monostate, std::string, double, bool>;
struct BoxUiValueState {
  BoxUiValue value;
  std::string validity = "missing", revision = "0", sourceSession;
};
struct BoxUiCommandState {
  std::string argumentType, reason, provenance = "unbound";
  bool enabled = false;
};
struct BoxUiSnapshot {
  std::string contextRevision = "0";
  std::map<std::string, BoxUiValueState> values;
  std::map<std::string, BoxUiCommandState> commands;
};
struct BoxUiState {
  double viewportWidth = 640;
  std::uint64_t epoch = 0, bindingRevision = 0, stateRevision = 0;
  std::map<std::string, BoxUiSnapshot> blocks;
};
struct BoxUiKey {
  DocumentToken source;
  std::string blockId;
  std::uint64_t epoch = 0, bindingRevision = 0, stateRevision = 0, frameSequence = 0;
};
struct BoxUiControl {
  std::string id, role, accessibleName, commandBinding, valueBinding, valueType;
  BoxUiKind kind = BoxUiKind::Button;
  unsigned version = 1;
  Rect rect, clip; // SVG logical pixels; one authoritative geometry map.
  bool enabled = false;
};
struct BoxUiFrame final : VisualResource {
  BoxUiKey key;
  BoxUiSnapshot snapshot;
  std::shared_ptr<const DiagramScene> staticScene, previewScene;
  std::vector<BoxUiControl> controls;
  std::vector<std::string> diagnostics;
  std::size_t bytes = 0;
};
struct BoxUiPreparedChild {
  std::string ref, svg, error;
  double width = 0, height = 0;
};
struct BoxUiLayoutRequest {
  BoxUiKey key;
  BoxUiSnapshot snapshot;
  std::vector<BoxUiPreparedChild> children;
  double width = 640, height = 480;
  unsigned budgetMs = 2000;
  std::function<bool()> cancelled;
};
} // namespace xfmd
