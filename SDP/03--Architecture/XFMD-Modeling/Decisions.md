# XFMD model architecture decisions

Parent: [PLAN-XFMD-0002](Plan.md). Date: 2026-09-29.
These are implementation choices within the authorized modeling assignment,
not new XFMD behavior or approval of a producer language extension.

## XM-ADR-001 — Runtime boundaries and source responsibilities

XFMD is the product System. The current interactive runtime is **XfmdDesktop**,
the FOX application process. Windows, navigation panes, workers and CMake static
libraries do not each become Containers. The command-line addressed-delivery
helper is another invocation of the XFMD executable; model its protocol role
explicitly when expanding IPC scenarios, rather than inventing a daemon.

Application owns document/workspace coordination, asynchronous work and FOX
adapters. Interpreter owns Markdown/diagram interpretation. Renderer owns layout
and hit testing. Contracts owns shared value/port definitions; it is neither a
runtime nor a service that executes those ports. The Rust composition crate is
link-time integration, not another process. Fonts/Cairo/librsvg are host adapters.

SDL view generation and the optional document broker are external processes.
Browser/OS file opening is an explicit external handoff. SDL/SDUI/SDPTool remain
separate systems. Fyne is not part of XFMD's current application runtime.

Evidence: `CMakeLists.txt`, `softwareArchitecture.md`, `Application.cpp`,
`ApplicationOpening.cpp`, `navigation/DocumentViews.cpp`, and the shared headers.
The source inventory's classification records responsibility, not a claim that
every call has received a semantic review.

## XM-ADR-002 — Supported pilot before whole-system composition

| Alternative | Benefit | Limitation / disposition |
| --- | --- | --- |
| Wait for all producer extensions | One future source-set/profile contract | Unnecessarily blocks describing existing supported responsibilities; not selected for the pilot |
| Author the full experimental MVP1 dialect | Richer System/state/source structure | Cannot claim supported parser validation; not selected as the validated baseline |
| Supported bounded inputs plus explicit mappings/gaps | Immediately executable checks and generated views | Cannot represent a complete linked System graph; selected for the first pilot only |

Use design-core 0.5 for one **bounded navigation-workflow structural input**.
The System boundary is documented in the System README; do not substitute `unit`
for an unsupported `system` declaration. Declare the real desktop Container and
the logical responsibilities participating in the pilot. The model is a candidate
representation of existing requirements/blueprints, which remain normative.

Do not duplicate these definitions into additional files to simulate imports.
Keep subsequent domain details in their owning source locations once their input
composition is supported or explicitly decided. No XFMD-specific concatenator is
introduced. GAP-XFMD-SDL-001 remains open; the pilot cannot close whole-model scope.

## XM-ADR-003 — Identity and fidelity

Use readable ASCII model symbols and maintain an explicit map to existing
hyphenated IDs; symbols do not allocate replacement FTR/FUNC/UR identities.
Each symbol has one declared owner. The model asserts only facts it can express:
ownership, allocation and contributions. Call order, admission, cancellation and
failure semantics remain in the linked workflow contract until encoded in an
appropriate supported behavioral profile. Structural edges are not call arrows.

SDUI 0.2 supplies **static state compositions** for this first pilot. Existing
buttons/inputs/frames can be used; missing trees, tabs, menus and document surfaces
are explicitly labeled representations. Named entry frames show states separately;
no fake tab switching, right-click or C++ callbacks are claimed. Do not put an
unresolved symbolic callback in the model merely to make it look wired.

## XM-ADR-004 — Reproducible, isolated tool execution

Use the pinned producer archive and binary hashes from XM-M2. Generate AST/dump,
SVG and SDL documentation through the real tools into the implementation phase's
derived-view directory, outside authored sources. Record source and output hashes.
Do not register the partial pilot as a complete System in SDPTool or install tools.
The existing native sidebar and production renderer remain unchanged.

## XM-ADR-005 — SDL for maintainable extension; no required XFMD SDUI model

Authority: the owner's later 2026-09-29 conversation, recorded in
[MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29).
This supersedes XM-ADR-003's SDUI work as an ongoing assignment; the delivered
experiment, claims and evidence retain their historical scope.

SDUI is an aid to explore user needs and exercise SDL before native implementation.
Reconstructing XFMD's existing screens would not serve the selected outcome.
Neither descriptive UI coverage nor live SDUI bindings are required. Missing
SDUI widgets and fidelity limits do not block the XFMD SDL baseline or #014.

Use SDL to expose functional ownership, collaboration, contract/dependency
direction and state/resource lifetimes. Before extending native behavior, map
the affected requirement and functionality to its owner and reused contracts;
describe any new boundary/dependency and its rationale before implementing it.
Keep existing domain responsibilities authoritative instead of duplicating them
inside new navigation/diagram presentation code. Review actual calls and failure
paths against the model/blueprints; parser validity is not conformance evidence.

No additional runtime, framework, parser or generalized extension architecture
is selected by this decision. New abstractions need concrete feature needs.
The next model work remains NAV-01–12 behavior/contract mapping and semantic
coverage of remaining existing responsibilities. #014/#015 keep their separate
feature scope, and the supported-language limitations remain visible.

## Remaining architecture boundary

The pilot decision is sufficient to begin DES-M1/IMP-M1 for this bounded input.
Whole-system source composition, complete behavior/profile mapping and readiness
for #014 are unresolved. Independent review and owner gap disposition remain
required before overall completion, not before every reversible model draft.

## XM-ADR-006 — Full structural inventory, declared scenarios and feasible SDUI

Latest owner refinement, 2026-09-29; executed 2026-09-30. This supersedes
XM-ADR-005's exclusion of further SDUI description. Complete SDL modeling remains
the goal; useful SDUI composition is selected, with labeled editor/preview regions
and no requirement for working file editing or full native fidelity.

XfmdDesktop.design is the current canonical input, replacing the bounded pilot
as the maintained model. Retain Navigation.design unchanged only as an XM-M3
reproduction fixture. One input groups actual desktop responsibilities until the
pinned producer supports linked source sets; no custom import/concatenation layer
or duplicate canonical definitions is introduced. This is an explicit temporary
source-layout compromise, not proof that formal System composition is supported.

The 37 existing blueprints and 10 current use cases have structural mappings;
22 declared Channel scenarios describe navigation and other principal workflows.
InteractionObservationShape is a design annotation, not a runtime protocol or
invented C++ payload. Review its limits and actual source contracts in
[Model-Semantics](../../04--Design/XFMD-Modeling/Model-Semantics.md). Three state
records are bounded excerpts, not complete data schemas. Structural coverage is
complete at the blueprint inventory level; semantic/evidence closure remains open.

Five SDUI concepts use supported controls and labeled areas. A basic Fyne-backed
multiline input is an optional producer follow-up; no language/host changes,
product code or feature #014/#015 work are selected here.
