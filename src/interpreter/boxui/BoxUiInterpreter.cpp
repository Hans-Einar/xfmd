#include "BoxUiInterpreter.h"
#include "contracts/boxui/private/BoxUiCodec.h"
namespace xfmd {
std::shared_ptr<const BoxUiModel> BoxUiInterpreter::parse(const std::string& source) {
  auto j = boxUiCodec::result(
      xfmd_boxui_parse_v1(1, reinterpret_cast<const uint8_t*>(source.data()), source.size()));
  auto model = std::make_shared<BoxUiModel>(boxUiCodec::model(j.at("model")));
  for (auto& c : j.at("childSources")) {
    BoxUiChild child;
    child.ref = c.at("ref");
    child.family = c.at("family");
    child.source = {c.at("sourceStart"), c.at("sourceEnd"), MappingQuality::Approximate};
    if (child.source.begin > child.source.end || child.source.end > source.size())
      throw Error(ErrorCode::Parse, "Invalid BoxUI child source span");
    try {
      if (!diagrams)
        throw Error(ErrorCode::Unsupported, "Child diagrams unavailable");
      auto parsed = diagrams->parse({c.at("source").get<std::string>(), child.source});
      child.model = parsed.model;
      child.error = parsed.error;
    } catch (const std::exception& e) {
      child.error = e.what();
    }
    model->children.push_back(std::move(child));
  }
  return model;
}
} // namespace xfmd
