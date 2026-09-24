# Version and build identity

XFMD displays `major.minor branch:commit-count`, for example
`0.3 sprint/007/phase/055-document-zoom:196`. On `main`, the latest integrated
PR number is used when available, for example `0.3 PR#39:200`; these are format
examples, not claims that a PR has been merged.

`VERSION` is the sole source of major/minor. Major changes for a deliberate major
product or compatibility break; minor changes for an explicitly selected feature
delivery. A phase, milestone or build does not automatically increment major/minor.
The current product version is `0.3`; CPack derives package version `0.3.0`.

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

## Commit count and history

The commit count is `git rev-list --count HEAD`: the number of distinct commits
reachable from the build's commit, including merge commits. An ordinary commit
increments it by one; a merge may add several because it imports a phase's history.
Phase branches inherit their history and continue counting. Parallel branches can
have the same count; branch/PR and product version are also part of the identity.
This is not a global allocator across parallel branches. No version file changes
for each commit, and a full clean clone yields the same count.

Published history must not be rebased, squash-merged or amended: that would change
the count. Integrate sprint PRs with merge commits. On main, the latest
`Merge pull request #N` is read along first-parent history; documentation commits
after the merge retain the PR name and receive a new count. Without a known PR,
`main` is displayed. The Git SHA is stored as technical provenance in build
metadata, not used as the displayed commit count.

## Builds and special cases

The version is generated during an explicit CMake build, not at commit or startup.
It appears in `xfmd --version`, the window title and the right-aligned bottom status
line; it adds no toolbar. The build writes `build/generated/xfmd-build.json` with
full source identity. An existing binary does not change version when Git moves.

- Local branches use their actual name. Detached checkout displays `detached`;
  GitHub Actions PR checkouts display `ci/<head-branch>`.
- Uncommitted changes, including unignored new files, add `+dirty`. Such builds
  are local trials, not published phase builds.
- Shallow history is rejected with instructions to run `git fetch --unshallow`.
  CI must fetch full history. Never display a misleadingly low count.
- A source archive without Git displays `major.minor source:unknown`; no invented
  commit number. Distributed binaries retain their embedded identity.
- Generated files are written only when their contents change. Rebuilding the
  same clean commit must not relink merely because the date/time changed.

Build frequency and documentation follow the [working method](working-method.md).
