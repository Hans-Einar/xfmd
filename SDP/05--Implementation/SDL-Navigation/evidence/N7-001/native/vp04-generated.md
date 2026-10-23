# VP04 — Interfaces and collaboration

Revision: `ec80ff038b0bcea3e58e88f4162e0b83ae754cc5b8539413cb615f948cd61699`.

consumes records use; no provider, Channel or executable message flow is inferred.

## All declared interfaces and explicit consumers

```mermaid
flowchart LR
    n_Api("Api (interface)")
    n_Desktop["Desktop (unit)"]
    n_Remote["Remote (container)"]
    n_SparePort("SparePort (interface)")
    n_Desktop -.->|consumes| n_Api
    n_Remote -.->|consumes| n_Api
```

Source facts: [f0000](sdl-source://s0/328/349), [f0011](sdl-source://s0/647/667).

Declarations: [Api](sdl-source://s0/34/48), [Desktop](sdl-source://s0/86/99), [Remote](sdl-source://s0/248/265), [SparePort](sdl-source://s0/266/286).

## Scope and notation

Every declared Interface is included, even without a consumer. Rectangles are explicit Unit/Container consumers; rounded nodes are interfaces. Dashed consumes arrows point from consumer to interface and mean designed use, not an observed call or mandatory availability. No provider, Channel, transport, call order or mode necessity is inferred. Units without consumes facts are outside this projection; VP02 shows the complete logical structure.

## Interface register

| Interface | Explicit consumers | Source facts |
| --- | --- | --- |
| [Api](sdl-source://s0/34/48) | [Desktop](sdl-source://s0/86/99), [Remote](sdl-source://s0/248/265) | [f0000](sdl-source://s0/328/349), [f0011](sdl-source://s0/647/667) |
| [SparePort](sdl-source://s0/266/286) | No declared consumer | — |

No declared consumer means no consumes fact is present; it does not prove an unused or unnecessary interface. Provider information is not expressed by consumes.

## Interface use

No Channel or provider is inferred.

| Consumer | Interface | Fact | Line |
| --- | --- | --- | --- |
| Desktop | Api | [f0000](sdl-source://s0/328/349) | 19 |
| Remote | Api | [f0011](sdl-source://s0/647/667) | 30 |

