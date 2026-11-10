# Local RPM CI and publication

| Field | Value |
| --- | --- |
| id | MAINT-XFMD-0006 |
| project | XFMD |
| state | active |
| PlanType | MaintenancePlan |
| BranchPolicy | current |
| CommitPolicy | milestone |

## Outcome and authority

Owner conversation 2026-11-11 explicitly requests an XFMD RPM published to Beinerud
using minibrowser's local CI method, and selects automatic publication after new
main commits. [KB-XFMD-032](../../KanBan/active/%23032--Change--Local-RPM-CI.md)
tracks the assignment; [Session0008](../../Sessions/session-%230008--RPM-publication.md)
tracks the route. Work continues through a signed public RPM, clean DNF installation
and an enabled local watcher. Existing concise version work KB031 is included.

## Baseline, design and compatibility

No XFMD RPM or release tag exists. Main is 953d09e, product 0.9; preserve the owner's
concise display. Package Version is 0.9.0; Release is the full first-parent commit
count plus .el10_2, increasing on every main commit without mutable counters or
new tags. This keeps product version separate from packaging revisions. Refuse
rewritten main history and a non-increasing published RPM identity.

Use a dedicated clean checkout and digest-pinned AlmaLinux 10.2 container. Build
and run the existing application tests before packaging; install the exact RPM
in a second clean container and check CLI, native launch, assets, metadata and
upgrade/removal. GitHub remains a separate non-publishing source check.
FOX is not supplied by target repositories: build pinned 1.6.57 and ship its
shared library privately under /usr/lib64/xfmd, with source and license. Do not
claim or replace system FOX. The package uses distribution libraries otherwise.
Optional SDPTool/SDUI tools are independently installed integrations; do not copy
unidentified local helper binaries into a redistributable package.

Server signing stays on Beinerud. Use a separate restricted XFMD receiver/promoter
and existing beinerud-repo-refresh, preserving minibrowser and other applications.
Only verified package bytes may be uploaded. Reject foreign paths/package names,
symlinks, oversized uploads, invalid channel/version and conflicting overwrites.
Verify signatures/downloads and DNF metadata after publication. Reuse existing
public key, repository layout and Apache configuration.

## Git policy and milestones

Use current unprotected main, preserving existing history. Commit the prior concise
version work separately, then commit the tested pipeline milestone. Push these
commits to origin/main as necessary to fulfill authorized automatic main publication.
No unrelated merge, GitHub Release or application version bump is selected.

- M1: committed reproducible pipeline, package verification and receiver/watcher
  rejection tests; durable installation and version contracts match implementation.
- M2: tested server receiver, first signed stable RPM, HTTPS and clean DNF proof,
  installed/enabled local timer and truthful evidence/closeout.

## Verification and current status

In progress. Record commands, exact candidate and results in this work area.
Use the copied ReleaseChecklist for applicable application gates; SDP Toolkit
publisher operations are not application release requirements. Same-context
checks are not independent review; no independent review has been obtained.
