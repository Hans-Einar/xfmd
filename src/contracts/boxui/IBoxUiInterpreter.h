#pragma once
#include "BoxUiModel.h"
namespace xfmd {
class IBoxUiInterpreter {
public:
  virtual ~IBoxUiInterpreter() = default;
  virtual std::shared_ptr<const BoxUiModel> parse(const std::string&) = 0;
};
} // namespace xfmd
