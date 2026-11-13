# XFMD0.11 shared-runtime release work

Scope: ACTIVITY-SDP-0004 / PLAN-SDP-0029 C3–C5, coordinated by SESSION-SDP-0021.
Local progress: [Session0011](../../../Sessions/session-%230011--Shared-tool-consumer.md).
Prior public release: signed 0.10.0-76.el10_2; its immutable receipt remains in0.10.
[Checklist](ReleaseChecklist.md) records current gate disposition.

## Published and independently reviewed — 2026-11-13

| Identity | Value |
| --- | --- |
| Exact application source | 16e57a34d5b0dd5f9f1ba8c49d96ab2fbe60048f |
| Published RPM | xfmd-0.11.0-81.el10_2.x86_64.rpm |
| Unsigned RPM SHA256 | 2be51524843ac67c0b64ac405b1aff918d8340b1b0a3a3781365ffb881f42aad |
| RPM payload digest | 63838a0301db38347f569e736d37395c397a467dbe560ea0bd6adc46db828803 |
| External engine | 2.3.0 / 774e150774d184d0604ae78f274586eeb2bad495 |
| External engine SHA256 | 2cfcf12f8bfb0d6fa8d11391db874f68d9bc9210c85974ff3cecee50f915ced6 |
| Exact output | /tmp/xfmd-activity4-rpm-final-81 |
| Independent artifact review | Approved local native technical gate; [receipt](evidence/qualified-81/review-receipt.json) |
| Server signature/public DNF | Passed; signed public upgrade and fresh install, each with all three offline native probes |
| Signed public RPM SHA256 | 7d1cb2a6f04064d5600409afd3725d971118fd27bb4d9ec45163847d949afd74 |
| Public URL | https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/stable/x86_64/Packages/xfmd-0.11.0-81.el10_2.x86_64.rpm |
| Publication receipt observed | 2026-11-13T09:38:38.016063+00:00 (receipt file time; exact server publication instant not recorded) |

The RPM contains no SDP engine/helper payload or producer source pin. It declares
OS libraries used by the shared engine's native SDUI host. Runtime compatibility
uses sdptool/0.2 plus runtime.setup/1 and sdui.source/1, independently of source or
version labels. A development engine without its embedded host may browse while
prototype launch explains the missing host; explicit development overrides remain.

All 95 configured AlmaLinux 10.2 x86_64 CTests pass, including all real-engine tests.
[Reconciliation](Test-contract-reconciliation.md) explains the five stale optional
test assumptions corrected to the Activity0003 opaque reading contract; no product
code was changed to satisfy them. Failed and aborted earlier outputs remain
separate trials and are not qualified evidence.

Clean target verification installs the exact signed 0.10 predecessor, upgrades to
this candidate and proves the old private engine is absent. Actual setup registers
an exact copy of the shared engine inside isolated HOME. With gh absent, auth
variables removed and an isolated network namespace rejecting external sockets,
all three exact native probes pass: reading/render/source/Back, SDUI preview/source/
check/launch/lifetime/overrides and a real Activity fixture. Reinstall, removal,
unrelated-file/shared-engine preservation and fresh install pass. These are
container tests, not an owner workstation installation or physical usability claim.

[Qualification receipt](evidence/qualified-81/qualification-receipt.json),
[CTest](evidence/qualified-81/ctest.log),
[installed verification](evidence/qualified-81/verify.log) and
[RPM inspection](evidence/qualified-81/rpm-inspection.json) retain observed evidence.
The verified-existing guard rechecks clean exact source, package/engine/probe hashes
and application identity before any publication. Evidence-only commits do not
change the qualified source/package identity above.

Executed publication command after coordinator authorization and shared public gates:

```sh
python3 packaging/rpm/local-ci.py --ref 16e57a34d5b0dd5f9f1ba8c49d96ab2fbe60048f --output /tmp/xfmd-activity4-rpm-final-81 --publish-existing --host beinerud
```

That path uploads existing reviewed bytes, verifies the signed matching payload,
then forces package/metadata signature checks during actual public DNF0.10-to0.11
upgrade and fresh install, repeating the installed native workflows. The [actual publication receipt](evidence/published-81/published.json),
[trusted signature output](evidence/published-81/signature.txt) and
[public DNF log](evidence/published-81/public-dnf.log) prove successful publication
and installed checks; local qualification remains a separate evidence layer.


The [coordinating independent review snapshot](evidence/qualified-81/coordinating-independent-review.md)
approves this exact local native technical gate. Its earlier superseded candidates
remain historical; the final native section identifies source16e57a3/RPM81 and
engine774e150. This approval does not bypass canonical exact-source CI, shared
public availability/install/update checks or the explicit RPM publication gate.

## Public delivery closeout

[Final independent review](evidence/published-81/independent-review.md) approves
combined technical delivery with no material finding. Actual published signed
bytes preserve the reviewed unsigned payload. Fresh-database RPM trust uses
fingerprint `8E0E49A101DDF1C7C15532FCCB7BB881901AF029`. Public DNF enables
package and repository-metadata signature checks, upgrades actual signed0.10 to
0.11, then removes and freshly installs0.11. Both installations repeat Reading,
SDUI and Activity probes without gh/auth/external networking and confirm the
registered engine774e150. All95 CTests are build qualification, not a claim that
95 tests reran on the public installation. Canonical/extension v2.3.0 and signed
public-default Framework checks passed before successor publication. Evidence
receipts are preserved alongside [reconciliation timing](evidence/published-81/reconciliation.json).

No owner acceptance, workstation upgrade, model completion or main merge is
claimed. The live gh extension remains0.2.3. Historical0.10 evidence is unchanged.
