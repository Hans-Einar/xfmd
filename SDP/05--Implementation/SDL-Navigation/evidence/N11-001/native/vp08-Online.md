# VP08 — Channel contracts and sequences

Revision: `9fb5d8d1fc95f22386d89e6ebcd0c283be4fb59d7a6782f56a9f0ae735759f79`.

Explicit scenario steps validated against permits, participation, mode and request/result correlation.

## Channel Bus — mode Online

```mermaid
flowchart LR
    n_Bus["Bus (channel)"]
    n_Desktop["Desktop (unit)"]
    n_Remote["Remote (container)"]
    n_Desktop -->|sender| n_Bus
    n_Bus -->|receiver| n_Remote
```

Source facts: [f0033](sdl-source://s0/1939/1998), [f0034](sdl-source://s0/1999/2058), [f0051](sdl-source://s0/2593/2653), [f0052](sdl-source://s0/2654/2714).

Declarations: [Bus](sdl-source://s0/136/148), [Desktop](sdl-source://s0/320/333), [Remote](sdl-source://s0/575/592).

## Channel IdleBus — mode Online

```mermaid
flowchart LR
    n_IdleBus["IdleBus (channel)"]
```

Source facts: .

Declarations only.

Declarations: [IdleBus](sdl-source://s0/404/420).

## Scope and notation

Channel topology is partitioned into one diagram per declared Channel, retaining every participant and binding. Repeated participants refer to the same source declarations. The diagrams show explicit participation in one Mode, not runtime delivery, transport or execution order. Sender arrows point from participant to Channel; receiver arrows point from Channel to participant. Each arrow groups all messages for that participant/channel/role; the register retains every binding and source proof. Modes have no implicit inheritance.

Selected mode: **Online**. Mode source: [Online](sdl-source://s0/510/522).

## Participation register

| Participant | Channel | Role | Message | Source fact |
| --- | --- | --- | --- | --- |
| [Desktop](sdl-source://s0/320/333) | [Bus](sdl-source://s0/136/148) | sender | [ChannelNotice](sdl-source://s0/195/217) | [f0033](sdl-source://s0/1939/1998) |
| [Desktop](sdl-source://s0/320/333) | [Bus](sdl-source://s0/136/148) | sender | [ChannelUpdate](sdl-source://s0/218/240) | [f0034](sdl-source://s0/1999/2058) |
| [Remote](sdl-source://s0/575/592) | [Bus](sdl-source://s0/136/148) | receiver | [ChannelNotice](sdl-source://s0/195/217) | [f0051](sdl-source://s0/2593/2653) |
| [Remote](sdl-source://s0/575/592) | [Bus](sdl-source://s0/136/148) | receiver | [ChannelUpdate](sdl-source://s0/218/240) | [f0052](sdl-source://s0/2654/2714) |

## Channel register

All declared Channels are retained, including those without participants in the selected mode. Contracts and permitted messages are global model declarations, not evidence of delivery in any mode.

| Channel | Contract | Permitted messages | Source facts |
| --- | --- | --- | --- |
| [Bus](sdl-source://s0/136/148) | [ChannelEvents](sdl-source://s0/171/194) | [ChannelNotice](sdl-source://s0/195/217), [ChannelUpdate](sdl-source://s0/218/240) | [f0008](sdl-source://s0/1061/1087), [f0012](sdl-source://s0/1204/1240), [f0013](sdl-source://s0/1241/1277) |
| [IdleBus](sdl-source://s0/404/420) | [ChannelEvents](sdl-source://s0/171/194) | [ChannelNotice](sdl-source://s0/195/217), [ChannelUpdate](sdl-source://s0/218/240) | [f0043](sdl-source://s0/2378/2408), [f0012](sdl-source://s0/1204/1240), [f0013](sdl-source://s0/1241/1277) |

## Scenario index

Open an individual Scenario in the navigation tree for its validated sequence. Source links below open declarations; steps are not concatenated across Scenarios. Completeness is a source assertion, not evidence of execution or test coverage. Preconditions and postconditions are not inferred.

| Scenario | Mode | Completeness | Steps | Source facts |
| --- | --- | --- | --- | --- |
| [Example](sdl-source://s0/357/374) | [Online](sdl-source://s0/510/522) | open | 1 | [f0040](sdl-source://s0/2254/2286), [f0041](sdl-source://s0/2287/2310), [f0042](sdl-source://s0/2311/2377) |

## Derived MessageSet

| Channel | Mode | Datagram | Sender | Receiver | Source IDs |
| --- | --- | --- | --- | --- | --- |
| Bus | Online | ChannelNotice | Desktop | Remote | [f0008](sdl-source://s0/1061/1087), [f0012](sdl-source://s0/1204/1240), [f0015](sdl-source://s0/1318/1352), [f0033](sdl-source://s0/1939/1998), [f0051](sdl-source://s0/2593/2653) |
| Bus | Online | ChannelUpdate | Desktop | Remote | [f0008](sdl-source://s0/1061/1087), [f0013](sdl-source://s0/1241/1277), [f0017](sdl-source://s0/1393/1427), [f0034](sdl-source://s0/1999/2058), [f0052](sdl-source://s0/2654/2714) |

Model gap NO_PARTICIPANTS: No explicit participation. (IdleBus; mode=).

