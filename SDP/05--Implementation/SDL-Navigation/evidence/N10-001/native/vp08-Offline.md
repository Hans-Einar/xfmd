# VP08 — Channel contracts and sequences

Revision: `0d032e724dc923219596fc5a1cc304cef87aa7d6d0f91f52c5ce2b274bd742e8`.

Explicit scenario steps validated against permits, participation, mode and request/result correlation.

## Channel Bus — mode Offline

```mermaid
flowchart LR
    n_Bus["Bus (channel)"]
    n_Desktop["Desktop (unit)"]
    n_Remote["Remote (container)"]
    n_Bus -->|receiver| n_Desktop
    n_Remote -->|sender| n_Bus
```

Source facts: [f0025](sdl-source://s0/1429/1491), [f0043](sdl-source://s0/2158/2217).

Declarations: [Bus](sdl-source://s0/136/148), [Desktop](sdl-source://s0/282/295), [Remote](sdl-source://s0/491/508).

## Channel IdleBus — mode Offline

```mermaid
flowchart LR
    n_IdleBus["IdleBus (channel)"]
```

Source facts: .

Declarations only.

Declarations: [IdleBus](sdl-source://s0/366/382).

## Scope and notation

Channel topology is partitioned into one diagram per declared Channel, retaining every participant and binding. Repeated participants refer to the same source declarations. The diagrams show explicit participation in one Mode, not runtime delivery, transport or execution order. Sender arrows point from participant to Channel; receiver arrows point from Channel to participant. Each arrow groups all messages for that participant/channel/role; the register retains every binding and source proof. Modes have no implicit inheritance.

Selected mode: **Offline**. Mode source: [Offline](sdl-source://s0/438/451).

## Participation register

| Participant | Channel | Role | Message | Source fact |
| --- | --- | --- | --- | --- |
| [Desktop](sdl-source://s0/282/295) | [Bus](sdl-source://s0/136/148) | receiver | [ChannelNotice](sdl-source://s0/195/217) | [f0025](sdl-source://s0/1429/1491) |
| [Remote](sdl-source://s0/491/508) | [Bus](sdl-source://s0/136/148) | sender | [ChannelNotice](sdl-source://s0/195/217) | [f0043](sdl-source://s0/2158/2217) |

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
| Bus | Offline | ChannelNotice | Remote | Desktop | [f0008](sdl-source://s0/834/860), [f0012](sdl-source://s0/977/1013), [f0015](sdl-source://s0/1091/1125), [f0025](sdl-source://s0/1429/1491), [f0043](sdl-source://s0/2158/2217) |
| Bus | Offline | ChannelUpdate |  |  | [f0008](sdl-source://s0/834/860), [f0013](sdl-source://s0/1014/1050), [f0017](sdl-source://s0/1166/1200) |

Model gap INCOMPLETE_PARTICIPATION: Incomplete participation for ChannelUpdate. (Bus; mode=Offline).

Model gap NO_PARTICIPANTS: No explicit participation. (IdleBus; mode=).

