#pragma once
#include "contracts/DocumentTypes.h"
#include "contracts/diagram/DiagramModel.h"
#include <memory>
#include <optional>
#include <vector>
namespace xfmd {
enum class BoxUiKind { Row, Column, Text, Value, Button, Input, Diagram };
struct BoxUiBinding {
  std::string id, role, type;
};
struct BoxUiNode {
  std::string id;
  BoxUiKind kind = BoxUiKind::Text;
  unsigned version = 1;
  std::optional<double> min, max, grow;
  std::optional<std::string> label, text, valueBinding, commandBinding, childRef, family;
  std::vector<BoxUiNode> children;
};
struct BoxUiChild {
  std::string ref, family, error;
  SourceRange source;
  std::shared_ptr<const DiagramModel> model;
};
struct BoxUiModel {
  std::string documentId;
  std::vector<BoxUiBinding> bindings;
  BoxUiNode root;
  std::vector<BoxUiChild> children;
};
} // namespace xfmd
