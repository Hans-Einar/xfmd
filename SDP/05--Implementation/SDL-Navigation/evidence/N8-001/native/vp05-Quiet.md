# VP05 — Dependencies by mode

Revision: `1da94717409fc03ce14a88a2de6245db0f15915310ae72e8fd720d276c97a730`.

requires in mode; modes have no implicit inheritance.

## Required interfaces — mode Quiet

```mermaid
flowchart LR
    n_Delivery["Delivery (capability)"]
    n_Unconnected["Unconnected (capability)"]
```

Source facts: .

Declarations only.

Declarations: [Delivery](sdl-source://s0/65/85), [Unconnected](sdl-source://s0/316/339).

## Scope and notation

Requires states that an Interface is necessary for a Capability in the selected Mode. It does not establish sufficiency, a provider, consumption, transport or runtime execution. Modes have no implicit inheritance. Dashed requires arrows point from Capability to Interface.

Selected mode: **Quiet**. Mode source: [Quiet](sdl-source://s0/234/245).

No required interfaces are declared for this mode. This does not prove independence or sufficiency.

## Capability requirements

| Capability | Required interface in selected mode | Source facts |
| --- | --- | --- |
| [Delivery](sdl-source://s0/65/85) | No declared requirement | — |
| [Unconnected](sdl-source://s0/316/339) | No declared requirement | — |

Capabilities without a declared requirement remain visible; no claim of completeness is inferred. Interfaces not required in this mode are outside this projection.


