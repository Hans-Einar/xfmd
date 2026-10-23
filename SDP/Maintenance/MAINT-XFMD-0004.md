# MAINT-XFMD-0004 — Model XFMD responsibilities and behavior in SDL

| Field | Value |
| --- | --- |
| id | MAINT-XFMD-0004 |
| project | XFMD |
| state | active |
| PlanType | MaintenancePlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| source | Owner conversation 2026-09-28; KB-XFMD-017 |

## Outcome and authorization

Establish a traceable SDL description of XFMD functionality, responsibility,
contracts and behavior, with reproducible views and explicit language limitations.
Use it to assess architectural impact before extending the application, preventing
uncontrolled growth and preserving maintainability. Make the adopted SDP usable
for subsequent XFMD work, starting with the native SDP sidebar in
[KB-XFMD-014](../KanBan/active/%23014--Proposal--SDP-sidebar-and-generated-navigation.md).

This is the overarching MaintenancePlan for
[KB-XFMD-017](../KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
The owner authorized execution on 2026-09-29 and explicitly requested a durable
register of all discovered gaps, including SDL profile limits and SDUI proposals.
Current execution covers XM-M1 reconciliation, XM-M2 architecture and a bounded
XM-M3 navigation pilot. Product behavior and producer repositories remain unchanged.
KB-XFMD-017 is active; final modeling coverage and review are still outstanding.

The parent owns scope, sequencing, shared decisions and completion. The five
subordinate plans will own their detailed assignments and evidence. They are
linked work records, not five copies of this plan or five new product phases.

## Owner scope clarification — 2026-09-29

The latest owner instruction selects a **complete SDL model of existing XFMD**
and **SDUI descriptions wherever current facilities are useful**. This refines
both earlier positions: a full native UI reproduction is unnecessary, but SDUI
is again part of the useful modeling deliverable. Earlier no-SDUI instructions
and the later broad widget wishlist are not the current acceptance boundary.

SDUI primarily supports early requirements exploration and exercising SDL before
native implementation. For XFMD it may illustrate composition, user intents and
representative states. The editor and Markdown preview may be clearly labeled
areas; loading, editing, saving and rich rendering need not work in the prototype.
A basic multiline text-input capability is a useful producer proposal, not a
condition for finishing this model. No Fyne port or native XFMD change is selected.

Complete SDL coverage means accounting for every current feature/functionality,
use case, responsible source unit, shared contract and significant workflow/state/
failure boundary. Preserve future/reserved identities separately. Structural
coverage, declared scenarios, actual code/test correspondence and independent
review are different claims. Unsupported formal System/source composition and
behavior semantics remain explicit gaps, not silently invented syntax or evidence.

Use this baseline to assess architectural impact before native SDP navigation
(#014) and SDL diagram preview (#015) expand the application. For each future
feature, name the responsibility owner, reused contracts, new dependencies,
state/resource owner, failure behavior and affected tests. Record material design
changes before dependent implementation and review code against those boundaries.
SDL tooling does not automatically prevent architectural drift.

Preserve the first SDUI experiment/evidence. Add feasible conceptual compositions
using supported syntax and label inactive controls and document surfaces clearly.
Missing advanced widgets, C++ bindings or viewer/editor parity do not block #017
or #014. Producer implementation remains separately owned and selected.

## Inspected baseline — 2026-09-28

- XFMD checkout: `sprint/009/phase/057-favorites`, HEAD `41fa494`.
  Current product sources include generated-document navigation, the revised
  workspace/sidebar and favorites with context-menu management. Existing
  requirements, architecture, blueprints and sprint evidence remain authorities
  until each concern has an explicitly reconciled successor.
- The working tree already contains process installation changes, KB-XFMD-019
  and the SDL source instructions. These are concurrent work, not this plan's
  implementation. [MAINT-XFMD-0003](MAINT-XFMD-0003.md) and the
  [installed manifest](../Framework/installed-toolkit.manifest.yaml) record
  signed Toolkit **1.0.0**, management profile `sdp-project-management/0.2`.
  Some local prose still names 0.2.0 or the former board path. Installation is
  not evidence that XFMD models exist or that all local instructions agree.
- The project manifest still has default product release/development values.
  [XFMD versioning](../../docs/versioning.md) and actual release/build evidence
  must be reconciled before mapping these fields; Toolkit and XFMD versions are
  different identities.
- Producer reference: `/home/warloc/git/SDP-vNow`, inspected HEAD
  `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`. Recent work includes signed Go
  installation, System-owned source organization, an MVP1 SDUI/navigation pilot,
  and request/routine governance studies. This is reference evidence, not an
  instruction to copy the producer's evolving workflow into XFMD.
- Producer `PLAN-SDP-0007` is now completed. Its StudyPlan uses the existing
  `RequirementPlan` enum; StudyPlan is its purpose, not a seventh installed type.
  Its synthesis recommends future governance/MCP/client work and reports that
  integrated enforcement is not implemented. This assignment can use explicit
  document checkpoints without waiting for that implementation.
- The producer documents supported **SDL design-core 0.5** and **SDUI 0.2**.
  Its whole-system MVP1 corpus is a separate experimental profile, not proof
  that the released parser accepts System declarations or that complete
  multi-file composition works. These are inspection findings, not fresh tool
  validation. Pin and probe actual tool inputs before authoring XFMD models.

## Scope and definition of coverage

The baseline is the existing XFMD application plus accepted requirements and
known discrepancies. Inventory all current functions; do not limit the model to
the sidebar. Keep implemented behavior, required behavior, planned extensions
and historical alternatives distinguishable. Reconstruct missing rationale from
branches, commits and evidence without treating implementation as owner approval.

The first deliverable is a coverage/authority map. For each concern record its
stable UR/SR/UC/AT/FEAT/FUNC or existing equivalent IDs, governing source, code
owner, SDL definition or explicit language limitation, feasible SDUI representation, verification reference,
status and unresolved gap. Multiple concerns may share a model; this is not a
requirement to produce one file or SDL object per method.

| Area to inventory | Required model coverage |
| --- | --- |
| Purpose and requirements | Actors, use cases, features, constraints and acceptance; retain requirement meaning and identities |
| Architecture | Actual runtime containers, source layers, external tools, ports/contracts, ownership and persistent state |
| Documents and navigation | Opening/root policy, editing/save/dirty state, history, links, generated views, window/pane targeting and leases |
| Rendering and output | Markdown, layout/source mapping, images/math, Mermaid, themes/zoom and print/PDF flows, including supported limits |
| User interaction behavior | User intents and observable outcomes for document/workspace controls and preferences; state/admission/failure ownership, with feasible SDUI composition and labeled editor/preview areas, without native behavior parity |
| Cross-cutting behavior | Configuration, resource lifetime, asynchronous work/cancellation, stale results, errors and build/install identity |

This is the starting checklist, not an exhaustive inventory claim. The
RequirementPlan must reconcile it against the entire blueprint/source inventory.
Each row must be modeled or have a specific, reviewed gap disposition. An
unsupported fact may remain explicit in Markdown while language support is
resolved; it must not disappear from coverage or count as validated SDL.

Authored models belong under **`SDP/SDL/XFMD/`**, following the installed
[source guide](../SDL/README.md) and [authoring instructions](../SDL/AGENTS.md).
Use actual container ownership and put SDUI under `SDUI/<UIContainer>/`.
The five numbered process phases retain plans, rationale and derived views.
Source directories neither create language syntax nor make libraries containers.

SDL describes functional responsibility and collaboration; existing native UI
code remains the implementation. SDUI prototypes illustrate composition where feasible; they are not native UI
specifications or a dependency on working editor/preview widgets.
FOX remains the toolkit. No Fyne migration, BoxUI revival or executable C++
binding is implied by this modeling work.

## Supporting plans

The owner authorized these work packages on 2026-09-29. Each has a substantive
plan and a registered lifecycle. All five plans are active. Detailed design and implementation currently cover
only the bounded navigation pilot; full coverage and independent review remain open.

| PlanType | Linked plan | Responsibility and exit condition |
| --- | --- | --- |
| RequirementPlan | [PLAN-XFMD-0001](../02--Requirements/XFMD-Modeling/Plan.md) | Establish the complete inventory, source authority, actors/use cases, requirement mapping and measurable coverage criteria; resolve current versus intended behavior |
| ArchitecturePlan | [PLAN-XFMD-0002](../03--Architecture/XFMD-Modeling/Plan.md) | Select System/container boundaries, external collaborations, source ownership and supported language/input profiles; record alternatives and unsupported constructs |
| DesignPlan | [PLAN-XFMD-0003](../04--Design/XFMD-Modeling/Plan.md) | Specify SDL responsibilities, contracts, collaboration and traceability; select representative behavior/failure workflows and a reproducible generation design |
| ImplementationPlan | [PLAN-XFMD-0004](../05--Implementation/XFMD-Modeling/Plan.md) | Author and integrate the model sources, generated views, registration and checks in bounded milestones; reconcile maintained documentation and instructions |
| VerificationPlan | [PLAN-XFMD-0005](../05--Implementation/XFMD-Modeling/VerificationPlan.md) | Define evidence early; verify complete coverage, semantics, model validation, generation/navigation and consistency against the identified candidate |

Verification is a distinct plan, not a new sixth numbered process directory.
The ImplementationPlan implements this modeling/adoption assignment; it does not
authorize the features in KB-XFMD-014 or a rewrite of XFMD. Detailed plans may be
refined together when dependencies are understood; the table is not a requirement
to finish every document before testing a small model.

## Phases, milestones and checkpoints

XM-M1 and the preparatory XM-M2 probes are active. Later milestones remain
planned; the evidence below does not establish full semantic/model coverage.

| Phase | Milestone | Observable acceptance before proceeding |
| --- | --- | --- |
| P0 — Establish baseline | XM-M1 | RequirementPlan and initial VerificationPlan exist; inventory reconciles code/docs/history, records actual tool versions and known gaps, and defines the coverage denominator |
| P1 — Select architecture | XM-M2 | ArchitecturePlan resolves runtime/source ownership, dependency direction and the SDL profile/input strategy; parser/profile evidence and explicit limitations support the selected model |
| P2 — Prove model design | XM-M3 | DesignPlan traces requirement → responsible functionality/contract → SDL representation or explicit limitation → actual source/test evidence for a representative existing workflow; generated views expose ownership and collaboration |
| P3 — Complete coverage | XM-M4 | ImplementationPlan extends the proven pattern across the inventory in bounded milestones; shared facts have one owner, stale maintained docs are reconciled, gaps remain visible |
| P4 — Verify and hand off | XM-M5 | VerificationPlan executes on the integrated candidate, independent review findings are resolved or explicitly dispositioned, and the owner receives the coverage/gap report plus the separate #014 planning handoff |

Suggested XM-M3 workflow: open a document through Files or favorites, edit it,
then request a generated navigation view. Cover dirty-buffer admission, pending
work/failure and pane replacement as well as the happy path. Final selection
belongs to the DesignPlan; the pilot must explain collaboration and behavior,
not merely inventory symbols. Feasible SDUI states supplement this contract; active file editing is not required.

Do not expand across the whole model until the representative input and
generation approach works. If a profile cannot represent a required concept,
record the precise missing capability and choose a bounded fallback or a producer
dependency. Experimental syntax needs an explicit design decision and separate
validation claims; copying the MVP1 corpus does not solve parser compatibility.

## Keeping execution aligned with the plan

At selection of each milestone, record its governing requirement/design IDs,
inputs, allowed changes, dependencies, acceptance checks and actual Git/tool
baseline in the responsible plan. Update the parent with the current milestone,
child plan links and unresolved decisions; keep detailed evidence in its owner.

Before closing a milestone, compare the result to that assignment and record
checks, candidate identity, limitations and review disposition. A checkbox,
successful parser run or completed KanBan card is not proof of feature fidelity.
Recheck affected evidence when the candidate changes. If scope or architecture
changes, revise the governing plan before continuing dependent work.

Every handoff must state completed work, remaining work, blockers and the next
permitted step. Record lifecycle transitions in the existing shared management
ledger; use the adopted Traceability contract for system-change/evidence records.
Do not introduce a parallel routine ledger or claim automated enforcement.
Independent review requires actual independent context and authorized execution;
an author's checks must not be labeled independent approval.

## Dependencies, boundaries and completion

- #017 is the selected modeling prerequisite for the owner's next #014 work.
  Closeout must provide useful XFMD model/preview fixtures and identify remaining
  producer navigation/manifest dependencies. It does not itself deliver the SDP tab.
- Direct `.design` preview (#015), widget ownership (#003), and shared rendering /
  Kitty output (#019) remain separate. Model them as planned where relevant,
  not as existing XFMD functionality or automatically selected implementation.
- Changes to SDL, SDUI, SDPTool, the MCP/client governance work, or another
  repository need separately selected work. Record producer gaps here with links;
  do not silently install a development Toolkit or change a language profile.
- Preserve existing IDs, dated evidence and append-only history. Reconcile and
  translate maintained documentation into English without rewriting immutable
  records. Generated views must be regenerated from their authored source.
- Completion requires coverage of the whole agreed inventory, reproducible
  parsing/rendering/generation within the selected profiles, source provenance,
  consistent current documentation and resolved or owner-dispositioned gaps.
  Feasible SDUI composition uses labeled placeholders where needed; live editing,
  full rendering and native bindings are not required. If language
  limitations leave facts outside SDL, report **qualified
  coverage**, not “fully modeled.” Owner disposition must explicitly decide
  whether that baseline is sufficient to proceed with #014.

## Git policy

Execute on the current branch without moving unrelated work. The owner selected
this plan on 2026-09-29. Document/model milestones use milestone
commits under this parent; each child must state its own BranchPolicy and
CommitPolicy before execution, including any selected phase branches under the
existing XFMD convention. This does not reopen Sprint 009 or rewrite its history.
Merge, push, release and installation are outside this planning assignment.

## Review points and current evidence

For joint review: confirm the coverage boundary, the five subordinate plan
responsibilities, and how language gaps affect readiness for #014. In particular,
decide whether a documented partial-language baseline is acceptable or whether
specific producer extensions must be delivered first. No such exception is
assumed by this draft.

This draft is based on repository/document inspection, not a complete source
audit, model execution, product test run or independent plan approval.

Draft checks, 2026-09-28/29 (local midnight boundary):

- `python3 SDP/KanBan/verify_board.py`: PASS — 19 cards, 79 card events and
  four management records; schemas, replay, placement, state and index agree.
- `git diff --check`: PASS.
- Targeted link inspection: 14 relative links in the new plan, its card and the
  Maintenance index, plus seven producer source references, resolve.
- The pre-existing 53,522 ledger bytes are unchanged. Draft registration appends
  EVT-PM-XFMD-000007 and EVT-KB-XFMD-000079; the board-index edit only updates
  #017's displayed title. Existing concurrent work is preserved.

These checks validate planning records, not the future model's completeness.

Producer references at the inspected commit:

- `SDP/02--Requirements/RoutineGovernance/StudyPlan.md` and `Synthesis.md` —
  study outcome and proposed process controls.
- `SDL/README.md`, `SDL/go/README.md`, `SDUI/README.md` — supported profiles and
  tool boundaries to probe before execution.
- `experiments/mvp1_sdl/README.md` and
  `SDP/04--Design/MVP1/SourcePilot/Plan.md` — recent source/UI pilot and its
  experimental-model limitations.

## Execution checkpoint — 2026-09-29

Owner authorization covers implementation of this plan and recording all SDL/SDUI
modeling gaps with extension suggestions. Initial checkpoint deliverables (superseded in scope by the pilot checkpoint below):

- [Baseline and authority map](../02--Requirements/XFMD-Modeling/Baseline.md).
- [Reproducible inventory](../02--Requirements/XFMD-Modeling/Inventory.md):
  37 blueprints, 156 referenced requirement identities and 281 source files.
  Semantic ownership and amendment/evidence review remain open, including 144
  files without literal blueprint filename mentions. These are review candidates.
- [Gap register](../02--Requirements/XFMD-Modeling/Gaps.md): 18 open findings
  covering SDL, SDUI, local documentation and evidence; proposals are not producer
  implementation selections.
- [Capability probe evidence](../05--Implementation/XFMD-Modeling/evidence/XM-M2/README.md):
  21 final positive/negative expectations pass on pinned tools. The initial harness
  exit-code assumption and its correction are retained in evidence.

XM-M1 is not yet closed: semantic inventory reconciliation remains. XM-M2 has
capability evidence, but the profile/composition decision and architecture handoff
remain open. At that initial checkpoint no model source or generated view had been authored.
The pilot checkpoint below records subsequent work; overall completion remains open.

## Pilot checkpoint — 2026-09-29, subsequent continuation

[Coverage](../02--Requirements/XFMD-Modeling/Coverage.md) now assigns all 71
UR/SR definitions to responsibilities and reconciles key amendments/source families.
[Architecture decisions](../03--Architecture/XFMD-Modeling/Decisions.md) authorize
supported bounded model inputs; the full System/source-set question stays open.
The [workflow contract](../04--Design/XFMD-Modeling/Navigation-Workflow.md) and
[model sources](../SDL/XFMD/README.md) deliver the first partial SDL/SDUI pilot.

The pilot maps 17 of 37 blueprint identities structurally, generates 52 reproducible
artifacts and supplies three visually inspected static shell states. It does not
encode all ordered behavior or provide live native widgets/bindings. The gap
register now has 20 entries. See [evidence](../05--Implementation/XFMD-Modeling/evidence/XM-M3/README.md).

This bounded prototype may proceed alongside semantic/evidence reconciliation;
it does not waive the whole-inventory or independent-review gates. Next work is
behavioral-profile mapping for NAV-01–12, review of the two newly exposed evidence
cases, and expansion to remaining responsibilities with explicit fidelity gaps.
Git metadata is read-only in this session, so the changes are not committed.

## Scope revision checkpoint — 2026-09-29

The later owner clarification above governs all current acceptance criteria.
All five child plans and KB-XFMD-017 remain active; no model milestone is newly
claimed complete. SDUI evidence is retained, while future work focuses on SDL
behavior/contract mapping and source responsibility review. Local Git write access
was restored after the earlier read-only checkpoint; this scope revision remains
uncommitted. Prior candidate manifests identify historical bytes, not these
revised plan documents. No authored model, generated output or product source
was changed by this revision.

Management history: EVT-PM-XFMD-000021, EVT-PM-XFMD-000022, EVT-PM-XFMD-000023, EVT-PM-XFMD-000024, EVT-PM-XFMD-000025, EVT-PM-XFMD-000026.

Scope-revision checks: board replay passes (19 cards, 83 card events, nine
management records); blueprint validation passes (37 blueprints, 71 requirements),
as does the symbol-presence check (285 callees; no caller-semantics claim).
All 155 local file links in the revised documents resolve. Whitespace checks pass,
the previous ledger prefix is unchanged, and 128 model/evidence/generated files
retain their pre-revision hashes. These are same-context documentation checks,
not independent review, new model coverage or native application test evidence.

## Current model checkpoint — 2026-09-30

The latest owner refinement above governs this delivery: a complete SDL model is
the goal, with SDUI where feasible and labeled editor/preview regions accepted.
[XfmdDesktop.design](../SDL/XFMD/XfmdDesktop.design) replaces the first pilot as
the current canonical input. It covers all 37 existing blueprint identities and
10 current use cases structurally, plus 22 declared example scenarios. Five SDUI
concepts illustrate composition without working document actions. The 243 output
hashes reproduce in a clean temporary directory; see
[XM-M4 evidence](../05--Implementation/XFMD-Modeling/evidence/XM-M4/README.md).

XM-M3's ordered-example mapping and XM-M4's structural inventory expansion are
now delivered within design-core 0.5. They do not close full semantic coverage,
formal System composition, C++ behavior evidence or independent review. The next
bounded task is to reconcile remaining normative/source/test semantics, including
NAV-05/NAV-10 regression evidence. No native product or producer source changed.
All plans and #017 remain active. Work is uncommitted; concurrent #020 and earlier
installation/backlog changes are preserved. Prior candidate hashes stay historical.

Management events: EVT-PM-XFMD-000027, EVT-PM-XFMD-000028, EVT-PM-XFMD-000029, EVT-PM-XFMD-000030, EVT-PM-XFMD-000031, EVT-PM-XFMD-000032, EVT-KB-XFMD-000085.

## 2026-10-01 — Session-led native integration delta

[SN-001](../05--Implementation/SDP-Navigation/evidence/SN-001/README.md) records the FUNC-032/UC-012 structural delta and sixth SDUI
concept alongside native implementation under PLAN-XFMD-0006. Current generated
views/inventory are regenerated; historical XM-M2/XM-M4 evidence remains intact.
This does not close this plan's earlier semantic, native-evidence or independent
review obligations. Continue with SESSION-XFMD-0001 for the new feature.
