# N15a — deferred discovery producer

Session0007 T011, KB-XFMD-025, PLAN-XFMD-0009.

## Contract and candidate

See [selected design](../../../../04--Design/SDL-Navigation/N15-Deferred-Discovery.md)
and [candidate hashes](candidate.json). Producer branch `sdp/xfmd-lazy-discovery`
extends the N5–N13 stack. `discover --navigation lazy-sdl` preserves source
inventory and validation while deferring SDL graphs. `tree --model ID --revision
HASH --model-only` retrieves a complete revision-bound model. The JSON actions
share these services. Default eager discovery remains compatible.

## Reproducible verification

- `go -C SDPTool test ./...`: passed; xfmd-n15-sdptool-tests-final.log.
- `go -C SDPTool vet ./...`: passed; xfmd-n15-vet.log (empty success output).
- `python3 Toolkit/scripts/validate_sdp.py --repo "$PWD"` in producer: passed;
  xfmd-n15-docs.log.
- Focused tests: xfmd-n15-focused-pass.log. Aggregate fixture expands 22,236
  nodes across twelve retained models; initial graph has 31 nodes. Covers payload
  parity, retained invalid states, composed-source freshness and CLI/JSON schemas.
- Native configure/build: configure.log/build.log. `ctest --test-dir build-ui
  -R 'Sdp.*Test|SdlSourceGuiTest' --output-on-failure -j4`: twelve pass, six GUI;
  native-compatibility.log. This proves existing eager consumer compatibility.
- Actual checkout service probe: probe.py and actual-project/summary.json plus
  full replies/stderr. Legacy eager fails; lazy succeeds with all 63 sources,
  44 models, 41 deferred models and 3,246 nodes. Separate XFMD model expansion
  succeeds with 2,466 nodes and eleven overview targets. Timings are single-run
  observations, not performance guarantees. Replay against the recorded model
  revision; the growing checkout can have a different source count later.

## Limits and follow-up

N15a does not change the native XFMD workflow. N15b must negotiate capabilities,
fetch on expansion, validate and merge atomically, preserve source/document state
and invalidate stale responses. Source scanning/parsing, per-model, filesystem,
byte and native row guards remain. KB025 remains in-progress. Installed XFMD 0.8
and gh sdp are unchanged. Same-context verification, no independent approval.
No authored SDL/SDUI changes, main merge, release or installation.

Preflight is retained in preflight.json. Shared dirty trees were preserved.
Initial failing runs are retained: fixture ordering was corrected, and the
compiled catalogue test now checks all six action identities after adding the
new action. Final tests pass; no removed behavior obligation.

Producer commit: `e194721bbad9eaba4713ffb3d692c9734de739ee`. Committed-tree main and PG merge forecasts
are clean; see integration-forecast.json. This is not a merge or independent review.
