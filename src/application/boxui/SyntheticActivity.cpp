#include "SyntheticActivity.h"
#include <cmath>
namespace xfmd {
BoxUiSnapshot SyntheticActivity::snapshot(const BoxUiModel& model) const {
  BoxUiSnapshot s;
  s.contextRevision = std::to_string(contextRevision);
  for (auto& b : model.bindings) {
    if (b.role == "value") {
      BoxUiValueState v;
      v.revision = std::to_string(revision);
      v.sourceSession = "local-activity";
      if (b.id == "measurement" && b.type == "number")
        v = measurement;
      else if (b.id == "context" && b.type == "string") {
        v.value = context;
        v.validity = "current";
      } else if (b.id == "activity-id" && b.type == "string") {
        v.value = std::string("A1");
        v.validity = "current";
      } else if (b.id == "status" && b.type == "string") {
        v.value = std::string(suspended ? "suspended" : "running");
        v.validity = "current";
      } else if (b.id == "progress" && b.type == "number") {
        v.value = 12.;
        v.validity = "current";
      }
      s.values.emplace(b.id, std::move(v));
    } else {
      BoxUiCommandState c;
      c.argumentType = b.type;
      bool known =
          (b.id == "set-context" && b.type == "string") ||
          (b.type == "none" && (b.id == "suspend" || b.id == "resume" || b.id == "mark-stale" ||
                                b.id == "reset-source" || b.id == "observe"));
      c.provenance = known ? "simulated" : "unbound";
      c.enabled = known && (b.id != "suspend" || !suspended) && (b.id != "resume" || suspended);
      c.reason = known ? (c.enabled ? "Local synthetic participant"
                                    : (suspended ? "suspended" : "running"))
                       : "unbound";
      s.commands.emplace(b.id, std::move(c));
    }
  }
  return s;
}
std::string SyntheticActivity::execute(const std::string& command, const BoxUiValue& value) {
  if (command == "set-context") {
    auto* text = std::get_if<std::string>(&value);
    if (!text || text->empty())
      return "Context must be nonempty";
    if (*text != context) {
      context = *text;
      ++contextRevision;
    }
  } else if (command == "suspend") {
    if (suspended)
      return "Activity is already suspended";
    suspended = true;
    suspendedContext = context;
  } else if (command == "resume") {
    if (!suspended)
      return "Activity is not suspended";
    if (context != suspendedContext)
      return "Resume rejected: context changed; original Activity A1 retained";
    suspended = false;
  } else if (command == "mark-stale") {
    measurement.validity = "stale";
    measurement.revision = std::to_string(++revision);
  } else if (command == "reset-source")
    resetSource();
  else if (command == "observe")
    observe(sourceSession, observation + 1, 13.);
  else
    return "Unknown synthetic command";
  ++revision;
  return {};
}
bool SyntheticActivity::observe(std::uint64_t session, std::uint64_t sequence, double value) {
  if (session != sourceSession || sequence <= observation || !std::isfinite(value))
    return false;
  observation = sequence;
  measurement = {value, "current", std::to_string(++revision), std::to_string(sourceSession)};
  return true;
}
void SyntheticActivity::resetSource() {
  ++sourceSession;
  observation = 0;
  measurement = {std::monostate{}, "missing", std::to_string(++revision),
                 std::to_string(sourceSession)};
}
} // namespace xfmd
