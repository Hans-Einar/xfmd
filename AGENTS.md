# Repository Guidelines

## Purpose and reading order

xfmd is a lightweight Markdown viewer/editor and a companion to `xfw` and `xfi`.
Read the [requirements](xfmd_requirements.md), [architecture](softwareArchitecture.md),
[working method](docs/working-method.md), [contribution guide](CONTRIBUTING.md),
[KanBan board](Agents/KanBan/README.md),
and affected [blueprints](src/blueprint/README.md) before making changes.
Distinguish dated design proposals from implemented code and current decisions.
Planned symbols are not existing implementations.

## Architecture and placement

- `src/application/`: FOX application, windows, commands, coordination and adapters.
- `src/interpreter/`: Markdown interpretation; no FOX or renderer dependencies.
- `src/renderer/`: presentation, layout, hit-testing and visual source mapping;
  no FOX or interpreter dependencies.
- `src/contracts/`: minimal FOX- and parser-free interfaces and data types.
- `src/blueprint/feature/` and `src/blueprint/functionality/`: design objects, not code.

FOX remains XFMD's toolkit. Application owns FOX integration; renderer describes
what to draw, and the FOX adapter draws it. Replacing an interpreter or renderer
should require an implementation and composition-root registration, not changes
to application workflows. SDUI's separate Fyne host does not change this boundary.

## Small files with clear responsibilities

Follow the architecture's file map. Give each header/implementation pair one main
role. `XfmdWindow` builds the window; it must not parse, save or calculate scrolling.
`DocumentSession` owns document state; dedicated coordinators handle preview,
navigation and scrolling. Avoid `Manager`, `Utils` and unrelated collections.
Above roughly 300 lines, consider splitting and document the reasoning; do not
split mechanically. New files need a documented owner and a natural place in the map.

## Conversation notes and KanBan

Use `Agents/KanBan/` for ideas, questions, investigations and follow-up notes from
chat that are not yet agreed product requirements or implementation plans.
Capture them during the discussion, with sources and a concrete next review.
Keep owner decisions, agent recommendations and open questions distinct.

After a discussion and before choosing the next round of work, review related
backlog notes for overlap and later corrections. Consolidate when useful, preserve
source cards under `superseded`, and record source/successor IDs and rationale in
the append-only ledger. Partial consolidation must leave unresolved scope visible.
Move only selected, bounded work to `active`; recording a note is not authorization
to implement it. Follow the board's rules for moves, links, outcomes and ledger replay.

This imports only the KanBan note workflow. Do not create an `SDP/` directory or
adopt the rest of the changing SDP process. Product truth stays in requirements,
blueprints, architecture, sprint plans and evidence, with links from the cards.

## Implementation workflow and traceability

Work in this order: **requirements → blueprint → contracts/plumbing → code → verification**.
Use the [templates](src/blueprint/templates/README.md). Keep blueprint chapter 5
named `Plumbing`, with concrete calls, source files, data and error paths. Mark
symbols as planned until they exist. Update blueprint and code in the same change.

Search for existing functionality before adding any. Features reuse functionality;
they do not call each other's private implementation. Expose only small, justified
contracts and identify the actual or planned consumer.

## Sprints, phases and builds

Follow the [working method](docs/working-method.md) and [versioning](docs/versioning.md).
Document new implementation deliveries under `sprints/`. Use one branch per phase,
one commit per milestone and one combined PR per sprint. Autonomous multi-phase
work builds at phase boundaries, not every commit. In collaboration with the user,
each fix may be built. Preserve merge history; do not squash/rebase published commits.
Capturing and reviewing KanBan notes does not itself require a product sprint.

## Style, checks and contributions

Write all new or revised documentation in English, including agent instructions,
KanBan cards and ledger reasons. Existing Norwegian documents remain authoritative
where applicable; translate them when revised without silently changing decisions
or evidence. Historical records retain their dated scope.
Code identifiers are English. Follow C++17 style: two spaces, `PascalCase` types/files,
`camelCase` methods and RAII ownership. Preserve established CMake/CTest checks.

Run `python3 tools/validate_blueprints.py` and `python3 tools/check_blueprint_symbols.py`
for design changes. Current build/test commands are in [README](README.md).
Use short imperative commit titles. PRs identify requirement/blueprint IDs,
changed plumbing, checks and known limitations; include screenshots for visible
GUI changes. Never mark anything `Verified` without documented test evidence.
