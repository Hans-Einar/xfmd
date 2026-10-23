# VP04 — Interfaces and collaboration

Revision: `0d032e724dc923219596fc5a1cc304cef87aa7d6d0f91f52c5ce2b274bd742e8`.

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

Source facts: [f0022](sdl-source://s0/1358/1379), [f0040](sdl-source://s0/2015/2035).

Declarations: [Api](sdl-source://s0/105/119), [Desktop](sdl-source://s0/282/295), [Remote](sdl-source://s0/491/508), [SparePort](sdl-source://s0/509/529).

## Scope and notation

Every declared Interface is included, even without a consumer. Rectangles are explicit Unit/Container consumers; rounded nodes are interfaces. Dashed consumes arrows point from consumer to interface and mean designed use, not an observed call or mandatory availability. No provider, Channel, transport, call order or mode necessity is inferred. Units without consumes facts are outside this projection; VP02 shows the complete logical structure.

## Interface register

| Interface | Explicit consumers | Source facts |
| --- | --- | --- |
| [Api](sdl-source://s0/105/119) | [Desktop](sdl-source://s0/282/295), [Remote](sdl-source://s0/491/508) | [f0022](sdl-source://s0/1358/1379), [f0040](sdl-source://s0/2015/2035) |
| [SparePort](sdl-source://s0/509/529) | No declared consumer | — |

No declared consumer means no consumes fact is present; it does not prove an unused or unnecessary interface. Provider information is not expressed by consumes.

## Interface use

No Channel or provider is inferred.

| Consumer | Interface | Fact | Line |
| --- | --- | --- | --- |
| Desktop | Api | [f0022](sdl-source://s0/1358/1379) | 54 |
| Remote | Api | [f0040](sdl-source://s0/2015/2035) | 72 |

