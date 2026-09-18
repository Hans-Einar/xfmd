#pragma once
#include "contracts/boxui/BoxUiFrame.h"
namespace xfmd {
// Local test participant. It grants no production/domain authority.
class SyntheticActivity {
  bool suspended = false;
  std::string context = "C1", suspendedContext = "C1";
  std::uint64_t revision = 1, contextRevision = 1, sourceSession = 1, observation = 1;
  BoxUiValueState measurement{12.0, "current", "1", "1"};

public:
  BoxUiSnapshot snapshot(const BoxUiModel&) const;
  std::string execute(const std::string& command, const BoxUiValue& value);
  bool observe(std::uint64_t session, std::uint64_t sequence, double value);
  void resetSource();
};
} // namespace xfmd
