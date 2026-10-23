# XFMD modeling baseline — 2026-09-29

Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Requirements: [PLAN-XFMD-0001](Plan.md).
Discovery record: [Gaps](Gaps.md). Reproducible inventory: [Inventory](Inventory.md).

## Authority and coverage denominator

XFMD HEAD is `41fa494` (use the complete identity in inventory.json), with
uncommitted process installation/planning work. Product behavior comes from the
current branch, not every historical branch. All new documents are English;
the older Norwegian requirement/design wording remains authoritative until its
meaning is reconciled and translated under this assignment.

| Concern | Current authority | Treatment |
| --- | --- | --- |
| Owner scope | #017 and the later 2026-09-29 owner clarification in MAINT-XFMD-0004 | Model current XFMD functionality, responsibilities and dependencies in SDL to guide maintainable extension; feasible SDUI composition uses labeled document areas without live editing; #014/#015 remain separate |
| Product requirements | `xfmd_requirements.md`, including later amendments | Preserve IDs and distinguish a requirement from its actual implementation/evidence |
| Detailed design | `src/blueprint/README.md` and its 37 definitions | Retain FTR/FUNC IDs; requirement references do not prove complete plumbing |
| Runtime/source ownership | `softwareArchitecture.md`, source and CMake targets | One FOX application; interpreter/renderer/contracts are source responsibilities, not automatically processes |
| Delivery evidence | `sprints/README.md`, phase records, `docs/evidence/` | Evidence remains scoped to its original candidate and test scenario |
| Installed process | Manifest → Framework 1.0.0; local profile 0.2 | Reconcile stale local prose; do not copy producer process proposals as installed capabilities |
| Model source placement | `SDP/SDL/README.md` and `AGENTS.md` | System-owned sources; numbered phases contain process material |

The generated inventory captures **10 features, 27 functionality definitions,
281 C++/Rust source/manifest files and 156 referenced requirement identities**.
There are 11 UC, 44 UR and 27 SR table-defined identities. The 45 UR and 28 SR
references also include BoxUI reservations; the 72 AT references include
historical/future scopes. Therefore 156 is an identity-discovery denominator,
not 156 implemented, independent obligations. Amendments add semantics without
necessarily allocating new IDs.

The baseline must be completed with semantic mapping of each definition and
amendment, not merely matching counts. `inventory.json` preserves all occurrences,
definition lines and hashes. Its blueprint filename mentions are search hints:
144 source files have no literal filename mention; some are internal helpers or
Rust modules covered at a broader level. They are a review queue, not 144 proven
documentation defects. The initial inventory had empty model links; the subsequent bounded pilot now
links 17 blueprint identities, with partial coverage explicitly recorded.

## Branch and requirement dispositions

- P0–P41 and subsequent delivered amendments describe the current Markdown,
  rendering, document, preferences and navigation responsibilities.
- Sprint 004 contributes FUNC-031/UC-011, UR-044/SR-027/SR-028 and AT-070–072.
  It generates documents through external SDL tools; it is not an SDUI runtime.
- Sprint 007 contributes content-based opening, shared workspace composition
  and zoom; Sprint 008 replaces the text-type buttons with the Markdown pill;
  Sprint 009 adds favorites, then replaces Add/Remove buttons with context menus.
  Model the latest amendment, while retaining old evidence's original scope.
- Sprint 003/BoxUI branches are separate. FTR-011, FUNC-027–030,
  UR-043/SR-026 and AT-068/069 are historical reservations, not missing features
  to implement on this branch. AT-069 is absent from literal identity extraction
  because the requirement prose abbreviates its reservation as `AT-068/069`.
- UC-006, UR-010, SR-014 and AT-010/024 describe future xfw integration.
  FUNC-031's endpoint does not deliver that requirement.
- #003, #014, #015 and #019 remain separate proposed capabilities. No native SDP
  sidebar, Fyne migration, direct SDL preview or Kitty output is inferred here.

## Required semantics not captured by counts

The modeling review must retain these observed boundaries:

- `DocumentSession` owns text/revision/dirty state; document operations own
  admission and save behavior. UI layout never becomes a second state owner.
- `DocumentViews` captures the window/pane, cancels superseded generation and
  manages leases; an addressed open rejects a dirty main buffer without a modal
  save prompt. A normal user open follows the existing dirty-dialog workflow.
- Files and Index change control visibility; the Files page has tree and
  Folders/Files/Favorites subpanels. Favorites are persistent and separate from
  recent history, sorted by folder/file groups with a conditional separator.
- Context-menu dismissal does not navigate. The menu holds a copied target and
  native keyboard focus; it releases its grab before invoking an action.
- Preferences use a draft, live appearance preview and commit/rollback; a static
  screen does not model Cancel, persistence failure or restoration.
- Rendering, source anchors and asynchronous result generation have independent
  revision/lifetime rules. SDL facts and screenshots cannot establish these alone.

## Evidence limits and next work

The [21 capability probes](../../05--Implementation/XFMD-Modeling/evidence/XM-M2/README.md)
use binaries built from a tracked producer archive, excluding its untracked
`SDL/go/sourceinput/` experiment. There is no claim of a released System parser,
whole-system model coverage, native SDUI interaction or independent review.

The subsequent [coverage reconciliation](Coverage.md) and first
[model pilot](../../SDL/XFMD/README.md) now provide source-family/requirement mapping
and partial structural links. Continue detailed behavior and evidence review for
the remaining inventory. Preserve explicit gaps for facts that the
current language cannot express; do not manufacture parser syntax to close them.
