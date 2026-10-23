# Derive SDP tabs, readable navigation labels and source opening from discovery

| Field | Value |
| --- | --- |
| id | KB-XFMD-021 |
| project | XFMD |
| type | Change |
| CardState | gate-review |
| created | 2026-10-01T09:28:31.148284+00:00 |
| source | Owner conversation 2026-10-01, SESSION-XFMD-0001 T003; supplied SDP agent explanation |
| next_review | Review delivered SDL/SDUI source behavior with the released producer; N4 evidence available |
| tags | sdp, discovery, navigation, source, labels |

## Owner request and scope

Use `gh sdp <root> discover --json` as the authority for SDP sub-tabs. Resolve
`navigation.roots` IDs through `navigation.nodes` and construct tabs from nodes
whose `kind` is `tab`. Future categories such as Sessions must not require a new
hardcoded tab in XFMD. Show descriptive abstraction/viewpoint names in the tree,
and offer **View source** when right-clicking a node associated with a `.design`
source. Add this work to the continuing Session and its existing implementation
plan. This turn captures and plans the change; it does not implement it.

The owner supplied an SDP agent statement that the installed 2.0.0 producer
already exposes SDL/KanBan/SDUI tab kinds, and that future Sessions discovery
needs an appropriate released tool/client update, not a project `gh sdp . upgrade`.
Treat the future upgrade statement as a reported dependency, not evidence of a
published or installed Sessions-capable release. No upgrade is performed here.

## Investigation — observed 2026-10-01

Real command: `SDP_OFFLINE=true gh sdp . discover --json` in this repository.
[Response excerpts and identity](../evidence/2026-10-01-sdp-navigation-discovery/observations.json)
record the observed sdptool/0.2 response, not simulated data.

| Concern | Producer response today | XFMD development implementation today |
| --- | --- | --- |
| Tabs | roots are `files`, `sdl`, `kanban`, `sdui`; only the latter three have kind `tab`, with labels SDL, KanBan, SDUI | SdpProtocol ignores roots and requires three fixed IDs; SdpPanel has fixed names/order and arrays of three trees |
| Viewpoints | Canonical `kind: viewpoint` nodes have complete labels, e.g. `VP02 — Architecture and logical decomposition` | References under A-levels use labels such as `VP02`; SdpPanel follows the reference for targets/children but retains the short alias label |
| Abstractions | A0–A5 nodes have only code labels; `delivery` is another grouping. No descriptive name/title is present on those abstraction nodes | Displays the returned code; cannot obtain missing names from that node |
| Source association | `sources[]` maps model `id` to project-relative `source`, kind/profile/state/revision; file-tree nodes have typed absolute `open` paths | SdpReply retains neither sources nor their mapping; no source context menu exists in SdpPanel |
| Sessions | Existing Sessions directory appears under the Files root; no Sessions tab root was returned | A future producer tab would currently be ignored |

Identifiers are **VP01–VP11**, not WP01–WP11. Canonical viewpoint labels already
cover all eleven viewpoints. No local title dictionary is needed for them.
Source rows and descendants with `target.model` can use that model ID to resolve
`sources[].id`; use the returned project root as base for `sources[].source`.
A source row that is invalid may lack a typed target, so source association for
those rows must also be covered explicitly before implementation.

Read-only implementation references: `src/application/sdp/SdpProtocol.h/.cpp`,
`src/application/ui/SdpPanel.h/.cpp`, `src/application/sdp/SdpNavigation.cpp`,
`src/application/io/FileOpenPolicy.cpp` and `ApplicationOpening.cpp`.

## Planned behavior and acceptance

1. **Dynamic tabs:** preserve root order, resolve IDs in a node map and create one
   native tab per root with kind `tab`, using its label, state and diagnostic.
   Do not turn the `files` directory root into another tab. Do not require the
   three legacy IDs or a fixed count. Validate missing/duplicate root references,
   bound graph/tab work and retain existing diagnostics and action allowlists.
   Preserve the selected tab by stable ID across reorder/add/remove; choose a
   defined available fallback if it disappears. Zero tabs and unavailable tabs
   must not break Files/Index or discard the current document.
2. **Readable labels:** for viewpoint references use the resolved canonical
   viewpoint label while retaining the same identity/target and reference guard.
   Preserve meaningful card Ref labels and aliases for other node kinds.
   Consume descriptive abstraction labels from the producer when available;
   use the code as truthful fallback. Do not guess layer membership or rename
   IDs/URIs. Full labels should remain accessible when sidebar width clips them.
3. **View source:** right-click a source/model row, or descendant with an
   unambiguous source association, and open the original `.design` in XFMD's
   native text editor. Opening source must not trigger diagram generation or
   replace it with generated Markdown. Reuse dirty Save/Discard/Cancel behavior;
   canceling preserves the current buffer. Revalidate current root/source
   association after refresh, require a regular contained file and handle missing,
   renamed, invalid or ambiguous sources visibly. Never infer a path from label
   text or execute a project-supplied command. For composed models, open the
   returned model entry source unless the producer explicitly identifies a more
   specific source; do not guess a declaration's physical file.
4. **Regression evidence:** real discovery plus fixtures for reordered/missing/
   extra tab roots (including a future Sessions tab), reference labels and bare
   A-level fallback; native right-click on source and descendant rows; dirty
   cancel, invalid-source editing, path containment and refresh races. Opening
   source and selected-view generation remain separate actions. Retain M3's
   existing link/window/output/shutdown checks and model/design reconciliation.

## Producer-owned gap and next contract check

SDPTool should return descriptive A0–A5 labels (or explicit stable code/title
metadata) derived from SDL's authoritative level catalog. XFMD should consume
these, not maintain an independent translation dictionary. The current upstream
`SDL/docs/integration/SDL-Viewpoint-Levels-and-Notation.md` describes A0 needs/
obligations, A1 functional intent, A2 system/Container, A3 internal Units/layers,
A4 behavior/contracts and A5 realization/evidence. These descriptions are read
from documentation, **not present in the inspected JSON**. Exact API wording and
source association for target-less invalid rows are producer contract questions.
No SDP-vNow files were changed and no message was sent to its agent.

The producer's returned `expansionDepthLimit` should be included in the adapter
review alongside roots and sources; it is currently discarded. Preserve XFMD's
independent hard resource bounds while respecting any stricter producer limit.

## Relationship and queue

[KB-XFMD-014](../active/%23014--Proposal--SDP-sidebar-and-generated-navigation.md)
retains the general SDP navigation delivery. This card owns the newly requested
metadata-driven presentation/source-access increment, not a duplicate of its
completed M2 evidence. No card is superseded or merged.
[KB-XFMD-015](../backlog/%23015--Proposal--SDL-design-file-preview.md) retains editable SDL
source paired with generated preview; merely opening source here does not deliver
that feature. [KB-XFMD-017](../active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md)
retains semantic modeling and independent review.

Selected under [PLAN-XFMD-0006 M3a](../../05--Implementation/SDP-Navigation/Plan.md),
inside S2 of [SESSION-XFMD-0001](../../Sessions/session-%230001--SDP-project-navigation.md).
This is the next bounded sidebar refinement before #015/M4. First reconcile
UR-046/SR-029 and FUNC-032 with the observed producer contract, then implement and
verify. #015 remains selected later but returns from queued to backlog so there
is only one next queued card. Missing A-level titles must not block dynamic tabs,
viewpoint labels or the source-opening work that the current contract supports.

## Worklog

2026-10-01T09:28:31.148284+00:00: recorded owner request and real discovery findings; added M3a and Session
links, queued this card and retained unfinished M3/#015/#017 scope. Planning-only
checkpoint; no XFMD binary, producer release or installed project was changed.

Registration: EVT-KB-XFMD-000091. Queue-related reviews: EVT-KB-XFMD-000092
(#015), EVT-KB-XFMD-000093 (#014); plan update EVT-PM-XFMD-000040. Validation:
`python3 SDP/KanBan/verify_board.py` passed (21 cards, 93 card events, 12 management
records); local card/plan/Session links and `git diff --check` passed. Product
source/build/install hashes remain unchanged from SN-002. The staged #020 card
was preserved.

## T004 execution — 2026-10-01

Owner explicitly authorizes implementing the complete #021 increment this turn.
Future real Sessions-producer testing follows the owner-provided tool update;
current tests must cover an extra tab without hardcoding its identity.

## T004/T005 result and next check

Implementation is delivered for review; see
[SN-003](../../05--Implementation/SDP-Navigation/evidence/SN-003/README.md).
Ordered tab roots, reference labels, producer A-label fallback, source catalog
association and native View source are implemented. Controlled native fixtures
cover an extra Sessions tab, reordering/removal, changed labels, zero tabs and
refresh while the source popup is open. Real SDPTool checks cover source opening,
invalid SDL, dirty cancel/discard and path/revision rejection.

CardState is gate-review for the owner-provided producer update and resulting
native Sessions check. No future producer release, independent review or owner
acceptance is claimed. Keep the card active until that check is recorded; remaining
M3 boundary work and #015 paired preview retain their separate scopes.

Owner T005 temporarily holds SDL/SDUI model-source and generated-view changes;
[KB-XFMD-022](../backlog/%23022--Idea--Git-based-design-baselines-and-change-blueprints.md)
captures Git/release baselines and change-blueprint discussion. This exception is
explicit; it does not turn the older model into a description of this increment.

## Session 0003 refinement — source identity and exact spans

Wherever generated content shows a source ID, make it a link to the original
source and highlight the exact referenced lines. Whole-file View source is the
existing delivered boundary; heading-fragment support (#024) does not implement
source-span navigation. Preserve the earlier delivery evidence with its scope.

Specify a producer/consumer source-reference contract carrying original file,
stable source/object identity, revision and start/end positions (document line/
column base and units). Handle declarations, relation facts, composed files and
reused SDUI instances; one ID may map to several source spans. Do not guess line
numbers from generated Markdown or resolve merely by basename. When provenance is
missing, show unavailable rather than opening an unrelated line. If the file has
changed, explicitly report stale provenance and resolve against matching revision
or re-resolve the ID; never highlight a misleading current line silently.

XFMD preserves dirty-buffer checks, selects/reveals the source editor range,
retains history, handles missing/unreadable files and cancels stale asynchronous
jumps. Test multiline/multi-file spans, Unicode positions, changed/unsaved source,
multiple declarations and generated-history navigation. Link both Markdown IDs
and diagram IDs where the diagram interaction contract supports them; provide a
source-reference list for diagrams until native node hit targets are supported.
Coordinate #028/#027 and upstream KB-SDP-032/KB-SDUI-005. This refinement remains
unimplemented and does not turn prior whole-file tests into span acceptance.

[Session 0003](../../Sessions/session-%230003--SDL-navigation-and-SDUI.md).

SDL diagram successor: [Session0005](../../Sessions/session-%230005--SDL-viewpoint-diagrams.md), created after SDUI0.7 delivery. Recover live producer contracts before N3/N4 execution; no new completion claim.

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
