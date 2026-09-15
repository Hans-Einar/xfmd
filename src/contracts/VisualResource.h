#pragma once
#include <memory>
#include <string>
namespace xfmd {
// Immutable presentation resource. The application supplies the native drawing adapter;
// interpretation and layout see only dimensions and shared lifetime.
struct VisualResource {
  double width = 0, height = 0, ascent = 0;
  virtual ~VisualResource() = default;
};
enum class EmbeddedKind { None, Image, Math };
struct EmbeddedContent {
  EmbeddedKind kind = EmbeddedKind::None;
  std::string source;
  bool display = false;
  std::shared_ptr<const VisualResource> visual;
};
} // namespace xfmd
