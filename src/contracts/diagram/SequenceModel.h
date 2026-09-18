#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace xfmd {
struct SequenceParticipant {
  std::string id, label;
  bool actor = false;
};
enum class SequenceEventKind : std::uint32_t {
  Message,
  Reply,
  NoteLeft,
  NoteRight,
  NoteOver,
  Activate,
  Deactivate,
  Alt,
  Opt,
  Loop,
  Par,
  Else,
  And,
  End
};
struct SequenceEvent {
  SequenceEventKind kind;
  std::uint32_t first = 0, second = 0;
  std::string text;
};
struct SequenceModel {
  std::vector<SequenceParticipant> participants;
  std::vector<SequenceEvent> events;
};
} // namespace xfmd
