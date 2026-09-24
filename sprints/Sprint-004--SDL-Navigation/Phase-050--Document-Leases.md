# Phase 050 — broker document leases

Sprint 004. Branch `sprint/004/phase/050-document-leases`, from P049 `d6f75b8`.
Mode: autonomous. Implemented, not fully Verified. UR-044/SR-027/SR-028,
FUNC-031; AT-070/071/072. SR-028/AT-072 were assigned during documentation
reconciliation to make the already implemented lease behavior traceable.

## Milestones and identified build

| Milestone | Commit | Scope |
| --- | --- | --- |
| P050-M1 | 5fa35e3 | Optional broker registration, lease metadata and release lifecycle. |
| P050-M2 | 47a245a | Historical native/broker verification report and screenshot; no application code changes. |

The original report identifies a Release build from 5fa35e3 and three affected
CTest passes (reported wall time 6.38 s), plus a real SDL daemon/mmdr/FOX probe.
The inherited P049 full-suite claim has the [provenance limitation](Phase-049--SDL-Navigation.md)
documented separately; it is not a clean, fully identified P050 full-suite run.

Recovered on 2026-09-24 from this worktree, copied byte-for-byte:

- [Build metadata](evidence/recovered-p050-build.json): clean commit
  5fa35e37a426a165682a2560df9212e5cf240360, number 190, version
  `0.1 sprint/004/phase/050-document-leases:190`.
- [CTest log](evidence/recovered-p050-tests.txt): NavigationGuiTest,
  DocumentViewsGuiTest and DocumentEndpointTest pass on September 22 at 15:21 CEST;
  individual durations 2.29, 3.34 and 0.75 seconds (rounded GUI aggregate 6.37 s).
- [Recovery manifest and SHA-256 hashes](evidence/recovery.json).

The log contains no Git SHA. Its association with the co-located build metadata
and phase report is supporting evidence, not a cryptographic binding. No program
was rebuilt or GUI test rerun to create these copies. The local configure cache
identified Release and SDL_TOOL_EXECUTABLE=/tmp/sdl; the original SDL executable's
source version/hash was not recorded and has not been inferred.

## External broker probe

[verify_native_broker.py at SDP commit b784552](https://github.com/Hans-Einar/SDP/blob/b784552/SystemDesignLanguage/go/tools/verify_native_broker.py)
and [G6's report at that commit](https://github.com/Hans-Einar/SDP/blob/b784552/SystemDesignLanguage/go/evidence/G6.md)
provide the versioned external source/reference. This pins the available script
and report, not proof of the exact binaries used on September 22.

The original report describes a leased SVG open, SIGKILL/restart of the daemon,
retained files and request sequences, invalid SDL preserving the document,
replacement freeing the old bundle and normal WM_DELETE_WINDOW freeing the active
lease. [The original broker image](broker.png) is preserved. No raw broker-run log
or complete daemon/reader/mmdr build manifest was recovered.

## Historical acceptance matrix at documentation reconciliation

| Acceptance | Available evidence | Limit |
| --- | --- | --- |
| AT-070: real click, target and navigator retention | Test source, historical P049 report; recovered P050 GUI log | Original P049 complete tree unknown; no explicit dirty-main failure evidence. |
| AT-071: addressed processes, stale/invalid/oversized input, missing/closed target | DocumentEndpointTest source and recovered pass | Peer-UID failure, connection/sequence bounds and disconnect timing not comprehensively demonstrated. |
| AT-072: broker restart, invalid generation, replacement and normal close | P050/G6 reports, external probe source, screenshot | External executable versions/raw output not preserved. |
| AT-072: retry/saturation and uncertain/crashed release | Current code review only | Needs explicit failure-path execution; not claimed Verified. |

## Release behavior and limits

The broker owns durable bundles, leases and crash recovery. XFMD records the new
lease only after opening succeeds, queues the previous lease on successful document
change, and attempts release on normal shutdown. Failed connect/send is retried
while the window lives; 256 queued releases reject further addressed opens.

Important correction to the earlier broad wording: XFMD removes a queued release
on a successful send, without waiting for a broker acknowledgment. It does not
persist pending releases across its own crash/shutdown. No durable release or
exactly-once reclamation is claimed. Visible broker files have no XFMD expiration;
uncertain leases require the broker's conservative retention/explicit cleanup policy.

## Reproduction recipe for a new identified run

This is an instruction for future execution, not a transcript of the historical run.
Use the README dependencies and record the XFMD source SHA/dirty state and build
metadata, plus source revisions and hashes for SDL, reader, daemon and mmdr. Set
SDL_TOOL to an absolute prebuilt SDL CLI path and SDL_REPO to the checkout containing
the pinned probe (historical paths below). Use a fresh build directory so the
recovered evidence is not overwritten.

```sh
cmake -S . -B build-reconciliation -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DSDL_TOOL_EXECUTABLE="${SDL_TOOL:?Set an absolute SDL CLI path}"
cmake --build build-reconciliation --parallel 4
ctest --test-dir build-reconciliation -N
ctest --test-dir build-reconciliation --output-on-failure -R '^(NavigationGuiTest|DocumentViewsGuiTest|DocumentEndpointTest)$'
```

Check that the real-tool test is actually listed. For the external broker probe,
set SDL_DAEMON, SDL_REQUEST and MMDR to identified absolute executable paths:

```sh
python3 tools/run_with_xvfb.py python3 "${SDL_REPO:?}/SystemDesignLanguage/go/tools/verify_native_broker.py" --xfmd "$PWD/build-reconciliation/xfmd" --daemon "${SDL_DAEMON:?}" --request "${SDL_REQUEST:?}" --renderer "${MMDR:?}" --capture "$PWD/build-reconciliation/broker.png"
```

The probe uses an isolated Xvfb display, its own daemon/window and temporary data;
it requires ImageMagick's import command for capture. Use the pinned checkout or
adapt moved paths explicitly. Preserve command output, build metadata, executable
hashes and fixture versions. Run the full suite/sanitizers only when the selected
verification scope requires them; a focused rerun is not full acceptance.

## New identified evidence — 2026-09-24

[P052's acceptance matrix](../Sprint-006--SDL-Acceptance/Phase-052--SDL-Acceptance.md)
now records 5/5 focused CTest passes and a fresh successful native broker probe,
with archived SDL sources and identified executable hashes. New checks cover
failed-open retention, 256-release admission/recovery, actual connect failure,
injected send failure, both-panel destruction, no acknowledgment and lost shutdown
retry. Historical evidence and its original provenance limits remain unchanged.
Arbitrary crash timing and real different-UID execution remain outside this run;
best-effort release semantics are confirmed, not strengthened.
