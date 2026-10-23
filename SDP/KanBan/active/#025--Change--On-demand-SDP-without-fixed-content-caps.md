# On-demand SDP navigation without fixed content caps

| Field | Value |
| --- | --- |
| id | KB-XFMD-025 |
| project | XFMD |
| type | Change |
| CardState | in-progress |
| created | 2026-10-06T01:17:23.175880+00:00 |
| source | Owner Session 0002 T006, 2026-10-06 |
| next_review | Review remaining source/file/byte, per-model and GUI guards after the installed N15b delivery |
| tags | SDP, generation, resources, navigation |

## Owner decision

The owner expects generation on demand, recalls a RAM-backed /run location, and
explicitly rejects the diagram-count restriction. Navigation must not become
unavailable because a project/selection has more than an arbitrary number of
items. This decision supersedes treating the current 24-diagram cap as desired
product behavior. Capture the complete path, not just one constant.

## Inspected baseline behavior — T006

- SDPTool/preview.go rejects more than 24 selected diagrams before document
  generation, then rejects more than 128 files or 32 MiB of generated output.
- XFMD SdpNavigation.cpp independently caps retention at 64 contexts / 256 MiB,
  and checks 128 files / 32 MiB per bundle. Successful bundles are retained until
  the window/session owner releases them; the destructor removes its temp tree.
- Other potentially exposed boundaries include 32 MiB tool/JSON input, discovery
  node/depth limits, timeout/watch limits, 8 MiB documents, and Mermaid profile
  limits (64 diagrams/document, 128 flowchart nodes, etc.). They are not all the
  same contract; inventory them before claiming navigation is unrestricted.
- New SDP sidebar uses std::filesystem::temp_directory_path()/xfmd-sdp-XXXXXX,
  usually /tmp, not WindowEndpoint::runtimeDirectory(). The older addressed
  navigator/IPC uses XDG_RUNTIME_DIR/xfmd, with a private /tmp fallback.
- Observed in this agent environment: XDG_RUNTIME_DIR=/run/user/1000; /run/user/1000
  is tmpfs; /tmp is xfs. This verifies mounts, not the environment of every running
  desktop XFMD process. Temporary storage does not itself imply RAM-backed storage.

## Complete implementation boundary — partially implemented in T007

1. Generate a selected detail on demand. Parent/group nodes should provide a
   complete navigable index rather than eagerly embedding every descendant.
   Progressive loading/pagination may control work per interaction, but must not
   omit items or impose a total browsable-content cap. Broad/all selection, when
   explicitly requested, must not silently truncate or fail at an arbitrary count.
2. Remove the producer's fixed selection cap together with compatible consumer
   bundle/retention behavior. Do not just change 24 to another magic number or
   bypass validation to make one sample pass. Audit limits by observable workflow;
   preserve UTF-8, path containment, schema and stale-result correctness.
3. Prefer a private XDG_RUNTIME_DIR location when available. Specify fallback and
   failure behavior explicitly. RAM-backed storage still has finite resources;
   actual allocation/I/O failure must be reported, not replaced by a fixed
   product-size restriction. Keep source/generated-resource ownership separate.
4. Release obsolete bundles according to document/history/resource references;
   regenerate history targets when necessary. Preserve current documents and
   source revisions, cancellation and rapid-selection stale-result rejection.
5. Keep generation cache and navigation lazy loading separate from rendering a
   single extremely large diagram. No universal single-diagram support claim
   follows merely from lifting the facade count check.

## Acceptance and handoff

Exercise 25+ diagrams in one model, >64 successive selections, revisiting history,
project switch/window close, cancellation, generator failure, temp fallback and
actual storage failure. Verify every entry remains reachable, no hidden truncation,
no growing unreachable bundle tree and no stale document replacement. Add explicit
coverage above every former user-visible cutoff being retired.

Producer owner: SDP-vNow SDPTool/SDUI/SDL as applicable. Consumer owner: XFMD.
Current environment permits reading SDP-vNow but not editing it or installed
binaries; the 24 cap has not been removed. This card is the durable change/handoff,
not a claim that the installed behavior already follows the owner decision.

Related: [#014](../active/%23014--Proposal--SDP-sidebar-and-generated-navigation.md),
[#021](../active/%23021--Change--Dynamic-SDP-tabs-labels-and-source-opening.md),
[#026](../superseded/%23026--Proposal--SDUI-navigation-and-source-structure-views.md).
[Session 0002](../../Sessions/session-%230002--Standalone-Mermaid-files.md), T006.

## T007 execution selection — 2026-10-06

Owner selects runtime-directory and on-demand fixes before #026/new Session.
Execute [PLAN-XFMD-0008](../../05--Implementation/SDP-Runtime/Plan.md).
The local runtime/parent-selection/lifetime increment is authorized now; remaining
producer and global content-limit work remains visible, not silently completed.

## T007/T008 implementation and verification checkpoint

The local candidate uses private XDG_RUNTIME_DIR (otherwise /run/user/<uid>) with
no /tmp fallback, expands parent viewpoints without bulk generation, removes the
cumulative retention stop and prunes idle bundles outside current/history usage.
[RT-001](../../05--Implementation/SDP-Runtime/evidence/RT-001/README.md) captures
source/binary identity and partial test evidence. Release build and runtime unit
pass; native acceptance remains blocked by host socket restrictions, reproduced
on owner-requested retry. This candidate is not installed. Producer and other
content-limit reconciliation remains unfinished; CardState stays in-progress.

## 2026-10-07 runtime verification and installation

Session 0003 T003's 0.6 candidate passes the native 105-selection, snapshot history,
cleanup and real-producer regressions; installation completed with backup.
[N2-001](../../05--Implementation/SDL-Navigation/evidence/N2-001/README.md) contains
exact successor evidence. Earlier environment-blocked results remain historical.
Producer 24-diagram/per-bundle and other cutoff work is still unfinished; this card
stays in-progress. Do not equate runtime verification with unrestricted generation.

## Session0005 T021 — N4 implementation and residual handoff

[N4-001](../../05--Implementation/SDL-Navigation/evidence/N4-001/README.md)
delivers focused use cases/explicit scenarios and original composed-source spans.
Fixed 24/48 diagram-count caps are removed, with 50-diagram coverage. Producer
commit fde2239 follows ecd9dcd on the main-based successor; full producer/native
checks pass. Main/PG merge forecasts are clean; no merge/release/install.

[Current disposition](../../04--Design/SDL-Navigation/Viewpoint-Disposition.md)
records exactly what this card still owns. Session implementation closure does
not complete this broader card or waive large-print, compatibility, remaining
resource limits, integration or review obligations. CardState is unchanged.

## Session0007 T006 — Whole-project discovery limit

The growing XFMD checkout now reproduces `limit: combined inventory exceeds 20000 nodes or 32 MiB` for whole-project discover. Both N9 baseline and N10 candidate reproduce it; this is not introduced by VP08. Explicit model tree/select still works. Generated evidence contains discoverable model copies, so source selection, lazy inventory and explicit resource diagnostics need coordinated producer handling. Do not silently hide evidence models or discard sources; preserve owner decisions about ordinary model visibility. This remains KB025 scope, not an N10 resource-limit fix.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N10-001/README.md

## Session0007 T011 — deferred discovery producer

N15a candidate e194721 implements opt-in compact discovery with all sources
retained and revision-bound per-model expansion, shared by CLI and JSON actions.
Full SDPTool tests/vet, producer document validation and twelve affected native
compatibility checks pass. The actual checkout retains 63 sources in compact
discovery; XFMD expansion returns all eleven overview targets. The aggregate
fixture expands 22,236 nodes across twelve models without building one combined
graph. Default eager behavior remains compatible. See N15-001 evidence.

Next: N15b native capability negotiation, expansion/atomic tree merge and stale
response handling. Installed XFMD still uses eager discovery. Source parsing,
remaining resource guards, integration and independent review are not completed
by N15a. KB025 and the broader plan remain active; no authored models changed.

## T012 delivery — N15b / XFMD 0.9

Owner-authorized local installation is complete. N15b negotiates producer support,
loads source graphs on expansion, validates/merges atomically and preserves dirty
buffers, source actions and refresh state. Full final CTest passes 94/94 (47 GUI).
The actual project loads all eleven XFMD viewpoints and generates native VP02.
Installed discovery retains 63 sources; the installed GUI is observed invoking
its private SDPTool with lazy-sdl, without an environment override.

XFMD 0.9 and standalone/private SDPTool 2.2.0-dev at e194721 are installed with
per-file checksum verification and predecessor backups. Mermaid and SDUI/Fyne
installed smoke checks pass. gh-sdp's signed 2.2.0 default and project receipt are
unchanged. This is a shared-dirty development delivery, not a public release or
main integration. Source archive, candidate/package hashes, screenshots, logs,
checklist and installation receipt are in N15b-001.

Remaining KB025: source scanning/parsing costs, filesystem/byte/per-model guards,
GUI row/depth guards and demand-driven non-SDL inventories. Broader KB020/028/032
acceptance, integration, legacy export alignment and independent review remain.
No authored SDL/SDUI edits or unrelated shared-tree staging. Cards/plan retain
active scope; local installation does not close those residual obligations.
