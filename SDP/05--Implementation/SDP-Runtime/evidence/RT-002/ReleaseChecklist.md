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

## RT-002 actual dispositions — owner-selected XFMD 0.5

Previous installed candidate: XFMD 0.4, MM-002. Owner Session 0002 T009 selects
0.5 and local installation. Public publication is not authorized. Exact candidate
identities and results are recorded alongside this checklist. Unchecked template
items are resolved below; they are not silently counted as passed.

| Template check | Actual disposition |
| --- | --- |
| Scope 1: version | Owner selects 0.5; CPack 0.5.0; development identity stays separate |
| Scope 2: contents/review | #023/#024 and local #025 increment; native verification and independent review pending; model-source hold preserved |
| Scope 3: notes | Unreleased notes updated; no public release date or freeze claimed |
| Scope 4: release-log | N/A to this unpublished local candidate; no reviewed publication notes frozen |
| Distribution 1–8 | N/A: application package does not distribute SDP, its signed descriptor or bootstrap |
| Publish 1: CI/review/clean tree | Pending; dirty tree and native host restrictions remain |
| Publish 2: tag/GitHub/assets | Not performed or authorized |
| Publish 3: publication reconciliation | N/A: no publication occurred |
| Consumer 1: gh-sdp pinning | N/A: no SDPTool distribution |
| Consumer 2: project SDP upgrade | N/A: no project SDP upgrade |
| XFMD package and identity | Record build/package and extracted identity results in README.md |
| XFMD installation | Authorized, but host write policy excludes /home/warloc/.local; no installed receipt claimed |
| XFMD GUI acceptance | Pending: previous runtime retry failed before application assertions; old 0.4 evidence is not exact-candidate acceptance |
