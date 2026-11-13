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

Current status: published and publicly verified. Exact candidate
8546e956d1a941ece9195d335b7196abea905289 produced
xfmd-0.10.0-76.el10_2.x86_64.rpm. The signed stable package passed authenticated
public predecessor upgrade and fresh install, exact app/private producer identity
and actual native workflows. [Publication receipt](evidence/published-76/publication-receipt.json)
and [public DNF/native log](evidence/published-76/public-dnf.log) retain evidence.


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


## Actual publication

[Signed RPM](https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/stable/x86_64/Packages/xfmd-0.10.0-76.el10_2.x86_64.rpm).
Unsigned SHA256: e7e39924edaad957355e40210f1b8a7e3989ca1d920dbf107e556588c90af563.
Signed SHA256: 8c5f2232d87c043bbae16b49e9fbe3815237d69fdb858efb7adb2d51f3587f2d.
Trusted fingerprint: 8E0E49A101DDF1C7C15532FCCB7BB881901AF029.
Public verification completed 2026-11-12T23:57:27.629079Z (2026-11-13 in Europe/Oslo);
server Last-Modified is separately recorded in the receipt. The successful package
was published through verified-existing without rebuilding its reviewed bytes.

The public log records an actual 0.9.0-71 to 0.10.0-76 upgrade with package and
repository metadata signature checks forced on, then removal and a fresh signed
public install. Both run exact app/producer identity, default native helper lookup,
native reading/render/source/Back and real Planned Activity workflows. The private
producer binary hash is d583f22669339e6a322c795e353c58cd7bd6be1ba650e066e05609b7352f036a.
Coordinating independent review approved the exact source/archive/package before
publication; actual public results were returned for final reconciliation.
