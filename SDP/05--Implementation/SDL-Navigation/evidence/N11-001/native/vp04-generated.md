# VP04 — Interfaces and collaboration

Revision: `9fb5d8d1fc95f22386d89e6ebcd0c283be4fb59d7a6782f56a9f0ae735759f79`.

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

Source facts: [f0026](sdl-source://s0/1723/1744), [f0050](sdl-source://s0/2572/2592).

Declarations: [Api](sdl-source://s0/105/119), [Desktop](sdl-source://s0/320/333), [Remote](sdl-source://s0/575/592), [SparePort](sdl-source://s0/593/613).

## Scope and notation

Every declared Interface is included, even without a consumer. Rectangles are explicit Unit/Container consumers; rounded nodes are interfaces. Dashed consumes arrows point from consumer to interface and mean designed use, not an observed call or mandatory availability. No provider, Channel, transport, call order or mode necessity is inferred. Units without consumes facts are outside this projection; VP02 shows the complete logical structure.

## Interface register

| Interface | Explicit consumers | Source facts |
| --- | --- | --- |
| [Api](sdl-source://s0/105/119) | [Desktop](sdl-source://s0/320/333), [Remote](sdl-source://s0/575/592) | [f0026](sdl-source://s0/1723/1744), [f0050](sdl-source://s0/2572/2592) |
| [SparePort](sdl-source://s0/593/613) | No declared consumer | — |

No declared consumer means no consumes fact is present; it does not prove an unused or unnecessary interface. Provider information is not expressed by consumes.

## Interface use

No Channel or provider is inferred.

| Consumer | Interface | Fact | Line |
| --- | --- | --- | --- |
| Desktop | Api | [f0026](sdl-source://s0/1723/1744) | 69 |
| Remote | Api | [f0050](sdl-source://s0/2572/2592) | 93 |

