# XM-M2 capability probe evidence — 2026-09-29

Parent: MAINT-XFMD-0004. Architecture: PLAN-XFMD-0002.
Verification: PLAN-XFMD-0005. [Gap register](../../../../02--Requirements/XFMD-Modeling/Gaps.md).

**Result:** all 21 final expectations pass: supported controls/core are accepted,
unsupported constructs are rejected, and the parser/layout boundary is explicit.
This confirms limitations; it does not mean those capabilities are implemented.

Producer source: tracked archive of
`3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`, containing SDL/go and SDUI/go.
Builds use the existing Go 1.27.1 toolchain with `GOWORK=off`,
`GOTOOLCHAIN=local` and `go build -mod=readonly`. The producer's untracked
sourceinput/node/package files are excluded. Nothing is installed or upgraded.
Binary/fixture hashes and exact executed commands are in [probe-results.json](probe-results.json).

Reproduce in a fresh temporary directory (`probe_root`), from a producer checkout
containing that commit. The commands below are instructions, not extra test results:

```sh
git -C /home/warloc/git/SDP-vNow archive 3b4cff695ecab36a0e897052e3f096bbeb9f3ea0 SDL/go SDUI/go | tar -x -C "$probe_root"
mkdir -p "$probe_root/bin"
GOWORK=off GOTOOLCHAIN=local /home/warloc/.local/share/sdp-toolchains/go1.27.1/go/bin/go -C "$probe_root/SDL/go" build -mod=readonly -o "$probe_root/bin/sdl" ./cmd/sdl
GOWORK=off GOTOOLCHAIN=local /home/warloc/.local/share/sdp-toolchains/go1.27.1/go/bin/go -C "$probe_root/SDUI/go" build -mod=readonly -o "$probe_root/bin/sdui" ./cmd/sdui
python3 SDP/05--Implementation/XFMD-Modeling/evidence/XM-M2/run_probes.py --sdl "$probe_root/bin/sdl" --sdui "$probe_root/bin/sdui" --producer-commit 3b4cff695ecab36a0e897052e3f096bbeb9f3ea0
```

The tracked script writes explicit fixtures and captures stdout/stderr. The first
run assumed exit 1 for SDUI rejection; actual CLI diagnostics correctly use exit 2.
[Initial results](initial-probe-results.json) are retained. The harness expectation
was corrected after inspecting CLI behavior; no producer implementation or
expected diagnostic was changed to obtain a pass.

Important distinctions:

- SDL `container` succeeds; proposed `system`, `include` and `requirement` forms
  fail supported-profile checking.
- SDUI controls render SVG; scroll formatting passes AST but fails layout.
- Ten additional widget kinds and icon/checked/context properties are rejected.
- An unresolved callback source passes frontend checks, as documented. This
  demonstrates a link-validation boundary, not a working callback.

No native Fyne/FOX host was run. No visual fidelity, accessibility, binding,
multi-file model composition or independent review is claimed. The observed
SDUI runtime's existing handle/generation protections are source-inspection facts,
not exercised by this frontend probe suite.
