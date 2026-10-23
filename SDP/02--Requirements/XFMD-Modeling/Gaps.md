# XFMD SDL/SDUI modeling gaps

Owner request: record all discovered gaps, with SDUI extension proposals.
Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Baseline: [inventory and interpretation](Baseline.md).
Evidence: [XM-M2 probes](../../05--Implementation/XFMD-Modeling/evidence/XM-M2/README.md).

This is a maintained discovery register, not a selected producer implementation
plan. The observations below remain unresolved, with their current XFMD relevance
qualified by the owner disposition below. Observation, proposed extension and
acceptance are deliberately separate. IDs remain stable as evidence and dispositions evolve.
No producer files or backlog cards were changed in this assignment.

## Owner disposition — latest refinement, 2026-09-29/30

[MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
now selects complete SDL coverage and useful SDUI where feasible. Editor/viewer
areas may be labeled placeholders; working file loading/editing/saving, native
bindings and rich viewer parity are not required. This refines the preceding
no-SDUI direction without reinstating the broad widget wishlist as acceptance.

GAP-XFMD-SDUI-001–011 remain producer observations/proposals. Only the honesty and
validity of feasible compositions are checked here; those missing capabilities
are not XFMD defects or blockers for #014. SDL completeness, actual workflow
correspondence and independent review remain separate unfinished obligations.

Evidence baseline: XFMD `41fa494`; SDP `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`.
Producer paths below are relative to `/home/warloc/git/SDP-vNow`. A rejected
invented widget spelling proves it is outside the current closed widget profile;
it does not mandate that spelling for a future extension.

## SDL

### GAP-XFMD-SDL-001 — Explicit System and composed source sets

**Observed:** design-core 0.5 accepts `container XfmdDesktop.` but rejects
`system XFMD.` and `include "other.design".` with UNSUPPORTED_SYNTAX.
`SDL/go/cmd/sdl/main.go` takes one check input. The whole-system MVP1 corpus uses
a separate experimental profile. An untracked producer `sourceinput/` directory
is work in progress, not integrated/released support.

**XFMD impact:** the System-owned directory convention cannot yet establish one
validated graph across container, shared-contract and UI source files. Do not
misrepresent XFMD as a Unit synonym for System or create a custom concatenator
that silently changes source locations and duplicate-identity semantics.

**Proposal:** reuse producer KB-SDL-005, `SDP/KanBan/backlog/#005--SDL--Change--System-and-source-sets.md`.
Require an explicit System boundary and source membership, deterministic ordering,
cross-file resolution, duplicate diagnostics, original source spans and bounded
cycle/path handling. Probe released tooling before selecting it.

**Closure:** multi-file positive fixture plus duplicate, missing member and
cross-file error-location cases pass the supported profile. Until then, bounded
self-contained inputs may be used with a documented cross-model limitation.

### GAP-XFMD-SDL-002 — Requirement identities and normative narrative

**Observed:** a `requirement UR001.` declaration is rejected. Supported Actor,
UseCase, Feature and Functionality relations do not preserve the full normative
UR/SR/AT text, amendments or acceptance evidence. Core identifiers also cannot
directly use the existing hyphenated XFMD IDs.

**Impact:** mapping 37 objects cannot prove the requirement/acceptance inventory
is covered. Renaming UR-001 to a model symbol must preserve an explicit identity
map and does not create a new product requirement.

**Proposal:** coordinate with producer KB-SDL-001 (Requirements narrative).
Support stable external identity/source references and typed links to obligations,
acceptance scenarios and superseding amendments, with narrative kept authoritative.
Retain the local inventory/map until a supported contract exists.

**Closure:** one current, one amended, one future and one reserved XFMD identity
round-trip without collapsing requirement, implementation and evidence status.

### GAP-XFMD-SDL-003 — Detailed state, failure and source/evidence bindings

**Observed by profile inspection:** structural facts are unordered and Mode has
no activation predicate. Separate action/class/channel/scenario profiles provide
some behavior/contract facilities; they are not equivalent to XFMD C++ execution
or a complete binding of blueprint plumbing to real functions and tests.

**Impact:** cancellation, dirty-buffer admission, generation identity, lease
release and preference rollback need more than `owns`/`contributes-to` edges.
This is a composition/expressiveness question, not a claim that SDL has no behavior.

**Proposal:** first map each required invariant to existing profiles. Record only
the unsupported remainder as extensions: typed pre/postconditions, failure and
cancellation outcomes, and source/evidence references with candidate identity.
Consult producer KB-SDL-006 (executable channel tests and unit bindings).

**Closure:** the selected navigation workflow covers success, cancellation,
stale result, dirty rejection and resource release with an explicit evidence
mapping. The 2026-09-30 model now validates 22 declared Channel/scenario examples,
including NAV-01–12. This establishes example order/permissions/correlation, not
C++ bindings, guard predicates or exhaustive state/interleaving coverage. See
[Model-Semantics](../../04--Design/XFMD-Modeling/Model-Semantics.md).

## SDUI

The current validated widget vocabulary is **button, input and svg**, plus
frames/groups and Markdown. Symbolic callbacks and host-owned runtime state
already exist. The gaps below extend that foundation rather than replacing it.

### GAP-XFMD-SDUI-001 — Trees and dynamic collections

**Observed:** `tree(...)` and `list(...)` fail `widget-kind`; see the closed
schema in `SDUI/go/parser/validate.go`. XFMD uses lazy file/reference trees and
folders/files/favorites lists (`WorkspacePanel`, `SidebarWidget`, `IndexPanel`).

**Proposal:** define a collection projection with stable item IDs, selection,
expanded IDs, lazy-child state and bounded paging/virtualization. Host callbacks
receive an item identity and model generation, not a native pointer. Add grouped
lists with explicit nonselectable separators for favorites. Storage, sorting and
file operations remain application responsibilities.

**Closure:** collapsed/loading/error/stale/deleted-item cases, keyboard activation,
folder-first grouping and context target identity work without unintended opens.
Static Markdown rows are an illustrative fallback, not a working tree/list.

### GAP-XFMD-SDUI-002 — Tabs, splitters and pane state

**Observed:** `tabs(...)` and `splitter(...)` fail `widget-kind`. Frames and
relative sizes can illustrate a composition but not selected-tab semantics,
splitter dragging, retained proportions or focus restoration.

**Proposal:** tab groups with stable page IDs, selected state and activation
events; split panes with bounded proportions, min sizes, collapse and restore.
Visibility/availability must be distinct from removing component identity.

**Closure:** Files ↔ Index and Editor/Split/Preview transitions preserve document,
focus, filter, pane proportions and the intended visibility of Files controls.

### GAP-XFMD-SDUI-003 — Scrolling and source-linked viewports

**Observed:** `overflow-y=scroll` passes AST validation but SVG/layout rejects it
with `unsupported-scroll`. This is a layout/host capability gap, not missing
parser syntax. `SDUI/docs/go-layout-contract.md` explicitly requires a host.

**Proposal:** declare viewport capability and expose logical scroll state with
host ownership; define clipping, axes, keyboard/wheel behavior and extent updates.
Keep source-anchor synchronization in XFMD, via typed viewport intents/events.

**Closure:** long tree/editor/preview content can scroll without clipping away
needed content; resize and bidirectional synchronization avoid feedback loops.
Do not silently translate scroll to clip or infer support from AST success.

### GAP-XFMD-SDUI-004 — Menus, dialogs and focus ownership

**Observed:** `menu(...)` and `dialog(...)` fail `widget-kind`; a button's
`onContext` property fails `widget-property`. XFMD right-click menus and modal
dirty/Open/Preferences dialogs have focus, dismissal and cancellation behavior.

**Proposal:** host-owned transient surfaces with an anchor/target identity,
invocation source, modality, focus restoration and typed result/dismiss events.
Menu actions reference shared commands. Escape/cancel must be explicit outcomes.

**Closure:** dismissing a favorites context menu neither navigates nor mutates
history; keyboard and pointer invocation agree; a stale target cannot act after
reload. Modal cancellation preserves the current document and preference draft.

### GAP-XFMD-SDUI-005 — Full document editor and rich preview surface

**Observed:** `textarea(...)` fails `widget-kind`. Current `input` supports
draft/commit semantics, but does not declare XFMD's multiline editor, byte/source
selection, undo history, find or synchronized rendering surface.

**Latest owner scope:** labeled editor/preview regions suffice. A basic multiline
input with explicit load/save actions is a useful optional producer extension.
Fyne already has MultiLine Entry; the SDUI host currently wraps Entry but does not
expose a multiline property. See the source/API references in Model-Semantics.

**Earlier richer proposal, deferred:** an explicit document-editor or host-surface capability with buffer
revision, selection/caret, read-only state, composition/IME and edit intents;
separate preview capability exposes hit testing, anchors and selection. Reuse
XFMD's document/render contracts instead of embedding the application in SDUI.

**Closure:** Unicode, multiline edits, undo, dirty admission, source anchors and
stale render frames retain semantics. A multiline label is not an editor.

### GAP-XFMD-SDUI-006 — Value controls and transactional forms

**Observed:** `slider`, `checkbox` and `select` fail `widget-kind`. XFMD
Preferences and reading-color controls use sliders, check buttons, combo boxes
and numeric spinners with ranges. Current runtime already offers draft/revert
and atomic updates; it does not supply these missing control profiles.

**Proposal:** typed numeric/range, boolean and choice controls, with bounds,
steps, selected values, validation feedback and separate changing/committed
events. Form-level apply/cancel refers to the application's draft transaction.

**Closure:** live appearance changes can roll back; persistence failure restores
the saved profile; programmatic updates never become user actions; keyboard
and pointer adjustments have equivalent accepted values.

### GAP-XFMD-SDUI-007 — Icons, toggles, shared commands and accessibility

**Observed:** button `icon` and `checked` fail `widget-property`; current
formatting has enabled/visible but no declared icon/toggle command contract.
XFMD's Markdown pill, exclusive view modes and common menu/toolbar commands
need state beyond a text label.

**Proposal:** commands with stable IDs, enabled/checked state, shortcut and
accessible label; button bindings can specify icon resources, tooltip and a
presentation role such as pill. Use semantic tokens, not FOX constants or
unrestricted arbitrary style properties. Accessible name must survive icon-only UI.

**Closure:** the same command state appears in menu, keyboard and toolbar;
disabled/checked behavior, tab order and accessible labels have host evidence.

### GAP-XFMD-SDUI-008 — Interaction and binding completeness

**Observed:** a callback into a deliberately missing `.sdl` file passes the
frontend. The parser validates symbolic shape, not external resolution.
`SDUI/go/runtime/README.md` documents explicit host handler registration,
stale/duplicate rejection and atomic batches: these are existing protections.

**Producer study correction:** SDL/go/bridge already validates module/action
signatures and input/output handles before installing handlers. Extend that
existing phase, with no parser side effects or competing binding engine. Extend
event kinds/payloads and required host
capabilities; cover modifiers, context requests, focus, selection and viewport
events. Preserve current revision/handle revocation and programmatic-update rules.

**Closure:** missing target, signature mismatch and unsupported host capability
fail before activation; old callbacks after reload cannot mutate the new UI.
Passing a `.sdui` parser must never be reported as working XFMD C++ bindings.

### GAP-XFMD-SDUI-009 — Reuse across source files and profile negotiation

**Observed by source/profile inspection:** component references reuse definitions
inside one document. `ref:` is a symbolic external domain reference, not an
SDUI component import. Producer's SourcePilot explicitly avoids unsupported
cross-file SDUI imports. Parser, layout and host have different capability limits.

**Proposal:** explicit, versioned component membership/imports with stable
instance paths and cycle/duplicate diagnostics; separately declare required
parser/layout/host capabilities and a clear unsupported result. Share command
and theme definitions without duplicating them into every screen.

**Closure:** a shared toolbar used by two screens retains distinct instance
identity and common semantics; an unsupported viewport/widget is diagnosed before
host activation. Multi-file SDUI probes remain pending.

### GAP-XFMD-SDUI-010 — Markdown and visual-resource fidelity

**Observed by profile/source inspection:** `SDUI/docs/markdown-provider.md`
describes a bounded provider: images/HTML are rejected, inline styling/link
navigation is incomplete, Mermaid verification covers flowchart/graph, and the
Fyne CLI shows diagram placeholders without a registered renderer. The `svg`
widget is a symbolic placeholder, not automatic resource execution. XFMD's
image/math/typed-Mermaid and link/selection behavior is substantially broader.

**Proposal:** declare renderer/provider capabilities independently of the SDUI
grammar and host. Let a preview surface request the specific supported content
profile and resource resolver; reuse owned rendering interfaces where appropriate
without copying XFMD internals or silently adding network loading. Track this
boundary alongside #019, without selecting its implementation.

**Closure:** profile negotiation and explicit fallback distinguish real rendering
from a placeholder. Verify representative images, math, Mermaid families, links
and selection at the chosen host; parser success and one SVG export are insufficient.
These broader resource checks have not run in this baseline.

### GAP-XFMD-SDUI-011 — Typography and glyph fidelity across exports

**Observed in the navigation pilot:** the SVG export displayed a replacement box
for the Refresh arrow and broke some prose inside words. Nested Markdown lists
also appeared as flat bullet rows. The layout implementation uses embedded Go
Regular metrics (`SDUI/go/layout/font.go`); the exact glyph/export failure cause
has not been isolated. The pilot uses an explicit short text fallback for Refresh.

**Proposal:** define measurable text/glyph coverage and consistent measurement /
painting/fallback contracts per provider, including word wrapping and list depth.
Surface missing glyphs rather than counting successful SVG generation as fidelity.
The XFMD Pango/Fontconfig path must not be assumed equivalent to this exporter.

**Closure:** representative Unicode icons, long words, nested lists and font
fallback have inspected exports and host evidence at multiple sizes. This is a
producer follow-up, not permission to patch SDUI during XFMD modeling.

## Documentation, integration and evidence

| ID | Observation and effect | Disposition / closure |
| --- | --- | --- |
| GAP-XFMD-DOC-001 | Inventory finds 144 source files without literal blueprint filename mentions; all 281 still need semantic ownership reconciliation. | Review by responsibility, include recent amendments, and map actual code/test evidence; do not create an SDL object per file merely to satisfy counts. |
| GAP-XFMD-DOC-002 | Product manifest has release 0.0.0/default development values, while XFMD VERSION is 0.3; toolkit prose still names 0.2.0 and CONTRIBUTING names the former KanBan path. | Reconcile maintained process pointers and distinguish installed Toolkit, product release and build identities; preserve historical receipts. |
| GAP-XFMD-DOC-003 | Existing requirements/architecture/blueprints are partly Norwegian and contain dated amendments. | Translate maintained content while preserving stable IDs and decision meaning; never rewrite historical evidence. |
| GAP-XFMD-DOC-004 | No adopted machine-readable system-change ledger schema exists locally; Traceability currently delegates to requirement/blueprint/sprint records. | Use linked evidence documents under the existing contract. Do not invent a Traceability event schema as part of modeling. |
| GAP-XFMD-EVID-001 | AST/profile probes do not demonstrate a complete XFMD model or native interaction; independent review has not occurred. | VerificationPlan requires coverage, semantic/workflow checks and independent review before overall closeout. |
| GAP-XFMD-EVID-002 | Source tracing distinguishes Save-then-failed-open from Cancel: a successful save may clear dirty even when the next file read fails. Invalid generated URI validation also precedes cancellation of an active generation. Existing scenario coverage has not yet been established for both cases. | Review/add meaningful transaction tests before claiming complete NAV-05/NAV-10 evidence. Preserve actual semantics; do not label every failed open an exact state rollback. |

## Suggested producer sequence — recommendation only

1. Close System/source-set identity and diagnostics (existing KB-SDL-005), while
   keeping requirements/source bindings explicit.
2. Define SDUI capability/binding contracts, then trees/lists, tabs/splitters and
   viewports as a small XFMD-driven pilot.
3. Add commands/context surfaces and typed value controls; validate focus,
   cancellation and update semantics together rather than only drawing widgets.
4. Address document/preview host surfaces and cross-file reuse with actual
   consumers and independent host verification.

These are producer proposals. XFMD can author supported structural models and
labeled static compositions now; full fidelity remains qualified by this register.

## 2026-10-07 clarification — widget evidence and ownership

XM-M2 fixtures are intentional supported/unsupported capability probes, not a
pending XFMD widget implementation. They remain historical evidence; do not delete
or rewrite them to make project discovery appear valid. Current SDUI parser still
supports button/input/svg; upstream KB-SDUI-003 retains the unselected broader
capability/widget pilot. XFMD0.7's combined preview and local Fyne launch do not
close these producer language/runtime proposals. KB-XFMD-027 recorded a proposal to exclude evidence fixtures from ordinary
model discovery. Owner T005 explicitly rejects that change: XM-M2 may remain
in the normal model overview with genuine diagnostics. No filtering or deletion
is selected. The expanded producer widget inventory was applied to KB-SDUI-003 in
Session0004 T006 after write access was restored; implementation remains backlog.
