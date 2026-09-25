# Working method: requirements, features and functionality

Status: current XFMD working method. This is a practical method for agent work,
using stable design objects and explicit change rules. Owner decision of
2026-09-25 introduces [SDP](../SDP/README.md) and the shared KanBan workflow.
KB-XFMD-017 owns the subsequent model/process adoption; existing product records
remain authoritative until reconciled.

The owner added a conversation-note workflow on 2026-09-24. Use the
[KanBan board](../SDP/Agents/KanBan/README.md) for unresolved discussions, findings and
follow-up ideas. Other projects can supply evidence, but do not become authorities
for XFMD requirements merely by being referenced. Write new/revised documentation
in English; preserve the meaning and provenance of older material when translating.

## 0. Conversation notes, sprints, phases and builds

Read the board at session start. Capture unresolved topics during discussion, then
review related backlog notes for overlap and later corrections before choosing the
next round of work. Selected work can use an existing card or a new synthesis;
preserve source cards, successor links and consolidation reasons in the append-only
ledger. Partial consolidation must retain unresolved scope. Follow the board's
rules for active scope, moves, closure and link updates.

Cards are working notes, not a second product specification. Recording or reviewing
one does not authorize its implementation. Once selected work becomes a product
change, follow requirements → blueprint → contracts/plumbing → code → verification
and link the resulting sprint plan from the card. Note capture itself needs no sprint.

A sprint groups an implementation delivery under `sprints/Sprint-NNN--Topic/`.
Its README identifies goals, requirements, phases and one combined PR. Each phase
has a Phase document with milestones, branch, tests and actual built version.
See the [register and template](../sprints/README.md).

- One branch per phase: `sprint/NNN/phase/NNN-topic`. Phase numbering continues
  after P41. Each phase starts from the previous phase branch within its sprint.
- One commit per milestone; update requirements, blueprint and code together.
  Record checks without claiming a phase build before one has actually run.
- Autonomous multi-phase work: one planned build at each phase boundary.
  Intermediate commits may use lightweight document/script checks. Correct and
  recheck failures; this boundary does not prohibit necessary corrective builds.
- Collaborative mode: when the user tries changes during development, each fix
  may be built. State this mode in the phase plan; it is not the autonomous default.
- A later milestone commit can record evidence without rebuilding unchanged code.
  Always identify the commit actually built; code changes require renewed checks.
- One PR per sprint, from the last phase branch to main. Use draft when useful;
  mark a finished sprint ready for review. No per-phase PRs. Preserve published
  history with merge commits, not squash/rebase. Merge and installation require
  the user's instruction; a phase build does not imply installation.

CI builds ready sprint PRs, main integration and explicit manual runs. Ordinary
phase pushes and draft PRs do not start program builds. This replaces the historical
per-phase PR practice. [Versioning](versioning.md) defines major/minor, branch/PR
and commit number. The owner has now authorized SDP adoption; see
[the migration handoff](../SDP/Agents/KanBan/backlog/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
Existing sprint/blueprint rules continue during the documented transition.

## 1. Design objects

A **feature** provides a coherent result with its own acceptance and lifecycle.
A **functionality** is a bounded service, mechanism or workflow with one owner
layer and an explicit public contract. A C++ method is an implementation detail,
not a reason for a separate document. A use case may use several features and
functionality directly; do not invent an empty feature to fill a hierarchy.

| Field | Values | Purpose |
| --- | --- | --- |
| Kind | Feature, Functionality | Coherent capability or bounded service. |
| Audience | User, System, Integration | Who observes or consumes the result. |
| Role (functionality) | Workflow, Service, Adapter, Mechanism | Orchestration, local service, technology connection or internal algorithm. |
| Owner | application, interpreter, renderer | One layer owns the contract and behavior. |
| Scope | FirstRelease, Future | Delivery scope, independent of design status. |

A system feature is valid when it has a coherent required capability and its own
acceptance. Do not use this to label every class a feature. Contracts have their
own files but are not functional owners.

## 2. Examples and boundaries

- Markdown presentation is a feature; interpretation, layout and FOX drawing are functionality.
- Live preview is a feature; debounce and document revisions are shared functionality.
- Links, back/forward and position restoration form the navigation feature.
- Synchronized scrolling is a feature; anchor conversion is reusable functionality.
- Load/save are document/storage services, not separate features.
- Splitter, view mode and sidebar are workspace functionality.
- Encoding/input validation is shared. A future lint feature needs concrete
  diagnostics and requirements; tolerant Markdown must not block saving through
  strict grammar validation.

The first baseline implemented four features and eleven functionality objects.
This historical count is not a target. Merge objects with the same responsibility;
split only for ownership, contract, reason to change or independent acceptance.

## 3. IDs, files and traceability

Use `UR-001`, `SR-001`, `FTR-001`, `FUNC-001` and `AT-001`. Reserve the next free
number in the register; never renumber existing objects. Example filenames:
`Feature-001--Markdown-Presentation.md`, `Functionality-001--Document-Session.md`.
Titles/slugs can change with updated links; an ID retains its meaning.

Traceability is **UC → UR/SR → FTR/FUNC → contracts/calls → AT → actual evidence**.
UR/SR can point directly to functionality. Every object has machine-readable
metadata and eight fixed chapters; chapter 5 remains `Plumbing`. Follow the
[templates](../src/blueprint/templates/README.md), including their structural labels.
The register maps requirements back to objects. Each functionality needs at least
one requirement. List cross-cutting requirements explicitly, not by hidden inheritance.

## 4. Required work before and during a change

1. Read requirements/architecture and search the register for existing owners/services.
2. Identify requirements, scope, error paths and observable acceptance; update
   requirements when introducing behavior.
3. Update affected blueprints before code; chapter 5 describes actual call direction.
4. Explain effects on data, revisions, ownership, errors and existing consumers.
5. Implement in the identified files; reconcile deviations in the same commit/PR.
6. Run relevant tests and validators; record commands, input, outcomes and commit.
7. Check that named symbols exist and documented calls actually happen.

A bounded user-authorized implementation needs no new permission at every step.
Do not use this method to stop routine work. Describe and clarify material changes
to product scope or contract meaning. Initial design was reviewed before
implementation; later changes retain the same traceability discipline.

## 5. Plumbing as a reuse map

Each row identifies the event/caller, public callee, source file, data/result and
failure/side effect. Mark `Planned` or `Implemented` explicitly. Feature tables
show end-to-end flow; functionality tables show public entry points and internal
service flow. Link the owner's blueprint instead of duplicating its contract.

For example, export uses `IInterpreter::parse`, not private PreviewCoordinator
methods. Justify reuse with a consumer and requirement. Put shared semantics under
the correct owner, remove duplication and update both call maps. Avoid generic
service buses, runtime plugin systems or Utils that hide dependencies.

## 6. Status and evidence

`Proposed → Ready → Implemented → Verified`; `Retired` means discontinued.

- Proposed: a design proposal; open questions remain explicit.
- Ready: requirements, contracts, error paths and test plan are settled, with gate evidence.
- Implemented: symbols exist; full acceptance is not necessarily verified.
- Verified: every relevant AT has evidence tied to an identified commit/environment.

Status and delivery scope are separate. Future requirements may be deferred in the
register without speculative stubs. New behavior returns the affected object to
Proposed/Ready; old evidence remains history, not evidence for new requirements.
Test failures block Verified, not necessarily unrelated work.

## 7. Minimum checks and improvement

`python3 tools/validate_blueprints.py` checks metadata, IDs, local links, chapters,
requirement coverage, dependencies and planned/implemented plumbing markers.
It checks structure, not code correctness, requirement quality or actual calls.
Semantic review and relevant tests remain necessary.

Local Markdown link checks cover XFMD-owned documents, including SDP/Agents/KanBan.
They skip dependency documentation under `.deps` and `third_party`, along with
Git/build/cache directories. Links originating in project documents remain checked,
including links to dependencies. Blueprint metadata and plumbing checks are unchanged.
CI runs Rust formatting checks on the four XFMD workspace packages explicitly;
`cargo fmt --all` also traverses the pinned path dependency despite workspace
exclusion. Keep that package list in sync when adding an XFMD Rust package.

`python3 tools/check_blueprint_symbols.py` checks that documented Implemented
callee names appear in the specified files. It reads text, not an AST, and does
not prove the caller invokes the callee. The historical P7 check covered 83 calls;
manual review found and corrected stale caller names. Use current output for
current counts. KanBan consistency checks are specified in its own README.

Further improvement after the initial implementation:

- Measure plumbing drift before introducing an AST-based symbol checker.
- Add structured contract fields only when they resolve a concrete ambiguity.
- Keep short decision records for real choices, not a document for every small choice.
- Consider schema generation after the concepts have been used in practice.

Minimize duplicated functionality and documentation, not necessary services. The
method should let the next agent find the right path without guessing or reinventing it.

## Historical revision 1.1

The [design revision](../softwareDesign.md) extended P0–P8 with proposed future
objects and changed contracts. Existing objects retained their original FirstRelease
scope while returning to Proposed for revised contracts. Implemented plumbing and
older evidence applied to the baseline; new Planned rows and acceptance needed
separate verification. P9–P13 described the next delivery; Future did not mean
indefinite postponement. See the dated design for the original inventory.

## Historical revision 1.2: implementation completed

P9–P13 used phase branches, milestone commits and individual PRs as then planned.
The register moved delivered objects to Implemented/FirstRelease; Verified was
not used as a synonym for implemented code. Tests/evidence covered mechanisms,
native FOX, isolated Window Maker, PDF reading/raster and failures. Physical
touchpad and monitor trials remained explicitly bounded. These paragraphs record
history, not the current sprint-PR workflow or latest product baseline.
