#include "BoxUiSession.h"
#include <algorithm>
#include <cmath>
namespace xfmd {
namespace {
const BoxUiNode* find(const BoxUiNode& n, const std::string& id) {
  if (n.id == id)
    return &n;
  for (auto& c : n.children)
    if (auto* found = find(c, id))
      return found;
  return nullptr;
}
bool same(const BoxUiIntent& a, const BoxUiIntent& b) {
  return a.key.source == b.key.source && a.key.epoch == b.key.epoch &&
         a.key.blockId == b.key.blockId && a.key.bindingRevision == b.key.bindingRevision &&
         a.contextRevision == b.contextRevision && a.widgetId == b.widgetId &&
         a.bindingId == b.bindingId && a.value == b.value &&
         a.expectedValueRevision == b.expectedValueRevision && a.eventId == b.eventId;
}
} // namespace
void BoxUiSession::expect(DocumentToken next) {
  std::lock_guard<std::mutex> lock(mutex);
  if (next == token)
    return;
  token = next;
  enabled = false;
  models.clear();
  participants.clear();
  state.blocks.clear();
  ++state.epoch;
  ++state.bindingRevision;
  ++state.stateRevision;
  ledger.clear();
}
bool BoxUiSession::simulated() const {
  std::lock_guard<std::mutex> lock(mutex);
  return enabled;
}
bool BoxUiSession::viewport(double width) {
  std::lock_guard<std::mutex> lock(mutex);
  width = std::clamp(width, 320., 8192.);
  if (std::abs(state.viewportWidth - width) < 1.)
    return false;
  state.viewportWidth = width;
  return true;
}
void BoxUiSession::snapshots() {
  state.blocks.clear();
  for (auto& p : participants)
    state.blocks[p.first] = p.second.snapshot(*models.at(p.first));
}
void BoxUiSession::toggle(ParseResult model) {
  std::lock_guard<std::mutex> lock(mutex);
  if (!model || model->token != token)
    return;
  enabled = !enabled;
  ++state.epoch;
  ++state.bindingRevision;
  ++state.stateRevision;
  ledger.clear();
  models.clear();
  participants.clear();
  state.blocks.clear();
  if (enabled) {
    for (auto& b : model->blocks)
      if (b.boxUi) {
        models[b.boxUi->documentId] = b.boxUi;
        participants.emplace(b.boxUi->documentId, SyntheticActivity{});
      }
    snapshots();
  }
}
BoxUiState BoxUiSession::freeze(DocumentToken source) const {
  std::lock_guard<std::mutex> lock(mutex);
  if (source != token)
    return {};
  return state;
}
bool BoxUiSession::current(const BoxUiFrame& frame) const {
  std::lock_guard<std::mutex> lock(mutex);
  return frame.key.source == token && frame.key.epoch == state.epoch &&
         frame.key.bindingRevision == state.bindingRevision &&
         frame.key.stateRevision == state.stateRevision;
}
BoxUiCommandResult BoxUiSession::dispatch(const BoxUiIntent& intent) {
  std::lock_guard<std::mutex> lock(mutex);
  auto reject = [&](const std::string& message) {
    return BoxUiCommandResult{"rejected", message, intent.commandId};
  };
  auto known = ledger.find(intent.commandId);
  if (known != ledger.end())
    return same(known->second.intent, intent) ? known->second.result
                                              : reject("Conflicting command ID reuse");
  if (intent.commandId.empty() || intent.eventId.empty())
    return reject("Missing event/command identity");
  if (ledger.size() >= 4096)
    return reject("Command ledger exhausted; explicitly restart prototype");
  auto execute = [&]() -> BoxUiCommandResult {
    if (!enabled || intent.key.source != token || intent.key.epoch != state.epoch ||
        intent.key.bindingRevision != state.bindingRevision ||
        intent.key.stateRevision != state.stateRevision)
      return reject("Stale source, binding or state");
    auto model = models.find(intent.key.blockId);
    auto snap = state.blocks.find(intent.key.blockId);
    if (model == models.end() || snap == state.blocks.end() ||
        snap->second.contextRevision != intent.contextRevision)
      return reject("Stale block or context");
    auto* node = find(model->second->root, intent.widgetId);
    if (!node || !node->commandBinding || *node->commandBinding != intent.bindingId)
      return reject("Widget/binding mismatch");
    auto cmd = snap->second.commands.find(intent.bindingId);
    if (cmd == snap->second.commands.end() || !cmd->second.enabled)
      return reject("Command disabled or unbound");
    if (node->kind == BoxUiKind::Input) {
      auto* text = std::get_if<std::string>(&intent.value);
      if (!text || text->size() > 16384 ||
          std::count_if(text->begin(), text->end(),
                        [](unsigned char c) { return (c & 0xc0) != 0x80; }) > 4096)
        return reject("Input must be at most 4096 Unicode scalars");
      auto value = snap->second.values.find(node->valueBinding.value_or(""));
      if (value == snap->second.values.end() ||
          intent.expectedValueRevision != value->second.revision)
        return reject("Value revision conflict; Escape restores latest accepted value");
    } else if (node->kind != BoxUiKind::Button ||
               !std::holds_alternative<std::monostate>(intent.value))
      return reject("Invalid activation argument");
    auto error = participants.at(intent.key.blockId).execute(intent.bindingId, intent.value);
    if (!error.empty())
      return reject(error);
    ++state.stateRevision;
    snapshots();
    return {"accepted", "Accepted by local synthetic participant", intent.commandId};
  };
  auto result = execute();
  ledger.emplace(intent.commandId, Entry{intent, result});
  return result;
}
} // namespace xfmd
