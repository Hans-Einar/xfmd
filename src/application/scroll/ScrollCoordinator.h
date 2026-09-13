#pragma once
#include "AnchorMapper.h"
#include "ScrollDynamics.h"
#include "contracts/IRenderer.h"
#include <functional>
#include <optional>
namespace xfmd {
enum class ViewOrigin { Editor, Preview };
class ScrollCoordinator {
  LayoutResult frame;
  std::optional<FrameKey> expectedFrame;
  DocumentToken expected;
  bool valid = false, synchronizing = false;
  bool split = false;
  SourceAnchor last;
  std::optional<SourceAnchor> pending;
  std::uint64_t lastSequence = 0;

public:
  std::function<void(SourceAnchor)> setEditor;
  std::function<void(double)> setPreview;
  void setSplit(bool enabled) { split = enabled; }
  void invalidate(DocumentToken);
  void setFrame(LayoutResult);
  void expectLayout(FrameKey key) {
    expectedFrame = std::move(key);
    valid = false;
  }
  void onViewportChanged(ViewOrigin, double position, DocumentToken, std::uint64_t sequence = 0,
                         bool programmatic = false, ScrollOrigin input = ScrollOrigin::UserWheel);
  SourceAnchor captureAnchor() const { return last; }
  void restoreAnchor(SourceAnchor);
};
} // namespace xfmd
