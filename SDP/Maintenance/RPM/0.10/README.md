# XFMD 0.10 exact producer package candidate

Owner authorization: ACTIVITY-SDP-0003 / PLAN-SDP-0028 permits a feature-branch
RPM publication and explicitly excludes a main merge. The native milestone is
d530aa7; final source qualification and publication remain distinct steps.

Matching producer: c3463b78d53ebf984b0f553ebe93a556fb3ac6a8. The committed
cmake/sdptool-source.json identifies its exact Git archive/digest. The producer
is built within the AlmaLinux 10.2 target builder, with its actual BuildRevision,
normalized binary digest, sources and dependency/Go notices retained.

The clean candidate pipeline runs all configured CTests including actual native
reading qualification against that binary; verifies local signed 0.9 predecessor
upgrade, reinstall/remove and fresh install; and records exact source/RPM hashes.
The caller can first build without publication, complete coordinating independent
review, then publish those same verified bytes through --publish-existing with
the exact --ref and --output. The retry path rechecks clean source, pinned producer,
application/provenance and RPM hash and does not rebuild a successful candidate.

Publication signs on Beinerud, checks the downloaded payload/signature, installs
the signed public predecessor, performs authenticated public DNF upgrade, verifies
exact application/producer identities and native default helper invocation, then
checks a fresh authenticated public install. No publication evidence is claimed
until these checks actually pass. [ReleaseChecklist](ReleaseChecklist.md) owns gates.

Current status: machinery and source pin prepared. Candidate build and independent
review pending. Build/output identity will be recorded after the source commit.


## Candidate 75 outcome and corrected successor

bdd0602/0.10.0-75 did not produce a publishable package: 88 of 89 target-container
CTests passed. StandaloneMermaidGuiTest queried a file immediately after a native
filter click, while WorkspacePanel deliberately debounces 200 ms before an async
refresh. A bounded wait for the actual file row preserves the existing activation
assertion. The targeted corrected test passes. No native product behavior changed.

Host 90 vs target 89 is an observed environment difference: target omits
FullscreenTest (Window Maker absent) and ThemePopupGuiTest (XTEST development
library absent), and adds SdpReadingGuiTest against the pinned producer. The
reading workflow passed in the failed candidate. No omitted test is claimed passed.

The successor pins the reviewed generic Activity README correction c3463b7.
Installed validation now runs exact-candidate native reading/render/source/history
and real Activity fixture probes against the installed private producer, FOX and
resources. These compiled test tools are retained outside the RPM and hashed;
actual /usr/bin/xfmd native startup/default helper lookup remains separately traced.
Only the disposable verification project is mutated. Both local and signed-public
checks run these workflows. Publication remains on hold until successor verification
and coordinating release review succeed.
