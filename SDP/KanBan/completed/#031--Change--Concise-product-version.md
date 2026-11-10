# KB-XFMD-031 — Concise product version

| Field | Value |
| --- | --- |
| id | KB-XFMD-031 |
| project | XFMD |
| type | Change |
| CardState | completed |
| created | 2026-11-10T01:47:58.056222+00:00 |
| source | owner-conversation-2026-11-10 |

## Need and analysis

The owner asks to remove `sprint/009/phase/057-favorites:206+dirty` from the
visible build name. On this workstation, `~/.local/bin/xfmd --version` reports
that suffix with product version 0.8. Local and GitHub main both identify commit
953d09ec9bee5ef573893cfa73afed13233dfb67, whose VERSION is 0.9.
FUNC-026 generates one display string consumed by CLI, title and status label;
there is no separate UI formatting defect. The new owner decision supersedes
UR-042's earlier branch/PR/count display requirement. SR-025 provenance remains.

## Selected scope and completion

Owner: Project Owner; executor: Codex. Direct bounded maintenance on current main;
leave a reviewable working-tree change, with no push, merge or public release.
Display major.minor in all existing consumers. Retain technical JSON metadata,
update existing isolated Git tests and check the real CLI and native status/title.
Build and update the local executable with a backup so the requested visible change
is available. No new feature, architecture boundary, SDL model, dependency or
version bump is selected. Maintained requirements, architecture, FUNC-026 and
versioning documentation must match the result. Existing model source ownership
and historical evidence remain unchanged.

## Local CI investigation

The existing local CI belongs to minibrowser. Its enabled user timer polls main
once per minute; watch-main.py invokes local-ci.py to build, verify and upload RPMs.
Beinerud's minibrowser-promote signs accepted packages and invokes
beinerud-repo-refresh to regenerate/sign rpm-md metadata. Installed state records
successful minibrowser 1.0.6 publication on 2026-10-26. XFMD's only GitHub workflow
builds/tests; its CPack generator is TGZ and neither public repository channel
contains XFMD. Adding an XFMD publication pipeline is not part of this display edit.

## Evidence and outcome

Delivered in the working tree and local installation as `xfmd 0.9`. Three focused
CTests pass, including native title/status checks; JSON retains technical provenance.
See [candidate, commands and results](../evidence/2026-11-10-product-version/README.md)
and [installation/backup receipt](../evidence/2026-11-10-product-version/installation.json).
No publication pipeline was added or activated.

## Worklog

- 2026-11-10T01:47:58.056222+00:00: registered the owner-authorized display correction after tracing
  FUNC-026 and confirming the separate minibrowser local CI. Same-context checks
  will establish this bounded result, not independent approval.

- 2026-11-10T01:51:00.695087+00:00: completed the objectively authorized display correction with local
  build, native/CLI checks and verified installation. No commit or push performed.
