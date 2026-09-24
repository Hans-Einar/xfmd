# Documentation audit and reconciliation

| Field | Value |
| --- | --- |
| id | KB-XFMD-001 |
| project | XFMD |
| type | Proposal |
| created | 2026-09-23T23:44:59Z |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD audit, BoxUI/SDUI direction and KanBan notes |
| owner | Codex |

## Need and source

The owner asked on 2026-09-24 to investigate process documentation left behind
recent XFMD development. This preserves the investigation as working notes and
possible follow-up, not a new product requirement or approved remediation plan.
The subsequent conversation recalled that the FOX BoxUI direction was superseded
by SDUI/Fyne; that changes which documentation needs completing versus retiring.

## Inspected baseline and evidence

Local Git refs: main `c245fd9`; SDL navigation/leases branch
`sprint/004/phase/050-document-leases` at `47a245a`; BoxUI branch
`sprint/003/phase/048-boxui-verification` at `a7495b8`. BoxUI has 12 commits and
SDL navigation four commits after the same main. They are parallel, not successive
integrated deliveries. Remote PR status was not queried during this investigation.

Read requirements, architecture, working method, implementation plan, sprint and
blueprint documentation; compared relevant code/tests and commit histories.
Existing blueprint checks passed: SDL branch 37 objects/69 requirements/239 callees;
BoxUI branch 41 objects/69 requirements/255 callees. These are structural/symbol
checks, not proof of behavior or a complete semantic audit. No program build or
GUI test was run for this audit.

## Findings by documentation area

| Area | Finding | Recovery source / remaining uncertainty |
| --- | --- | --- |
| Historical implementation plan | P33–P35 are missing; the status heading still describes P0–P13. | Phase branches, commits and P33/P34/P35 design/evidence exist. |
| New delivery overview | The plan explicitly delegates new deliveries to sprints, but the sprint entrypoint lists conventions rather than a consolidated delivery index. | Add navigation/status links, not duplicate full P42–P50 plans in the old plan. |
| Sprint 001–002 | Build metadata, logs and screenshots exist. Sprint 002's README lacks final integration reference; FUNC-026 describes status testing without linking its phase evidence. | Main contains PR35 merge `4e21e99` and PR36 merge `c245fd9`. |
| BoxUI | Five new blueprints and five phase documents exist on its branch; its implementation-plan appendix still says awaiting review. P046/P047 lack precise built revision/version/commands. Existing reused service blueprints were not updated for BoxUI. | Preserve historical results; first determine disposition under KB-XFMD-002 rather than complete an abandoned delivery by default. |
| SDL navigation requirements | UR-044/SR-027 and AT-070/071 describe addressing and navigation, but do not explicitly cover lease lifetime, replacement/close, broker restart, release failures or the 256-entry release queue. | Reconstruct candidate requirements from P050, DocumentViews and external broker tests; owner review still needed. |
| FUNC-031 | Only three plumbing rows. Delivery/open, lease registration, documentChanged, flushReleases and shutdown are absent. Chapter 4 says leases come later, chapter 8 says implemented; verification still says planned. | Map concrete code and actual consumers; preserve errors, dirty-buffer rejection, queue limits and lifecycle. |
| P049 evidence identity | Says phase build from `820d86f`, yet DocumentEndpointTest is introduced in `d6f75b8`. The latter also contains code changes and retesting. | Determine actual tested tree/dirty state; do not assign all results to 820d86f or pretend to know the original build identity. |
| P049/P050 reproducibility | Committed notes and two PNGs exist, but not raw logs, build JSON, full command lines or pinned SDL/renderer test versions. | Recover local artifacts where identifiable; otherwise rerun selected checks under a new, accurately identified baseline. |
| Conditional native test | DocumentViewsGuiTest is registered only with SDL_TOOL_EXECUTABLE; phase instructions omit this configure prerequisite. | CMake/test code and local cache identify it. An ordinary CTest run need not cover this test. |
| Blueprint register | UR-039–041 and SR-021–023 still show Future despite Implemented/FirstRelease objects. Introduction describes an old baseline. | Reconcile six rows with delivered scope and evidence without claiming full Mermaid compatibility. |
| Requirement identity | UC-008 names both A4/PDF and Mermaid. Later BoxUI/SDL requirements lack an equally clear UC trail. | Review established references before assigning a new unique UC; do not blindly renumber published objects. |
| Architecture/design entrypoints | Requirements heading stops at P14, architecture at P15; softwareDesign still labels P25 Mermaid Proposed. Detailed later sections carry much newer information. | Mark historical sections and give one current entrypoint, preserving dated decisions. |
| Method/agent guidance | Language instruction was Norwegian; method/check counts and baseline pointers were stale. | English policy and note workflow are updated by KB-XFMD-004; product-document cleanup remains open. |

This is a targeted review with examples, not a claim that every old blueprint call
or acceptance criterion has been checked. Git can reconstruct implementation and
committed evidence; it cannot prove why an earlier agent missed instructions or
recover an unrecorded test run from its commit message alone.

## Additional recovery sources

Local unversioned files found during the audit: build/generated/xfmd-build.json
identifies clean source `5fa35e37a426a165682a2560df9212e5cf240360`, version
`0.1 sprint/004/phase/050-document-leases:190`. The local LastTest.log records
NavigationGuiTest, DocumentViewsGuiTest and DocumentEndpointTest passing on
September 22 at 15:21 CEST. The CMake cache used SDL_TOOL_EXECUTABLE=/tmp/sdl.
These are recoverable artifacts, not committed evidence or the missing P049 full log.

The external broker test exists in SDP-vNow: introduced by `b784552`, referenced
by SystemDesignLanguage/go/evidence/G6.md, including XFMD commits 5fa35e3/47a245a.
The external checkpoint also identifies XFMD PR38; live PR status is unverified.

[Broker probe at the inspected source revision](https://github.com/Hans-Einar/SDP/blob/321e193258966bea049ea02178192404f8daad99/SystemDesignLanguage/go/tools/verify_native_broker.py)

[G6 evidence at the inspected source revision](https://github.com/Hans-Einar/SDP/blob/321e193258966bea049ea02178192404f8daad99/SystemDesignLanguage/go/evidence/G6.md)

## Next review and completion criteria

Agent recommendation: resolve BoxUI disposition first, then select a bounded
Sprint 004 traceability/evidence repair; handle register/index consistency next.
Do not automatically turn historical BoxUI gaps into integration tasks. Preserve
old evidence, label unresolved provenance, and distinguish reconstructed descriptions
from newly executed verification.

Complete this planning note when findings are reviewed and mapped to selected
work, explicit deferral or irrelevance. Link any resulting active synthesis and
plan; the note does not promise immediate fixes to every document.

## Related notes

[BoxUI disposition](%23002--Study--BoxUI-branch-disposition.md) changes the priority
and meaning of the BoxUI findings. [Widget ownership](../backlog/%23003--Question--SDUI-widget-ownership.md)
keeps the remaining SDUI design question separate from XFMD documentation repair.

## Discussion review — 2026-09-24

Retain documentation reconciliation as a separate backlog proposal. The later SDUI direction changes BoxUI gaps into a disposition question; do not plan full BoxUI documentation completion by default.

## Selected work — 2026-09-24

The owner has now authorized work on this card. Branch:
`docs/kanban-documentation-reconciliation`, based on SDL branch `47a245a`.
This is documentation/process reconciliation, not a new application feature phase.
Earlier audit statements remain dated findings, not a current defect list.

Selected scope: connect AGENTS and CONTRIBUTING to the working method/board;
reconcile Sprint 004 requirements, concrete plumbing and evidence provenance;
preserve identifiable local evidence; repair the delivery index, missing P33–P35
history, scope rows and UC collision. Use English for revised current text; keep
explicitly labelled historical records rather than silently changing their claims.

Completion: map every audit finding to a correction, explicit historical limitation
or a linked follow-up. Existing documentation validators and board/link checks must
pass. No application behavior, dependency pin, installation, branch merge or remote
PR state change is selected. BoxUI disposition and SDUI widget ownership remain
KB-XFMD-002/003. Missing historical test provenance must never be invented.

## Outcome — bounded reconciliation completed 2026-09-24

The selected documentation work is complete locally on
`docs/kanban-documentation-reconciliation`, with no application-code or dependency
change. This outcome does not claim full Sprint 004 runtime verification.

| Audit finding | Disposition |
| --- | --- |
| Workflow/contribution entrypoints | AGENTS now links working method, CONTRIBUTING and board; CONTRIBUTING is English and uses the current sprint/PR convention. |
| P33–P35 absent from phase history | Restored design/implementation/evidence commit rows and main-ancestry references in implementationPlan. |
| Missing current delivery overview | sprints/README now identifies baseline and all four sprints, local branch/integration state and evidence. No remote-state claim. |
| Sprint 001/002 traceability | Sprint 002 integration recorded; FUNC-026 links both actual phase evidence reports. Sprint 001's existing report retained. |
| BoxUI-specific gaps | Retained as historical branch/disposition work under KB-XFMD-002; not promoted into this branch's active product scope. |
| Lease requirements/acceptance | SR-028/AT-072 identify existing best-effort lease behavior and its limits; UR-044/SR-027 clarify addressed delivery and bounded IPC. |
| FUNC-031 call map/status | Reconstructed actual 14-step plumbing and callback ownership; separate modes/lifetimes and verification limits. Existing reused services identify this consumer. |
| P049 test identity | Historical claim retained and explicitly qualified: clean 820d86f cannot contain the later endpoint test. Remaining evidence work is KB-XFMD-005. |
| P050 evidence/reproduction | Copied two local artifacts byte-for-byte with recovery manifest/hashes; external probe/report pinned; commands and missing dependency provenance documented. |
| Conditional native test | SDL_TOOL_EXECUTABLE prerequisite and test-list check documented in phase and contribution instructions. |
| Mermaid scope rows | Six Future rows corrected to FirstRelease; register describes current branch inventory and reserved BoxUI IDs. |
| UC collision | UC-008 remains PDF; UC-010 names Mermaid; UC-011 identifies generated-document navigation. Existing requirement IDs preserved. |
| Stale status entrypoints | English reading guides identify historical phase/design text and point to current sprint/coverage records. Original dated evidence remains historical. |
| English blueprint structure | Templates now use English headings; validator accepts complete English or legacy heading sets while rejecting mixed/missing/reordered/empty sections. |

Checks performed: blueprint structure (37 objects, 70 requirements), symbol presence
(250 callees), layer boundaries and diff whitespace pass. Nine isolated validator
probes cover English/legacy success and malformed headings/broken links. Recovery
hashes and bytes match originals. Restored P33–P35 commits and Sprint 001/002 merges
are ancestors of local main. UC definitions are unique and the six scope rows match.
Board chains, placement and current links are checked after the completion move.
No application build, new GUI result, commit, push, merge or installation is claimed.

The original audit remains above as dated input. Remaining test work is
[KB-XFMD-005](%23005--Study--SDL-acceptance-evidence.md); BoxUI/SDUI choices
remain KB-XFMD-002/003. Existing untranslated baseline prose and old evidence were
not silently rewritten as current English decisions; revised current material is English.

Source-reference maintenance, 2026-09-24: The external SDP checkout moved source files during parallel work. Replace missing local source links with the already-inspected commit-pinned references; preserve the dated findings and card scope.
