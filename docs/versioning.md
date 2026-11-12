# Version and build identity

XFMD displays only `major.minor`, currently `0.10`, in the window title and status
line; `xfmd --version` prints `xfmd 0.10`. The owner's 2026-11-10 request supersedes
the earlier visible branch/PR, commit-count and `+dirty` suffixes. Technical
provenance remains in generated and installed `xfmd-build.json`.

`VERSION` is the sole source of major/minor. Major changes for a deliberate major
product or compatibility break; minor changes for an explicitly selected feature
delivery. A phase, milestone or build does not automatically increment major/minor.
The current product version is `0.10`; CPack derives package version `0.10.0`.

## Product milestone decision — 2026-09-24

The owner assigned these product names after the Sprint 007 implementation:

| Product version | Milestone |
| --- | --- |
| 0.2 | The previous navigation delivery. |
| 0.3 | Sprint 007's new workspace UI: unified opening/routing, path/filter row, compact sidebar/history tabs, toolbar/color popup and shared document zoom. |

This follows the existing minor-version rule. The navigation milestone's `0.2`
name is retrospective: the inspected Git VERSION history and installed binaries
still reported `0.1`. Do not relabel historical binaries, amend published commits,
change old test evidence or invent a 0.2 release/tag. The current VERSION changes
directly from 0.1 to 0.3. This decision does not itself publish a release or merge
the sprint PR. See [Sprint 007](../sprints/Sprint-007--Workspace-UI/README.md).

## Product milestone decision — 2026-10-01

The owner selects **0.4** for the SDP project-navigation delivery: native discovery,
on-demand generated views, automatic navigation refresh, dynamic producer-declared
tabs, readable viewpoint labels and original-source opening (KB-XFMD-014/021).
This is an explicitly selected feature delivery under the existing minor-version
rule; it adds a workflow without selecting a major compatibility break.

The authorized action is a local development build and installation. Keep the
branch/count and +dirty identity; preserve prior 0.3 evidence and installed backup.
Version selection does not close outstanding Session acceptance, publish a tag or
release, or authorize model-source edits. See
[SN-004](../SDP/05--Implementation/SDP-Navigation/evidence/SN-004/README.md).

## Product milestone decision — 2026-10-06

The owner selects **0.5** and requests local installation in Session 0002 T009.
This delivery includes raw Mermaid files and local heading links (#023/#024),
plus the #025 runtime-directory, lazy parent selection and bundle-retention
increment. The producer's 24-diagram and remaining per-result limits are still
open; version selection does not imply their removal or close #025.

Use 0.5.0 for CPack and retain branch/count/+dirty identity. The native runtime
checks remain blocked by the current host's socket restrictions. Record actual
installation separately; no public tag/release or model-source update is selected.

## Commit count and history

The commit count is `git rev-list --count HEAD`: the number of distinct commits
reachable from the build's commit, including merge commits. An ordinary commit
increments it by one; a merge may add several because it imports a phase's history.
Phase branches inherit their history and continue counting. Parallel branches can
have the same count; branch/PR and product version remain part of the metadata
identity.
This is not a global allocator across parallel branches. No version file changes
for each commit, and a full clean clone yields the same count.

Published history must not be rebased, squash-merged or amended: that would change
the count. Integrate sprint PRs with merge commits. On main, the latest
`Merge pull request #N` is read along first-parent history; documentation commits
after the merge retain the PR name and receive a new count. Without a known PR,
`main` remains the metadata branch. The Git SHA is stored as technical provenance
in build metadata, alongside the numeric commit count. Neither is displayed in the UI.

## Builds and special cases

The version and metadata are generated during an explicit CMake build, not at
commit or startup. Only the product version appears in `xfmd --version`, the
window title and the right-aligned bottom status line; it adds no toolbar. The
build writes `build/generated/xfmd-build.json` with full source identity. An
existing binary does not change version when Git moves.

- Local branches use their actual name. Detached checkout records `detached`;
  GitHub Actions PR checkouts record `ci/<head-branch>` in metadata.
- Uncommitted changes, including unignored new files, set JSON `dirty: true`. Such builds
  are local trials, not published phase builds.
- Shallow history is rejected with instructions to run `git fetch --unshallow`.
  CI must fetch full history. Never record a misleadingly low count.
- A source archive without Git displays the product version and records branch
  `source`, with null commit/count; no invented provenance. Distributed binaries
  retain their embedded product version.
- Generated files are written only when their contents change. Rebuilding the
  same clean commit must not relink merely because the date/time changed.

Build frequency and documentation follow the [working method](working-method.md).

## Product milestone decision — 2026-10-07

Owner Session 0003 T003 selects **0.6** for the native SDL navigation cleanup and
requests a local build/installation. It includes the earlier 0.5 runtime increment
and preserves branch/count/+dirty provenance. 0.5's prepared package remains
historical; its installation was not recorded as successful. This selection does
not claim producer aggregate views, exact source spans, SDUI launch or all of #028
complete. No public tag/publication or authored-model update is selected.

## SDUI milestone decision — 2026-10-07

Owner Session 0004 T002 selects 0.7 for combined SDUI composition/text preview,
revision-checked source spans, explicit separate-window Fyne launch and SDP toolbar
availability. Build/package/install are authorized; public publication is not.
The observed predecessor is installed 0.6, recorded in N2-001. SDL viewpoint
projections and KanBan mutation services remain separate work. SDUI tools are
bundled privately without changing gh sdp or the project's installed SDP profile.

## Diagram development milestone — 2026-10-10

Owner Session0005 T011 selects 0.8 and requests local installation of the current
development candidate. This minor delivery includes native use-case rendering,
typed overview activation and the verified Mermaid routing/PDF corrections through
N3g. VP01/VP07 overview content depends on the selected producer's capabilities.
Large-overview readability, focused scenario/source work and remaining card
acceptance stay open. Preserve branch/count/+dirty identity; no public tag or
release, gh upgrade or authored-model change is selected. CPack version:0.8.0.

## Navigation development delivery — 2026-10-21

Session0007 T012 authorizes autonomous completion through local installation.
Select 0.9 for the additive viewpoint catalog, exact SDL source navigation,
packet readability and deferred-model discovery workflow. This is the agent's
version selection under the existing feature-delivery rule, not a quoted owner
number. Preserve branch/count/+dirty and exact candidate hashes. The accompanying
SDPTool is a development build of e194721, not a public SDP release. gh-sdp's
signed default and the project's SDP installation receipt stay unchanged.

## Automatic AlmaLinux RPM revisions — 2026-11-11

The owner selects local CI and automatic publication after new main commits.
RPM Version is VERSION plus `.0` (currently `0.10.0`). RPM Release is the full
first-parent commit count plus `.el10_2`. A merge counts once in this packaging
revision; this is separate from the all-reachable count retained in JSON.
A new main commit therefore upgrades through DNF without changing the visible
major.minor or inventing a release tag. Preserve published first-parent history.
The watcher refuses rewinds and non-increasing package identities. Publication
requires exact committed inputs, local tests, clean package verification and
server signing; only a successful public DNF check records watcher success.
See [local CI](../packaging/rpm/README.md) and
[release work](../SDP/Maintenance/RPM/Plan.md). GitHub Release tags remain separate.


## Product milestone decision — Activity0003

The owner explicitly selects **0.10** for readable producer-owned SDL browsing,
including generic native consumption and the exact matching private SDPTool in
the RPM. This is a selected minor feature delivery under the existing contract.
Protocol compatibility retains older producers and existing selection URI handling;
producer order/actions replace native SDL display repairs only when advertised.
Package publication from the qualified feature branch is authorized; merging main
is a separate action. Version selection does not claim publication or acceptance.
