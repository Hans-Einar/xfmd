# BoxUI branch disposition and reusable changes

| Field | Value |
| --- | --- |
| id | KB-XFMD-002 |
| project | XFMD |
| type | Study |
| created | 2026-09-23T23:44:59Z |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD audit, BoxUI/SDUI direction and KanBan notes |
| owner | Codex, selected by the project owner |
| next_review | Revisit conditional runtime candidates only with a demonstrated consumer/regression; remote cleanup only if selected |

## Need and owner direction

On 2026-09-24 the owner recalled dropping the FOX-widget BoxUI variant in favor
of standalone SDUI/Fyne, and asked whether other branch changes should be kept.
The earlier audit must not be read as a recommendation to merge the whole branch.
This note records inspected facts and an extraction recommendation; it does not
close a PR, delete a branch, retire requirements or authorize code integration.

## Direction confirmed by local sources

In SDP-vNow at `321e193258966bea049ea02178192404f8daad99`, checkpoint 07 records
CP1-D19–D21 on 2026-09-21: Go replaces the planned Rust/C-ABI core, Fyne is the
first standalone interactive host, no parallel FOX/TUI implementation now, and
shared SDUI geometry also supports static SVG. Target architecture SDUI-ARCH-003
explicitly supersedes the mandatory FOX/XFMD route. Checkpoint 11 describes
implemented Go/Fyne and XFMD's separate document navigation role.

[SDUI direction at the inspected source revision](https://github.com/Hans-Einar/SDP/blob/321e193258966bea049ea02178192404f8daad99/docs/checkpoint%231/07-SDUI-0.2-and-Go-Direction.md)

file:///home/warloc/git/SDP-vNow/SDUI/docs/target-architecture.md

[Implementation checkpoint at the inspected source revision](https://github.com/Hans-Einar/SDP/blob/321e193258966bea049ea02178192404f8daad99/docs/checkpoint%231/11-Go-Implementation-and-Navigation.md)

Conclusion: the FOX BoxUI work is historical reuse material for this direction,
not a prerequisite for current SDUI. XFMD remains a FOX Markdown/document host;
it can show exported UI images without becoming the interactive SDUI host.
The sources do not establish a formal cancellation of XFMD PR37. Live PR state
and current installed binary were not checked.

## Branch review

Compared main `c245fd9` to `sprint/003/phase/048-boxui-verification` at `a7495b8`
(12 commits). Most changes implement BoxUI parsing, ABI/JSON codecs, session,
FOX controls, layout, preview/PDF, tests, dependency pin and documentation.
The following small changes are separable candidates, not already adopted fixes:

| Candidate | Source | Assessment and gate before extraction |
| --- | --- | --- |
| Limit Rust formatting to XFMD's four packages | `ffb98e8`, .github/workflows/build.yml | Strongest general candidate. Replaces cargo fmt --all with explicit package selection; avoids checking/formatting a pinned path dependency. Those four packages also exist on the current branch. Reproduce/verify on the current baseline, then copy the isolated hunk. |
| Exclude .deps and third_party from local Markdown link validation | `c81b508`, tools/validate_blueprints.py | Independent validator boundary improvement; dependency documentation should not be treated as XFMD-owned links. Retain checking of project documents, including the new board. Verify a broken project link still fails. |
| Match additional SVG color spellings | `1ecffa1`, src/application/adapters/DiagramPainter.cpp | Recognizes lowercase forms and lowercase hex with ff alpha in addition to the original spelling. Potentially useful beyond BoxUI. Demonstrate a current Mermaid case and test light/dark plus intentional text-color exceptions; do not claim all SVG spellings/alpha cases are supported. |
| Forward key events to focused FOX children | `ffb98e8`, src/application/adapters/FoxPreviewInput.cpp | Confirmed fix for BoxUI's embedded input; no demonstrated present need in XFMD's non-BoxUI preview. Keep as conditional knowledge, not a mandatory carry-over. Requires a real consumer and native event regression before adoption. |

No other independent runtime feature was identified in the reviewed XFMD diff.
This is not an audit of all changes inside the renderer-fork pin range
589517a..61a85b6. Do not update the dependency pin merely to preserve these XFMD
hunks; a separate fork review is needed if that upgrade is proposed.

The status-version changes predate the branch and are already on main. Do not
attribute them to BoxUI or copy them again. Full commits listed above mix these
candidates with BoxUI work; do not cherry-pick them blindly.

Historical lessons worth retaining: actual X11 key events found a focus-chain bug
that direct setText/widget-handler tests missed; draft/accepted-state separation,
revision checks and frozen export semantics are useful design/test references.
SDUI already has its own runtime and host; this is not a request to port the C++ code.

## Next review and completion criteria

Agree the branch/PR disposition and a keep/skip decision for each candidate.
Agent recommendation: retain history; investigate the first two candidates first,
consider SVG mapping only with a current regression, and defer FOX child-focus
routing unless a non-BoxUI consumer needs it. Record any selected extraction in an
active card with focused acceptance and the original source commit/hunk.

If the BoxUI effort is formally retired, describe that outcome rather than spending
time completing its old product documentation as if it were still planned. Preserve
IDs, branch commits and evidence. Closing/deleting/publishing is a separate action.

## Related notes

[Documentation reconciliation](%23001--Proposal--Documentation-reconciliation.md)
contains the older audit and its updated priority. The question of entirely custom
SDUI widgets remains [separate](../backlog/%23003--Question--SDUI-widget-ownership.md).

## Discussion review — 2026-09-24

Retain branch disposition and isolated change candidates together. No whole-branch merge or code extraction is selected. Documentation repair and the SDUI widget decision have different outcomes.

Source-reference maintenance, 2026-09-24: The external SDP checkout moved source files during parallel work. Replace missing local source links with the already-inspected commit-pinned references; preserve the dated findings and card scope.

## Selected work — 2026-09-24

The owner selected item #2 after the proposed isolated extraction was described.
This supersedes the earlier review's statement that no extraction was selected.
Scope: verify and adopt the two independent tooling fixes; assess SVG mapping and
FOX child-focus routing against current consumers; retain BoxUI branch history.
Owner: Codex. Completion requires keep/defer decisions, source attribution,
focused regression evidence, and consistent board/ledger links. Remote PR closure,
branch deletion and dependency-pin upgrades are outside this selection.

Implementation delivery: [Sprint 005, P051](../../../sprints/Sprint-005--Tooling-Boundaries/Phase-051--Tooling-Boundaries.md).

## Outcome — 2026-09-24

The selected study and isolated extraction are complete in the working tree.
The earlier candidate table records the pre-selection assessment; this section
is the current disposition. Implementation and repeatable check evidence belong
in [Sprint 005 / P051](../../../sprints/Sprint-005--Tooling-Boundaries/Phase-051--Tooling-Boundaries.md),
not in this discussion note.

| Candidate | Disposition | Evidence / reason / resumption trigger |
| --- | --- | --- |
| Rust formatting scope, ffb98e8 | Adopted isolated CI hunk | Current `--all` traverses dependency targets; selected packages do not. Both pass today's files. Temporary negative controls prove dependency-only defects are ignored and defects in each XFMD package still fail. |
| Dependency Markdown exclusion, c81b508 | Adopted isolated validator hunk | Dependency links are outside XFMD ownership. Temporary probes verify project and KanBan failures remain detected, including project links into missing dependency files. Existing English/legacy blueprint support is preserved. |
| SVG color spelling, 1ecffa1 | Deferred; no runtime hunk copied | Current adapter requests Theme::modern; its primary colors already match the existing uppercase mappings. DiagramReadingTest contains light/dark palette checks, but was not rerun. No current regression was demonstrated. The candidate also broadens white-fill matching without broadening the intentional uppercase-only white-text exception, so it needs targeted text and palette coverage before reuse. This is not proof that every existing diagram/color spelling is correct. Revisit with a failing supported Mermaid fixture. |
| FOX focused-child routing, ffb98e8 | Deferred; no runtime hunk copied | Current FoxRenderHost builds scrolling/paint infrastructure without the BoxUI input controls that motivated the fix. No present non-BoxUI input consumer was identified. Revisit when embedding a focusable child that requires routing, using actual native key events. |
| Renderer fork pin 589517a..61a85b6 | Retain current 589517a pin | The two tooling fixes do not require an upgrade. Fork-internal changes have not been audited; audit them separately if proposing an upgrade. |
| Remaining BoxUI implementation | Retain branch history; no whole-branch integration | SDUI's selected standalone Go/Fyne direction supersedes the mandatory interactive XFMD/FOX route. C++ implementation, fixture and test lessons remain inspectable in the original commits. No additional independent runtime feature was identified by the branch review. |

Working branch: `sprint/005/phase/051-tooling-boundaries`, based on `47a245a`
with the earlier uncommitted documentation reconciliation preserved. The two
source commits were not cherry-picked wholesale. No new commits, remote PR
changes, branch deletion, merge or installation were performed. Formal PR37
closure is not inferred from this completion; historical IDs/evidence remain.

The bounded result is two reusable tooling fixes plus explicit decisions on the
remaining candidates, not a claim that BoxUI runtime acceptance is complete.
KB-XFMD-003 retains the separate SDUI widget-ownership discussion; KB-XFMD-005
retains SDL acceptance evidence follow-up.
