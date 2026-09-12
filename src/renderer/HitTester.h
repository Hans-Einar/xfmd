#pragma once
#include "contracts/RenderFrame.h"
namespace xfmd {
class HitTester { public: static HitResult hitTest(const RenderFrame&, Point); };
}
