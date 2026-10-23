# VP08 — Channel contracts and sequences

Revision: `0d032e724dc923219596fc5a1cc304cef87aa7d6d0f91f52c5ce2b274bd742e8`.

Explicit scenario steps validated against permits, participation, mode and request/result correlation.

## Channel Bus — mode Quiet

```mermaid
flowchart LR
    n_Bus["Bus (channel)"]
```

Source facts: .

Declarations only.

Declarations: [Bus](sdl-source://s0/136/148).

## Channel IdleBus — mode Quiet

```mermaid
flowchart LR
    n_IdleBus["IdleBus (channel)"]
```

Source facts: .

Declarations only.

Declarations: [IdleBus](sdl-source://s0/366/382).

## Scope and notation

Channel topology is partitioned into one diagram per declared Channel, retaining every participant and binding. Repeated participants refer to the same source declarations. The diagrams show explicit participation in one Mode, not runtime delivery, transport or execution order. Sender arrows point from participant to Channel; receiver arrows point from Channel to participant. Each arrow groups all messages for that participant/channel/role; the register retains every binding and source proof. Modes have no implicit inheritance.

Selected mode: **Quiet**. Mode source: [Quiet](sdl-source://s0/465/476).

No participation is declared for this mode. This does not prove independence or absence of runtime communication.

## Channel register

All declared Channels are retained, including those without participants in the selected mode. Contracts and permitted messages are global model declarations, not evidence of delivery in any mode.

| Channel | Contract | Permitted messages | Source facts |
| --- | --- | --- | --- |
| [Bus](sdl-source://s0/136/148) | [ChannelEvents](sdl-source://s0/171/194) | [ChannelNotice](sdl-source://s0/195/217), [ChannelUpdate](sdl-source://s0/218/240) | [f0008](sdl-source://s0/834/860), [f0012](sdl-source://s0/977/1013), [f0013](sdl-source://s0/1014/1050) |
| [IdleBus](sdl-source://s0/366/382) | [ChannelEvents](sdl-source://s0/171/194) | [ChannelNotice](sdl-source://s0/195/217), [ChannelUpdate](sdl-source://s0/218/240) | [f0035](sdl-source://s0/1886/1916), [f0012](sdl-source://s0/977/1013), [f0013](sdl-source://s0/1014/1050) |

## Scenario index

Open an individual Scenario in the navigation tree for its validated sequence. Source links below open declarations; steps are not concatenated across Scenarios. Completeness is a source assertion, not evidence of execution or test coverage. Preconditions and postconditions are not inferred.

No Scenarios are declared in this scope.

## Derived MessageSet

| Channel | Mode | Datagram | Sender | Receiver | Source IDs |
| --- | --- | --- | --- | --- | --- |

Model gap NO_PARTICIPANTS: No explicit participation. (IdleBus; mode=).

