#pragma once
#include "AnchorMapper.h"
#include "contracts/IRenderer.h"
#include <functional>
#include <optional>
namespace xfmd {
enum class ViewOrigin { Editor, Preview };
class ScrollCoordinator {
  LayoutResult frame;
  DocumentToken expected;
  bool valid = false, synchronizing = false;
  bool split = false;
  SourceAnchor last;
  std::optional<SourceAnchor> pending;
  std::uint64_t lastSequence = 0;

public:
  std::function<void(SourceAnchor)> setEditor;
  std::function<void(int)> setPreview;
  void setSplit(bool enabled) { split = enabled; }
  void invalidate(DocumentToken);
  void setFrame(LayoutResult);
  void onViewportChanged(ViewOrigin, std::size_t position, DocumentToken,
                         std::uint64_t sequence = 0, bool programmatic = false);
  SourceAnchor captureAnchor() const { return last; }
  void restoreAnchor(SourceAnchor);
};
} // namespace xfmd
