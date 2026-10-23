# XFMD 0.4 — Scoped delivery checklist

Copy this checklist into the selected release's work area. Record version,
previous release, exact commit, owner publication authority, evidence paths and
actual outcomes. Unchecked means pending; use N/A only with a reason. A generated
release log is not proof of publication. The project release contract governs.


Session T006, 2026-10-01. Scope: owner-authorized local development installation,
not a public release. Previous installed product: 0.3, SN-003. Exact dirty candidate,
commands, hashes and receipt are in README.md and candidate.json beside this file.
The existing docs/versioning.md contract governs. SDL/SDUI source changes remain
held by T005. No new release automation or publication workflow is adopted here.

## Scope and content

- [x] Select product version from compatibility impact. Keep language/profile, Evidence: owner T006, VERSION and docs/versioning.md.
  process/management schema and client versions distinct.
- [ ] Enumerate included work and remaining exclusions; validate selected model, Local content/links/tests pass; independent review and model reconciliation remain pending under the existing Session.
  tests, review dispositions and links on the actual candidate.
- N/A — local development delivery; no canonical release-note/generator workflow adopted in this project. Version decision and delivery evidence use docs/versioning.md and SN-004: Update canonical RELEASE-NOTES.md Unreleased entries with stable work IDs.
  Freeze selected entries into a dated version section; preserve released bytes.
- N/A — local development delivery; no canonical release-note/generator workflow adopted in this project. Version decision and delivery evidence use docs/versioning.md and SN-004: Run `sdptool release-log --version X.Y.Z --output Releases/X.Y.Z.md`.
  Check with the same command plus --check. Use --all --output Releases --check
  to check every recorded version. Generator extracts reviewed notes, not Git guesses.

## Installation distribution (SDP publisher)

- N/A — XFMD does not distribute SDP: Check Template/sdp-root and project-root guidance. Include Sessions guide and
  blank template, never project conversations, boards or existing work records.
- N/A — XFMD does not distribute SDP: Review SDPTool/profiles/payload.json: every distributed file, destination,
  ownership and removed/moved path. Inventory completeness tests must pass.
- N/A — XFMD does not distribute SDP: Review SDPTool/profiles/five-phase.json: capabilities, schema/profile versions
  and exact supported predecessor descriptor digests, including current stable.
- N/A — XFMD does not distribute SDP: Reconcile SDP.manifest.yaml, Toolkit/SDP-install.manifest.json compatibility
  metadata, package version, release record and notes. Retained legacy metadata
  does not replace the current Go inventory or signed descriptor.
- N/A — XFMD does not distribute SDP: Build package and immutable sdp-release.json from a clean exact commit using
  the Go profile builder. Verify file hashes, sourceCommit and executable identity.
  Never edit the built descriptor or use a project-local manifest as authority.
- N/A — XFMD does not distribute SDP: Sign descriptor with the trusted publisher key; verify signature, binary hash
  and SHA256SUMS. Store no private keys or secrets in evidence/repository.
- N/A — XFMD does not distribute SDP: Rehearse install and each supported upgrade from original verified descriptor
  bytes. Check new Sessions files, existing Session/document preservation, receipt,
  Maintenance entry/history, conflict handling and repeated no-op.
- N/A — XFMD does not distribute SDP: Test extracted-source operation, packaged signed child and normal bootstrap.
  Development artifacts or test signers are not production signed evidence.

## Publish and consumer readiness

- N/A — no public release or SDP client/project upgrade in this assignment: Exact candidate CI and required independent review pass; working tree clean;
  tag absent; merge/publication explicitly authorized.
- N/A — no public release or SDP client/project upgrade in this assignment: Publish annotated tag and GitHub Release with generated log, signed descriptor,
  executable and checksums; verify downloaded assets against exact release.
- N/A — no public release or SDP client/project upgrade in this assignment: Reconcile actual tag/commit/time and append truthful release events only after
  success. Do not rewrite a released log or silently correct released notes.
- N/A — no public release or SDP client/project upgrade in this assignment: Verify gh-sdp selection: its compiled default is pinned. Either publish/update
  the client default or document an explicit SDP_RELEASE selecting this release.
- N/A — no public release or SDP client/project upgrade in this assignment: Provide preview/apply commands and expected receipt. Owner-requested manual
  project upgrade remains manual; never confuse updating the gh extension with
  upgrading the project's SDP directory.

Consuming projects reuse the scope/content/publication checks; mark SDP publisher
inventory/signing steps N/A if they do not distribute SDP itself. Keep their own
application/package/installer acceptance checks alongside this list.

## XFMD local installation checks

- [x] Build 0.4 and validate embedded version against build metadata.
- [x] Run targeted version/native SDP regressions on this candidate; retain SN-003
  broader evidence only where product sources are unchanged.
- [x] Stage CMake installation; preserve previous files, replace executable last
  and compare installed executable and metadata with the tested build.
- [x] Smoke-test installed binary in isolated Xvfb/temp HOME; record receipt/backup.
- [x] Update Session and version decision; preserve model source hashes and staged #020.

The pending independent review/model work prevents treating this checklist as a
passed public-release gate. It does not block the explicitly authorized local install.
