# VP06 — Activities and delivery plan

Revision: `0d032e724dc923219596fc5a1cc304cef87aa7d6d0f91f52c5ce2b274bd742e8`.

refines, addresses, delivers and depends-on; status is an explicit source claim.

## All activities and explicit refinement

```mermaid
flowchart LR
    n_ABuild["ABuild (activity)"]
    n_AIsolated["AIsolated (activity)"]
    n_APlan["APlan (activity)"]
    n_AVerify["AVerify (activity)"]
    n_ABuild -->|refines| n_APlan
    n_AVerify -->|refines| n_APlan
```

Source facts: [f0003](sdl-source://s0/673/694), [f0007](sdl-source://s0/811/833).

Declarations: [ABuild](sdl-source://s0/34/50), [AIsolated](sdl-source://s0/51/70), [APlan](sdl-source://s0/71/86), [AVerify](sdl-source://s0/87/104).

## Explicit activity prerequisites

```mermaid
flowchart LR
    n_ABuild["ABuild (activity)"]
    n_AIsolated["AIsolated (activity)"]
    n_APlan["APlan (activity)"]
    n_AVerify["AVerify (activity)"]
    n_AVerify -.->|depends-on| n_ABuild
```

Source facts: [f0005](sdl-source://s0/738/764).

Declarations: [ABuild](sdl-source://s0/34/50), [AIsolated](sdl-source://s0/51/70), [APlan](sdl-source://s0/71/86), [AVerify](sdl-source://s0/87/104).

## Scope and notation

All declared Activities are retained, including isolated activities. Refines points from detailed to abstract activity; depends-on points from dependent activity to its explicit prerequisite. Decomposition and prerequisites are separate diagrams. Neither relation inherits status or dependencies. No dates, durations, execution order or calendar schedule are inferred.

Implementation status is a source claim, not test evidence or owner acceptance. Addresses models Functionality coverage; delivers models a Feature contribution, not complete delivery; illustrates links an example Scenario, not an executed test.

## Activity register

| Activity | Source-claimed status | Refines | Source facts |
| --- | --- | --- | --- |
| [ABuild](sdl-source://s0/34/50) | implemented | [APlan](sdl-source://s0/71/86) | [f0002](sdl-source://s0/625/672), [f0003](sdl-source://s0/673/694) |
| [AIsolated](sdl-source://s0/51/70) | Unspecified | No declared parent | — |
| [APlan](sdl-source://s0/71/86) | planned | No declared parent | [f0004](sdl-source://s0/695/737) |
| [AVerify](sdl-source://s0/87/104) | verified | [APlan](sdl-source://s0/71/86) | [f0006](sdl-source://s0/765/810), [f0007](sdl-source://s0/811/833) |

## Addressed responsibilities

| Activity | Functionality | Explicit owner | Source facts |
| --- | --- | --- | --- |
| [ABuild](sdl-source://s0/34/50) | [Display](sdl-source://s0/296/318) | [Desktop](sdl-source://s0/282/295) | [f0000](sdl-source://s0/571/596), [f0023](sdl-source://s0/1380/1401) |

## Planned Feature contributions

| Activity | Feature | Source fact |
| --- | --- | --- |
| [ABuild](sdl-source://s0/34/50) | [Navigation](sdl-source://s0/418/437) | [f0001](sdl-source://s0/597/624) |

No illustrative Scenarios are linked to activities.

## Functionality without activity coverage

This lists missing addresses relations in the model, not unimplemented native code.

No unaddressed Functionality declarations in this snapshot. This is not proof of completed implementation.


