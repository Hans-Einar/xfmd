# XFMD 0.10 exact producer package candidate

Owner authorization: ACTIVITY-SDP-0003 / PLAN-SDP-0028 permits a feature-branch
RPM publication and explicitly excludes a main merge. The native milestone is
d530aa7; final source qualification and publication remain distinct steps.

Matching producer: 5802355a8af6a89f9ea3fc035198c32d46d35567. The committed
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
