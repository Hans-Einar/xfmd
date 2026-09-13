#pragma once
#include "ITextMetrics.h"
#include "DocumentTypes.h"
#include <cmath>
#include <functional>
namespace xfmd {
enum class LayoutMode { Continuous, Paged };
struct PaperSpec {
  LayoutUnit width=210*72/25.4,height=297*72/25.4,margin=20*72/25.4;
  bool operator==(const PaperSpec& p) const {return width==p.width && height==p.height && margin==p.margin;}
  void validate() const {
    if(!std::isfinite(width)||!std::isfinite(height)||!std::isfinite(margin)||margin<0||
       width<72||height<72||width>2000||height>2000||width-2*margin<40||height-2*margin<40)
      throw Error(ErrorCode::Layout,"Invalid paper size or margins.");
  }
};
struct LayoutProfile {
  LayoutMode mode=LayoutMode::Continuous;
  PaperSpec paper;
  bool operator==(const LayoutProfile& p) const {return mode==p.mode && paper==p.paper;}
};
struct FrameKey {
  DocumentToken token;
  LayoutProfile profile;
  FontSetId fonts=0;
  std::uint64_t generation=0;
  LayoutUnit flowWidth=0;
  bool operator==(const FrameKey& k) const {
    return token==k.token && profile==k.profile && fonts==k.fonts && generation==k.generation && flowWidth==k.flowWidth;
  }
};
} // namespace xfmd
