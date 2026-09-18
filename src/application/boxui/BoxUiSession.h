#pragma once
#include "SyntheticActivity.h"
#include "contracts/IInterpreter.h"
#include <mutex>
namespace xfmd {
struct BoxUiIntent {
  BoxUiKey key;
  std::string widgetId, bindingId, contextRevision, eventId, commandId, expectedValueRevision;
  BoxUiValue value;
};
struct BoxUiCommandResult {
  std::string status, message, commandId;
};
class BoxUiSession {
  mutable std::mutex mutex;
  DocumentToken token;
  bool enabled = false;
  BoxUiState state;
  std::map<std::string, std::shared_ptr<const BoxUiModel>> models;
  std::map<std::string, SyntheticActivity> participants;
  struct Entry {
    BoxUiIntent intent;
    BoxUiCommandResult result;
  };
  std::map<std::string, Entry> ledger;
  void snapshots();

public:
  void expect(DocumentToken);
  bool simulated() const;
  bool viewport(double);
  void toggle(ParseResult);
  BoxUiState freeze(DocumentToken) const;
  bool current(const BoxUiFrame&) const;
  BoxUiCommandResult dispatch(const BoxUiIntent&);
};
} // namespace xfmd
