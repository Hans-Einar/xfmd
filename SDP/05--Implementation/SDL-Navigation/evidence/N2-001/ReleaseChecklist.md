# ReleaseChecklist

Copy this checklist into the selected release's work area. Record version,
previous release, exact commit, owner publication authority, evidence paths and
actual outcomes. Unchecked means pending; use N/A only with a reason. A generated
release log is not proof of publication. The project release contract governs.

## Scope and content

- [ ] Select product version from compatibility impact. Keep language/profile,
  process/management schema and client versions distinct.
- [ ] Enumerate included work and remaining exclusions; validate selected model,
  tests, review dispositions and links on the actual candidate.
- [ ] Update canonical RELEASE-NOTES.md Unreleased entries with stable work IDs.
  Freeze selected entries into a dated version section; preserve released bytes.
- [ ] Run `sdptool release-log --version X.Y.Z --output Releases/X.Y.Z.md`.
  Check with the same command plus --check. Use --all --output Releases --check
  to check every recorded version. Generator extracts reviewed notes, not Git guesses.

## Installation distribution (SDP publisher)

- [ ] Check Template/sdp-root and project-root guidance. Include Sessions guide and
  blank template, never project conversations, boards or existing work records.
- [ ] Review SDPTool/profiles/payload.json: every distributed file, destination,
  ownership and removed/moved path. Inventory completeness tests must pass.
- [ ] Review SDPTool/profiles/five-phase.json: capabilities, schema/profile versions
  and exact supported predecessor descriptor digests, including current stable.
- [ ] Reconcile SDP.manifest.yaml, Toolkit/SDP-install.manifest.json compatibility
  metadata, package version, release record and notes. Retained legacy metadata
  does not replace the current Go inventory or signed descriptor.
- [ ] Build package and immutable sdp-release.json from a clean exact commit using
  the Go profile builder. Verify file hashes, sourceCommit and executable identity.
  Never edit the built descriptor or use a project-local manifest as authority.
- [ ] Sign descriptor with the trusted publisher key; verify signature, binary hash
  and SHA256SUMS. Store no private keys or secrets in evidence/repository.
- [ ] Rehearse install and each supported upgrade from original verified descriptor
  bytes. Check new Sessions files, existing Session/document preservation, receipt,
  Maintenance entry/history, conflict handling and repeated no-op.
- [ ] Test extracted-source operation, packaged signed child and normal bootstrap.
  Development artifacts or test signers are not production signed evidence.

## Publish and consumer readiness

- [ ] Exact candidate CI and required independent review pass; working tree clean;
  tag absent; merge/publication explicitly authorized.
- [ ] Publish annotated tag and GitHub Release with generated log, signed descriptor,
  executable and checksums; verify downloaded assets against exact release.
- [ ] Reconcile actual tag/commit/time and append truthful release events only after
  success. Do not rewrite a released log or silently correct released notes.
- [ ] Verify gh-sdp selection: its compiled default is pinned. Either publish/update
  the client default or document an explicit SDP_RELEASE selecting this release.
- [ ] Provide preview/apply commands and expected receipt. Owner-requested manual
  project upgrade remains manual; never confuse updating the gh extension with
  upgrading the project's SDP directory.

Consuming projects reuse the scope/content/publication checks; mark SDP publisher
inventory/signing steps N/A if they do not distribute SDP itself. Keep their own
application/package/installer acceptance checks alongside this list.

## Local XFMD 0.6 dispositions — 2026-10-07

Owner Session 0003 T003 authorizes build/install, not public publication. The installed
executable was observed reporting 0.5 during T003; earlier records had no successful
0.5 installation evidence. Preserve that historical limitation rather than claiming
that the current installed version is still 0.4.
Exact candidate, logs and package/receipt are retained in this work area. Unticked
source template entries are resolved below rather than treated as passed.

| Check | Disposition |
| --- | --- |
| Version | Owner selects 0.6; package 0.6.0; branch/count/+dirty retained |
| Contents | N2 consumer tree cleanup and prior runtime/Mermaid/heading features; N3/N4, SDUI and remaining producer limits excluded and explicit |
| Notes | Canonical Unreleased updated; no public release date or freeze claimed |
| Release log | N/A for unpublished local development package |
| SDP publisher inventory/profile/descriptor/signing/upgrade/bootstrap checks | N/A: XFMD does not distribute SDP itself |
| CI/independent review/clean tree | No clean-tree/independent-approval claim; candidate tests recorded separately |
| Tag/GitHub publication and reconciliation | Not selected or performed |
| gh-sdp pin/default/project upgrade | N/A: no producer distribution or project SDP upgrade |
| XFMD build/package/tests | PASS: Release, extracted binary identity, 88/88 CTest including 42 GUI; exact hashes in README.md |
| XFMD install/backup/smoke | PASS: 168 file hashes, prior 0.5 backup, installed native smoke; receipt retained |
