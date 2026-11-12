# ReleaseChecklist — XFMD 0.10 candidate

Copy this checklist into the selected release's work area. Record version,
previous release, exact commit, owner publication authority, evidence paths and
actual outcomes. Unchecked means pending; use N/A only with a reason. A generated
release log is not proof of publication. The project release contract governs.

## Candidate and authority

Owner selected XFMD 0.10 and explicitly authorized feature-branch RPM publication
under ACTIVITY-SDP-0003 / PLAN-SDP-0028. No main merge is selected. Predecessor:
signed public xfmd-0.9.0-71.el10_2.x86_64.rpm at 8893a32. Final consumer candidate
is pending; matching producer is 5802355a8af6a89f9ea3fc035198c32d46d35567. Development evidence is
Reading-001; none of its checks establish release-candidate publication.

The SDP publisher inventory/signing section below is inherited: this package
ships a private standalone helper, not a new signed Toolkit installation release.
Those items require explicit N/A dispositions at closeout; they are not passed
Toolkit gates. Application RPM/signature/public-DNF gates below remain pending.

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


## XFMD RPM acceptance

- [ ] Exact committed producer Git archive pin/digest and embedded BuildRevision.
- [ ] Clean target-native build with required matching producer and native reading CTest.
- [ ] Source and module license inventory, binary digest and package provenance.
- [ ] Exact signed 0.9 predecessor install followed by local candidate upgrade.
- [ ] Unprivileged installed native startup and default private helper invocation.
- [ ] Reinstall, remove/unrelated-file preservation and fresh install.
- [ ] Coordinating independent review of the final integrated source candidate.
- [ ] Server signing, public matching payload download and trusted-key signature.
- [ ] Public signed metadata/package checks and actual DNF 0.9-to0.10 upgrade.
- [ ] Exact app and producer identities after public upgrade and fresh public install.
- [ ] Truthful final notes, hashes, time and publication evidence after success.
