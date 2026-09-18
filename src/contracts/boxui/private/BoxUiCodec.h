#pragma once
// Private ABI codec; JSON does not enter public model or application contracts.
#include "contracts/boxui/BoxUiAbi.h"
#include "contracts/boxui/BoxUiFrame.h"
#include "nlohmann/json.hpp"
namespace xfmd::boxUiCodec {
using Json = nlohmann::json;
constexpr auto contract = "BX-HOST/0.1-draft1";
Json result(XfmdDiagramResult);
BoxUiKind kind(const std::string&);
std::string kind(BoxUiKind);
BoxUiNode node(const Json&);
Json node(const BoxUiNode&);
BoxUiModel model(const Json&);
Json model(const BoxUiModel&);
Json key(const BoxUiKey&);
Json snapshot(const BoxUiModel&, const BoxUiSnapshot&);
Rect rect(const Json&);
} // namespace xfmd::boxUiCodec
