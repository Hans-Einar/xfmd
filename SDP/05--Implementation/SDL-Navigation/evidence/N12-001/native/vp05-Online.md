# VP05 — Dependencies by mode

Revision: `799b274252245be2ac5258510067da66a08bb45276538bcbb6482b3189e898c3`.

requires in mode; modes have no implicit inheritance.

## Required interfaces — mode Online

```mermaid
flowchart LR
    n_Delivery["Delivery (capability)"]
    n_SparePort["SparePort (interface)"]
    n_Unconnected["Unconnected (capability)"]
    n_Delivery -.->|requires| n_SparePort
```

Source facts: [f0025](sdl-source://s0/1801/1844).

Declarations: [Delivery](sdl-source://s0/299/319), [SparePort](sdl-source://s0/715/735), [Unconnected](sdl-source://s0/773/796).

## Scope and notation

Requires states that an Interface is necessary for a Capability in the selected Mode. It does not establish sufficiency, a provider, consumption, transport or runtime execution. Modes have no implicit inheritance. Dashed requires arrows point from Capability to Interface.

Selected mode: **Online**. Mode source: [Online](sdl-source://s0/510/522).

## Capability requirements

| Capability | Required interface in selected mode | Source facts |
| --- | --- | --- |
| [Delivery](sdl-source://s0/299/319) | [SparePort](sdl-source://s0/715/735) | [f0025](sdl-source://s0/1801/1844) |
| [Unconnected](sdl-source://s0/773/796) | No declared requirement | — |

Capabilities without a declared requirement remain visible; no claim of completeness is inferred. Interfaces not required in this mode are outside this projection.


