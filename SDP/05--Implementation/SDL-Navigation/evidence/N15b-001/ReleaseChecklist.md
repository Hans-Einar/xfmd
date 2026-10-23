# XFMD 0.9 local development delivery checklist

Owner Session0007 T012 authorizes local build/install. Previous installed XFMD 0.8.
Producer: e194721, 2.2.0-dev. Public release/tag/merge and project process upgrade
are excluded. Exact dirty-source and package hashes are retained in this directory.


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

- [x] N/A — no signed SDP distribution or project installation upgrade. Check Template/sdp-root and project-root guidance. Include Sessions guide and
  blank template, never project conversations, boards or existing work records.
- [x] N/A — no signed SDP distribution or project installation upgrade. Review SDPTool/profiles/payload.json: every distributed file, destination,
  ownership and removed/moved path. Inventory completeness tests must pass.
- [x] N/A — no signed SDP distribution or project installation upgrade. Review SDPTool/profiles/five-phase.json: capabilities, schema/profile versions
  and exact supported predecessor descriptor digests, including current stable.
- [x] N/A — no signed SDP distribution or project installation upgrade. Reconcile SDP.manifest.yaml, Toolkit/SDP-install.manifest.json compatibility
  metadata, package version, release record and notes. Retained legacy metadata
  does not replace the current Go inventory or signed descriptor.
- [x] N/A — no signed SDP distribution or project installation upgrade. Build package and immutable sdp-release.json from a clean exact commit using
  the Go profile builder. Verify file hashes, sourceCommit and executable identity.
  Never edit the built descriptor or use a project-local manifest as authority.
- [x] N/A — no signed SDP distribution or project installation upgrade. Sign descriptor with the trusted publisher key; verify signature, binary hash
  and SHA256SUMS. Store no private keys or secrets in evidence/repository.
- [x] N/A — no signed SDP distribution or project installation upgrade. Rehearse install and each supported upgrade from original verified descriptor
  bytes. Check new Sessions files, existing Session/document preservation, receipt,
  Maintenance entry/history, conflict handling and repeated no-op.
- [x] N/A — no signed SDP distribution or project installation upgrade. Test extracted-source operation, packaged signed child and normal bootstrap.
  Development artifacts or test signers are not production signed evidence.

## Publish and consumer readiness

- [x] N/A — local development installation; no public publication, CI or independent review claim. Exact candidate CI and required independent review pass; working tree clean;
  tag absent; merge/publication explicitly authorized.
- [x] N/A — local development installation; no public publication, CI or independent review claim. Publish annotated tag and GitHub Release with generated log, signed descriptor,
  executable and checksums; verify downloaded assets against exact release.
- [x] N/A — local development installation; no public publication, CI or independent review claim. Reconcile actual tag/commit/time and append truthful release events only after
  success. Do not rewrite a released log or silently correct released notes.
- [x] N/A — local development installation; no public publication, CI or independent review claim. Verify gh-sdp selection: its compiled default is pinned. Either publish/update
  the client default or document an explicit SDP_RELEASE selecting this release.
- [x] N/A — local development installation; no public publication, CI or independent review claim. Provide preview/apply commands and expected receipt. Owner-requested manual
  project upgrade remains manual; never confuse updating the gh extension with
  upgrading the project's SDP directory.

Consuming projects reuse the scope/content/publication checks; mark SDP publisher
inventory/signing steps N/A if they do not distribute SDP itself. Keep their own
application/package/installer acceptance checks alongside this list.

## Local consumer acceptance

- [x] Final native build and complete CTest suite.
- [x] Actual project discovery, deferred expansion and generated VP02 workflow.
- [x] Complete candidate source/binary/package hashes and extracted package check.
- [x] Preserve installed XFMD/SDPTool and helpers as backup; atomic per-file replacement.
- [x] All installed files match verified package; standalone SDPTool matches private copy.
- [x] Installed GUI invokes private producer with lazy-sdl, without environment override.
- [x] Installed raw Mermaid and SDUI/Fyne startup checks.
- [x] gh-sdp binary/default and project receipt unchanged.
- [x] Record actual installation, limitations and Session/card/plan outcome.

Review disposition: implementation self-verification, no independent approval or
owner visual acceptance. Retained KB020/025/028 follow-ups are not closed by this
local delivery. Source model hold remains in force.

All applicable local checks have executed; see README.md, candidate.json,
package.json and installation.json for actual evidence. The public publisher
items explicitly marked N/A are not claims of signature, CI or review approval.
