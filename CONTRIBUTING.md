# Contributing to XFMD

Start with [AGENTS.md](AGENTS.md), the [working method](docs/working-method.md),
[KanBan board](Agents/KanBan/README.md), [requirements](xfmd_requirements.md),
[architecture](softwareArchitecture.md) and affected [blueprints](src/blueprint/README.md).
The working method defines the process; this guide provides contributor entrypoints.

## Discussions and selected work

Capture unresolved ideas, investigations and chat follow-up in Agents/KanBan.
A note is not a requirement or implementation authorization. Review related notes
before selecting work; preserve later corrections and source/successor links when
consolidating. Move selected, bounded work to active and record transitions in the
append-only ledger. See the board for the exact card and ledger rules.

For implementation, follow **requirements → blueprint → contracts/plumbing → code
→ verification**. Search for existing functionality before adding code. Features
reuse documented services instead of each other's private implementation. Use
stable IDs and the blueprint templates. Chapter 5 stays Plumbing and describes
actual calls, symbols, source files, data and failures. Mark future symbols Planned.
Update code and affected call maps together; methods do not each need a design object.

## Architecture and code style

Application owns FOX, document workflows and adapters. Interpreter interprets
Markdown; renderer owns presentation/layout. Both use clean contracts. Register
replaceable implementations in the composition root. Keep one main role per
header/implementation pair; windows construct UI while coordinators handle work.
Do not parse or save in XfmdWindow. Consider splitting around 300 lines and explain
justified exceptions; avoid generic Manager/Utils files.

Use C++17, two spaces, PascalCase types/files, camelCase methods, RAII and the
existing .clang-format. Documentation and code identifiers are English. Preserve
dated evidence when translating or reconciling earlier documents.

## Builds and checks

Use the bootstrap/build/test commands in [README](README.md). CTest covers clean
contracts, document I/O, parsing, layout, coordination and FOX under isolated Xvfb.
Tests use `<Subject>Test.cpp`; fixtures belong in tests/fixtures and assets in
packaging. No percentage coverage threshold is defined. Record relevant AT/requirement
coverage and failure cases; use a separate sanitizer build for lifetime changes.

```sh
python3 tools/validate_blueprints.py
python3 tools/check_blueprint_symbols.py
python3 tools/check_layers.py
git diff --check
```

Structure checks do not replace semantic review of calls, ownership or behavior.
For SDL's real-tool GUI test, configure SDL_TOOL_EXECUTABLE explicitly; see the
[Sprint 004 test instructions](sprints/Sprint-004--SDL-Navigation/Phase-050--Document-Leases.md).
A default CTest run does not necessarily include that conditional test.

## Delivery and review

Follow the [sprint register](sprints/README.md) and [versioning](docs/versioning.md):
one `sprint/NNN/phase/NNN-topic` branch per phase, one commit per milestone and
one combined PR per sprint. Autonomous work builds at phase boundaries; collaborative
fixes may be built individually. Preserve published history with merge commits,
not squash/rebase. The older `phase/pN-...` convention is historical.

Use short imperative commit titles. PRs identify requirement/blueprint IDs, changed
plumbing, executed checks and limitations, with screenshots for visible GUI changes.
Verified requires actual documented evidence against an identified revision.
Merge and installation follow the owner's instruction, not an automatic phase step.
