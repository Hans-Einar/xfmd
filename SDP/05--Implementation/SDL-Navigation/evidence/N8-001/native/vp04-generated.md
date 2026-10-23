# VP04 — Interfaces and collaboration

Revision: `1da94717409fc03ce14a88a2de6245db0f15915310ae72e8fd720d276c97a730`.

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

Source facts: [f0002](sdl-source://s0/423/444), [f0013](sdl-source://s0/742/762).

Declarations: [Api](sdl-source://s0/34/48), [Desktop](sdl-source://s0/86/99), [Remote](sdl-source://s0/260/277), [SparePort](sdl-source://s0/278/298).

## Scope and notation

Every declared Interface is included, even without a consumer. Rectangles are explicit Unit/Container consumers; rounded nodes are interfaces. Dashed consumes arrows point from consumer to interface and mean designed use, not an observed call or mandatory availability. No provider, Channel, transport, call order or mode necessity is inferred. Units without consumes facts are outside this projection; VP02 shows the complete logical structure.

## Interface register

| Interface | Explicit consumers | Source facts |
| --- | --- | --- |
| [Api](sdl-source://s0/34/48) | [Desktop](sdl-source://s0/86/99), [Remote](sdl-source://s0/260/277) | [f0002](sdl-source://s0/423/444), [f0013](sdl-source://s0/742/762) |
| [SparePort](sdl-source://s0/278/298) | No declared consumer | — |

No declared consumer means no consumes fact is present; it does not prove an unused or unnecessary interface. Provider information is not expressed by consumes.

## Interface use

No Channel or provider is inferred.

| Consumer | Interface | Fact | Line |
| --- | --- | --- | --- |
| Desktop | Api | [f0002](sdl-source://s0/423/444) | 22 |
| Remote | Api | [f0013](sdl-source://s0/742/762) | 33 |

