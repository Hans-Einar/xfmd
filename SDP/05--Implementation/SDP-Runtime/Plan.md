# PLAN-XFMD-0008 — Runtime storage and demand-driven SDP navigation

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0008 |
| project | XFMD |
| state | active |
| PlanType | ImplementationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| Source cards | KB-XFMD-025 |
| Session | SESSION-XFMD-0002 T007 |

## Authority and outcome

Owner 2026-10-06 requests fixing runtime-directory storage and on-demand generation
before starting a new Session or #026. Implement the available XFMD boundary now.
Use the current shared dirty branch; preserve prior candidate evidence. Git is
read-only in this host, so milestone commits cannot be made. Upstream SDP-vNow and
installed binaries are also read-only; no circumvention, install or upstream fix
is claimed. #025 remains open for producer and other cutoff reconciliation.

## Selected design before code

- Allocate per-window request directories lazily only when a concrete generated
  view is requested. Use XDG_RUNTIME_DIR when set and nonempty, otherwise
  /run/user/<uid>. Require an absolute real user-owned mode-0700 directory and
  private xfmd child; create unique xfmd-sdp-* workspaces below it. Do not silently
  fall back to disk-backed /tmp. Report allocation/configuration failure and
  preserve the document. Respect a valid host-supplied XDG runtime location;
  do not hardcode UID 1000 or claim every host's runtime directory is tmpfs.
- SDP discovery/refresh and opening a parent viewpoint must not generate its
  diagrams. Activate a viewpoint with children by expanding its native tree row;
  diagram/focus leaves and explicit generated links retain typed producer actions.
  A viewpoint with no children keeps its existing target, since there is no
  discoverable detail list to substitute. No invented producer URI semantics.
- Remove the 64-context/256-MiB accumulated-retention admission stop. Keep current
  and history-referenced bundles; remove unreachable bundles only when preview
  and export workers are idle. Preserve distinct immutable snapshots and captured
  revision/context links. Do not change existing history length or silently
  regenerate an old revision against changed source.
- Retain per-result validation and existing producer limits in this increment;
  this is not a claim that #025's complete no-cutoff goal is achieved. Its separate
  upstream 24/128/32-MiB and renderer/discovery limits remain explicitly recorded.
- New application helper SdpRuntime owns runtime path validation/allocation. It
  does not parse SDL or change the older IPC runtime fallback policy.

## Milestones and evidence

| Milestone | Acceptance | State |
| --- | --- | --- |
| R1 | Runtime allocation and lazy viewpoint activation, lifetime cleanup | completed on successor 0.6; N2-001 |
| R2 | Unit/runtime and native real-producer tests, Release binary, docs | completed on successor 0.6; N2-001 |

Tests must prove missing/unsafe runtime errors, valid isolated runtime paths,
no workspace/generation during discovery or parent activation, concrete leaf
output beneath runtime, Back/Forward snapshot survival, >64 selections and
unreachable cleanup/window destruction. Keep cancellation/stale/dirty regressions.
Tests use private XDG_RUNTIME_DIR under a writable temporary HOME because the
host cannot write the actual desktop /run directory. No actual /run installation
or mount behavior is certified by these tests. Model-source hold remains intact.

## T007/T008 checkpoint

[RT-001](evidence/RT-001/README.md) identifies the built Release candidate and
captures source inputs, logs and remaining obligations. Runtime unit test PASS;
non-GUI selection 41/45 PASS. Socket/ptrace restrictions block four non-GUI checks
and all four focused native checks. The owner's T008 retry reproduces AF_UNIX
errno 1 and Xvfb startup failures. No native acceptance or installation is claimed.
Keep this plan active until integrated verification and its handoff are complete.

## 2026-10-07 successor verification

Session 0003 T003 removes the previous environment block on the exact 0.6 successor.
[N2-001](../SDL-Navigation/evidence/N2-001/README.md) records runtime unit/native
105-selection/history/cleanup and real-producer regressions PASS in the 88-test
suite, plus successful installation. R1/R2 local delivery is complete. The plan
remains active for independent review/disposition; #025 retains wider producer and
content-limit work. Previous permissions/evidence describe their actual dated host.
