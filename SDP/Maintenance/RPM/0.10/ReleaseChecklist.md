# ReleaseChecklist — XFMD 0.10 published RPM

Instantiated from SDP/Framework/ReleaseChecklist.md. Owner publication authority:
ACTIVITY-SDP-0003 / PLAN-SDP-0028. Exact reviewed application source:
8546e956d1a941ece9195d335b7196abea905289; exact private producer:
c3463b78d53ebf984b0f553ebe93a556fb3ac6a8. Predecessor: signed public
xfmd-0.9.0-71.el10_2.x86_64.rpm. Selected surface is the signed Beinerud stable RPM
repository; no main merge is selected. [Receipt and evidence](evidence/published-76/publication-receipt.json).

## Scope and content

- [x] Owner-selected minor feature delivery 0.10; RPM 0.10.0-76.el10_2. Existing
  language/profile and client schema identities remain distinct from this app version.
- [x] Included Activity2 consumer and Activity3 producer-owned reading, exact
  private producer and native/package plumbing. 89 configured target CTests pass;
  native HEOS/fixture/PDF and coordinating independent review support the selected
  behavior. Existing explicit source/graph/display bounds remain declared.
- [x] Canonical RELEASE_NOTES.md records the actual publication with stable work IDs
  and evidence links, preserving earlier sections.
- [x] N/A — sdptool release-log extracts Toolkit release notes; this application RPM
  uses the exact publication receipt and repository evidence instead.

## Installation distribution (SDP publisher)

- [x] N/A — Template/project-root guidance and Sessions inventory: no Toolkit
  installation distribution is published by this application release.
- [x] N/A — SDPTool/profiles/payload.json inventory: the private CLI is a pinned
  executable, not a newly published Toolkit installation payload.
- [x] N/A — five-phase profile/predecessor descriptor compatibility: no SDP project
  install or profile upgrade is selected.
- [x] N/A — Toolkit manifests/compatibility descriptors: no Toolkit release identity
  is changed or asserted. Private source/binary identity is independently recorded.
- [x] N/A — Go profile-builder sdp-release.json: application RPM provenance,
  executable/source hashes and clean candidate metadata are checked instead.
- [x] N/A — SDP descriptor signing: RPM/download/repository signatures are verified
  under Beinerud's existing key; no private keys enter the evidence.
- [x] N/A — SDP install/upgrade receipt rehearsal: actual application predecessor
  upgrade, reinstall/removal/fresh install and preservation checks pass instead.
- [x] N/A — Toolkit extracted-source/bootstrap/signed-child gates: this release
  supplies the matching private executable and sources without a Toolkit upgrade.

## Publish and consumer readiness

- [x] Exact candidate CI and required independent review pass. Clone metadata is
  dirty=false and detached at the reviewed source. Coordinator independently
  approved the exact unsigned SHA; neither tag nor merge is inferred.
- [x] Actual selected publication completed: signed stable RPM, matching payload
  download, trusted-key signature and public DNF transactions. Annotated tag and
  GitHub Release are N/A for the selected application publication surface.
- [x] Actual source/unsigned/signed hashes, server Last-Modified and public
  verification completion time are recorded after success. Traceability is appended.
- [x] N/A — gh-sdp default/SDP_RELEASE changes: installed native lookup selects the
  package's private producer; the separate gh-sdp installation is not upgraded.
- [x] Consumer commands and expected app version are documented. This publishes an
  application RPM, so no project SDP receipt or manual project upgrade is claimed.

## XFMD RPM acceptance

- [x] Exact committed producer Git archive pin/digest and embedded BuildRevision.
- [x] Clean target-native build; native reading CTest uses that matching producer.
- [x] Source, module/Go notices, normalized binary digest and package provenance.
- [x] Exact signed 0.9 predecessor install followed by local candidate upgrade.
- [x] Actual /usr/bin/xfmd unprivileged startup and default private helper invocation.
- [x] Exact-candidate native test tools outside the RPM exercise installed private
  producer/FOX/resources: all eleven landings, render/source/history and real Activity.
- [x] Reinstall, removal/unrelated-file preservation and fresh install pass.
- [x] Coordinating independent review approved source, exact archive and tested bytes.
- [x] Server signing, matching public payload download and trusted-key signature pass.
- [x] Forced package/metadata signature checks and actual public DNF 0.9-to-0.10 upgrade.
- [x] Exact app/producer identities and native workflows pass after public upgrade
  and fresh authenticated public installation.
- [x] Actual notes, hashes, times, receipt and public workflow evidence recorded.

All applicable gates have observed evidence. The 89 target vs 90 host count is
explained in README.md: two environment-dependent GUI tests are absent in the
container and the real producer reading test is added. No omitted test is claimed
passed. Same-context checks are distinguished from the coordinating independent
review in Activity0003/06-Verification/B5/Review.md. Source 8546e95 remains the
published candidate even when later evidence-only commits close records.


Final independent agent release review also checked actual public signature,
payload identity, signed DNF upgrade/fresh install and native workflows, approving
with no findings. Its retained disposition is
[evidence/published-76/coordinating-independent-review.md](evidence/published-76/coordinating-independent-review.md).
