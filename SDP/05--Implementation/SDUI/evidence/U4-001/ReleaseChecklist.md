# ReleaseChecklist — XFMD 0.7 local installation

Owner Session0004 T002 authorizes build/install 0.7; predecessor installed 0.6.
Candidate HEAD 41fa4948c5377edd792ba3f503e6b92b0fd4717a plus dirty source archive
identified in candidate.json. Evidence and exclusions: README.md. Local consumer
application delivery only; no SDP distribution or public publication selected.

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

- [x] N/A — XFMD does not distribute the SDP process.  Check Template/sdp-root and project-root guidance. Include Sessions guide and
  blank template, never project conversations, boards or existing work records.
- [x] N/A — XFMD does not distribute the SDP process.  Review SDPTool/profiles/payload.json: every distributed file, destination,
  ownership and removed/moved path. Inventory completeness tests must pass.
- [x] N/A — XFMD does not distribute the SDP process.  Review SDPTool/profiles/five-phase.json: capabilities, schema/profile versions
  and exact supported predecessor descriptor digests, including current stable.
- [x] N/A — XFMD does not distribute the SDP process.  Reconcile SDP.manifest.yaml, Toolkit/SDP-install.manifest.json compatibility
  metadata, package version, release record and notes. Retained legacy metadata
  does not replace the current Go inventory or signed descriptor.
- [x] N/A — XFMD does not distribute the SDP process.  Build package and immutable sdp-release.json from a clean exact commit using
  the Go profile builder. Verify file hashes, sourceCommit and executable identity.
  Never edit the built descriptor or use a project-local manifest as authority.
- [x] N/A — XFMD does not distribute the SDP process.  Sign descriptor with the trusted publisher key; verify signature, binary hash
  and SHA256SUMS. Store no private keys or secrets in evidence/repository.
- [x] N/A — XFMD does not distribute the SDP process.  Rehearse install and each supported upgrade from original verified descriptor
  bytes. Check new Sessions files, existing Session/document preservation, receipt,
  Maintenance entry/history, conflict handling and repeated no-op.
- [x] N/A — XFMD does not distribute the SDP process.  Test extracted-source operation, packaged signed child and normal bootstrap.
  Development artifacts or test signers are not production signed evidence.

## Publish and consumer readiness

- [x] N/A — no public SDP/application publication or project upgrade selected.  Exact candidate CI and required independent review pass; working tree clean;
  tag absent; merge/publication explicitly authorized.
- [x] N/A — no public SDP/application publication or project upgrade selected.  Publish annotated tag and GitHub Release with generated log, signed descriptor,
  executable and checksums; verify downloaded assets against exact release.
- [x] N/A — no public SDP/application publication or project upgrade selected.  Reconcile actual tag/commit/time and append truthful release events only after
  success. Do not rewrite a released log or silently correct released notes.
- [x] N/A — no public SDP/application publication or project upgrade selected.  Verify gh-sdp selection: its compiled default is pinned. Either publish/update
  the client default or document an explicit SDP_RELEASE selecting this release.
- [x] N/A — no public SDP/application publication or project upgrade selected.  Provide preview/apply commands and expected receipt. Owner-requested manual
  project upgrade remains manual; never confuse updating the gh extension with
  upgrading the project's SDP directory.

Consuming projects reuse the scope/content/publication checks; mark SDP publisher
inventory/signing steps N/A if they do not distribute SDP itself. Keep their own
application/package/installer acceptance checks alongside this list.

## Actual consumer acceptance

- [x] Owner explicitly selects 0.7; CPack maps this to 0.7.0.
- [x] Selected SDUI scope and residual KB029/SDL work enumerated in README.md.
- [x] 89/89 CTest, producer race/facade tests, native/Fyne/PDF inspection pass.
- [x] Exact source archive and binary/package hashes retained; dirty tree disclosed.
- [x] Canonical dated notes and generated 0.7.0 release log check pass.
- [x] Extracted binary version and both bundled helper hashes verified.
- [x] Install backup/rollback, 210 file hashes and exact receipt verified.
- [x] Installed Mermaid and SDUI/Fyne smoke checks pass in isolated Xvfb/HOME.
- [x] Compatibility: existing discovery/SDL adapter retained; no model grammar,
  authored source, gh extension or project-SDP upgrade performed.
- [x] Review disposition recorded: implementation self-verification only. No
  independent reviewer or owner acceptance is fabricated; KB027 is gate-review.
- [x] Publication null. No clean-tree/tag/signature/CI/upgrade gate claimed passed.
