# ReleaseChecklist — XFMD 0.8 local installation

Copy this checklist into the selected release's work area. Record version,
previous release, exact commit, owner publication authority, evidence paths and
actual outcomes. Unchecked means pending; use N/A only with a reason. A generated
release log is not proof of publication. The project release contract governs.

## Scope and content

- [x] Select product version from compatibility impact. Keep language/profile,
  process/management schema and client versions distinct.
- [x] Enumerate included work and remaining exclusions; validate selected model,
  tests, review dispositions and links on the actual candidate.
- [x] Update canonical RELEASE-NOTES.md Unreleased entries with stable work IDs.
  Freeze selected entries into a dated version section; preserve released bytes.
- [x] Run `sdptool release-log --version X.Y.Z --output Releases/X.Y.Z.md`.
  Check with the same command plus --check. Use --all --output Releases --check
  to check every recorded version. Generator extracts reviewed notes, not Git guesses.

## Installation distribution (SDP publisher)

- [x] N/A — consuming application; no SDP distribution.  Check Template/sdp-root and project-root guidance. Include Sessions guide and
  blank template, never project conversations, boards or existing work records.
- [x] N/A — consuming application; no SDP distribution.  Review SDPTool/profiles/payload.json: every distributed file, destination,
  ownership and removed/moved path. Inventory completeness tests must pass.
- [x] N/A — consuming application; no SDP distribution.  Review SDPTool/profiles/five-phase.json: capabilities, schema/profile versions
  and exact supported predecessor descriptor digests, including current stable.
- [x] N/A — consuming application; no SDP distribution.  Reconcile SDP.manifest.yaml, Toolkit/SDP-install.manifest.json compatibility
  metadata, package version, release record and notes. Retained legacy metadata
  does not replace the current Go inventory or signed descriptor.
- [x] N/A — consuming application; no SDP distribution.  Build package and immutable sdp-release.json from a clean exact commit using
  the Go profile builder. Verify file hashes, sourceCommit and executable identity.
  Never edit the built descriptor or use a project-local manifest as authority.
- [x] N/A — consuming application; no SDP distribution.  Sign descriptor with the trusted publisher key; verify signature, binary hash
  and SHA256SUMS. Store no private keys or secrets in evidence/repository.
- [x] N/A — consuming application; no SDP distribution.  Rehearse install and each supported upgrade from original verified descriptor
  bytes. Check new Sessions files, existing Session/document preservation, receipt,
  Maintenance entry/history, conflict handling and repeated no-op.
- [x] N/A — consuming application; no SDP distribution.  Test extracted-source operation, packaged signed child and normal bootstrap.
  Development artifacts or test signers are not production signed evidence.

## Publish and consumer readiness

- [x] N/A — local development install only; public CI/review/signing/publication/upgrade gates not claimed.  Exact candidate CI and required independent review pass; working tree clean;
  tag absent; merge/publication explicitly authorized.
- [x] N/A — local development install only; public CI/review/signing/publication/upgrade gates not claimed.  Publish annotated tag and GitHub Release with generated log, signed descriptor,
  executable and checksums; verify downloaded assets against exact release.
- [x] N/A — local development install only; public CI/review/signing/publication/upgrade gates not claimed.  Reconcile actual tag/commit/time and append truthful release events only after
  success. Do not rewrite a released log or silently correct released notes.
- [x] N/A — local development install only; public CI/review/signing/publication/upgrade gates not claimed.  Verify gh-sdp selection: its compiled default is pinned. Either publish/update
  the client default or document an explicit SDP_RELEASE selecting this release.
- [x] N/A — local development install only; public CI/review/signing/publication/upgrade gates not claimed.  Provide preview/apply commands and expected receipt. Owner-requested manual
  project upgrade remains manual; never confuse updating the gh extension with
  upgrading the project's SDP directory.

Consuming projects reuse the scope/content/publication checks; mark SDP publisher
inventory/signing steps N/A if they do not distribute SDP itself. Keep their own
application/package/installer acceptance checks alongside this list.

## Actual consumer acceptance

Owner Session0005 T011 authorizes local installation of 0.8 over 0.7. Exact
candidate and dirty-tree status: candidate.json; contents and exclusions: README.md.

- [x] Version, executable, manpage/package and build identity agree (0.8/0.8.0).
- [x] Full 91/91 CTest, including 44 GUI tests, passes on the selected build.
- [x] Package 210-file inventory and application/private-helper hashes verified.
- [x] Installed 210 hashes equal package; backup, timestamp and receipt retained.
- [x] Installed Mermaid and SDUI/Fyne startup verified under isolated Xvfb/HOME.
- [x] Canonical 0.8 notes and all generated release sections checked; prior 0.7 log unchanged.
- [x] Authored sources, gh sdp and project SDP profile unchanged; ongoing scope remains open.
- [x] Same-context verification only; no independent review or public publication claimed.
