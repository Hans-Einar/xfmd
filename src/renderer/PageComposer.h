#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
class PageComposer {
public:
  static void compose(RenderFrame&,const PaperSpec&,const std::function<bool()>& cancelled={});
};
} // namespace xfmd
