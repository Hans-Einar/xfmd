#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace xfmd {
// Versioned domain records, never a serialized dependency Graph or source code.
// Field schemas and validation live in contracts/diagram/rust/src/semantic.rs.
enum class DiagramFamily : std::uint32_t { State = 2, Class, Requirement, EntityRelationship };
enum class SemanticTag : std::uint32_t {
  Direction = 0,
  State = 10,
  StateTransition,
  StateRegion,
  ClassType = 20,
  ClassAttribute,
  ClassOperation,
  ClassAnnotation,
  ClassRelation,
  Requirement = 30,
  RequirementAttribute,
  RequirementRelation,
  Entity = 40,
  EntityAttribute,
  EntityRelation
};
struct SemanticRecord {
  SemanticTag tag;
  std::vector<std::string> fields;
};
struct SemanticDiagram {
  DiagramFamily family;
  std::vector<SemanticRecord> records;
};
} // namespace xfmd
