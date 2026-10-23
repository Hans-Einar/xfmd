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

## MM-001 actual dispositions — 2026-10-04

Scope: local XFMD 0.4 / CPack 0.4.0 candidate, not a public version release.
Previous installed product: owner-tested 0.4, receipt 20261001T194155Z from SN-004.
Candidate HEAD remains 41fa4948c5377edd792ba3f503e6b92b0fd4717a + dirty; exact
source/binary/package identities and logs are in [MM-001](evidence/MM-001/README.md).
Owner T002 authorizes build; T003 adds heading links. Publication authority: none.
The copied unchecked template items are resolved below, not silently passed.

| Template check | Actual disposition |
| --- | --- |
| Scope 1: version | PASS: retain owner 0.4; package 0.4.0, branch/count/+dirty distinct |
| Scope 2: contents/review | Local implementation/tests/links PASS; independent review pending; authored model updates held by owner |
| Scope 3: notes | Unreleased entries updated with KB-XFMD-023/024. Dated freeze pending an actual release selection; no false release date |
| Scope 4: release-log | N/A to this unversioned local build artifact. Canonical notes remain Unreleased; no generated publication log claimed |
| Distribution 1: template guidance | N/A: XFMD does not distribute SDP |
| Distribution 2: payload inventory | N/A: XFMD does not distribute SDP |
| Distribution 3: upgrade profiles | N/A: XFMD does not distribute SDP |
| Distribution 4: signed compatibility metadata | N/A: XFMD does not distribute SDP |
| Distribution 5: Go builder/descriptor | N/A: CMake/CPack XFMD candidate; no SDP descriptor and no clean-commit claim |
| Distribution 6: publisher signing | N/A: unsigned local application archive; SHA256 checked, no signature claim |
| Distribution 7: SDP install/upgrade rehearsal | N/A: no SDP installation payload |
| Distribution 8: signed bootstrap | N/A: no SDP bootstrap; extracted XFMD startup checked separately |
| Publish 1: CI/review/clean tree/authority | PENDING for any public release. Local CTest passes; independent review absent and shared tree dirty |
| Publish 2: tag/GitHub/assets | NOT PERFORMED: public release not selected |
| Publish 3: actual publication events | N/A: no publication occurred; no release success event written |
| Consumer 1: gh-sdp pinned default | N/A: this package does not publish SDPTool |
| Consumer 2: project upgrade receipt | N/A: this task neither installs XFMD nor upgrades project SDP |
| XFMD build/package | PASS: optimized Release, extracted binary hash match, system libraries present |
| XFMD acceptance | PASS: 85 CTest checks, raw Mermaid native/PDF and heading-link tests; owner acceptance remains pending |

## T004 installation — 2026-10-05

Owner authorized installation of the prepared candidate. [MM-002](evidence/MM-002/README.md)
records checksum-verified package installation, all-file hash verification, backup
and installed native startup. Prior no-install statements describe T003's boundary;
T004 supersedes that boundary only. Review/acceptance remains pending; no public
release or product/model source change occurred.
