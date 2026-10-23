# VP04 — Interfaces and collaboration

Revision: `799b274252245be2ac5258510067da66a08bb45276538bcbb6482b3189e898c3`.

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

Source facts: [f0026](sdl-source://s0/1845/1866), [f0070](sdl-source://s0/3513/3533).

Declarations: [Api](sdl-source://s0/105/119), [Desktop](sdl-source://s0/320/333), [Remote](sdl-source://s0/697/714), [SparePort](sdl-source://s0/715/735).

## Scope and notation

Every declared Interface is included, even without a consumer. Rectangles are explicit Unit/Container consumers; rounded nodes are interfaces. Dashed consumes arrows point from consumer to interface and mean designed use, not an observed call or mandatory availability. No provider, Channel, transport, call order or mode necessity is inferred. Units without consumes facts are outside this projection; VP02 shows the complete logical structure.

## Interface register

| Interface | Explicit consumers | Source facts |
| --- | --- | --- |
| [Api](sdl-source://s0/105/119) | [Desktop](sdl-source://s0/320/333), [Remote](sdl-source://s0/697/714) | [f0026](sdl-source://s0/1845/1866), [f0070](sdl-source://s0/3513/3533) |
| [SparePort](sdl-source://s0/715/735) | No declared consumer | — |

No declared consumer means no consumes fact is present; it does not prove an unused or unnecessary interface. Provider information is not expressed by consumes.

## Interface use

No Channel or provider is inferred.

| Consumer | Interface | Fact | Line |
| --- | --- | --- | --- |
| Desktop | Api | [f0026](sdl-source://s0/1845/1866) | 75 |
| Remote | Api | [f0070](sdl-source://s0/3513/3533) | 119 |

