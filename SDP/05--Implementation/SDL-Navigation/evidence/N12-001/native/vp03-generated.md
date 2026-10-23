# VP03 — Responsibilities and capabilities across the architecture

Revision: `799b274252245be2ac5258510067da66a08bb45276538bcbb6482b3189e898c3`.

owns, realizes and provides. Capability is not Feature.

## Responsibilities and capabilities across the architecture

```mermaid
flowchart LR
    n_Desktop["Desktop (unit) | owns: R01 | contributes: C01 | provides: C01"]
    n_Local["Local (container)"]
    n_Remote["Remote (container)"]
    n_Standalone["Standalone (unit)"]
    n_Unconnected("Unconnected (capability)")
    n_Local -->|contains| n_Desktop
```

Source facts: [f0028](sdl-source://s0/1893/1914), [f0029](sdl-source://s0/1915/1944), [f0030](sdl-source://s0/1945/1970), [f0031](sdl-source://s0/1971/1997), [f0039](sdl-source://s0/2349/2375), [f0044](sdl-source://s0/2531/2554).

Declarations: [Desktop](sdl-source://s0/320/333), [Local](sdl-source://s0/439/455), [Remote](sdl-source://s0/697/714), [Standalone](sdl-source://s0/736/752), [Unconnected](sdl-source://s0/773/796).

## Scope and notation

The complete logical architecture uses contains edges, including isolated components. R keys identify responsibilities on their immediate owners (owns). C keys identify capabilities: contributes summarizes an owns + realizes path; provides records an explicit offered capability. These annotations are not new SDL relations. Realizes is a contribution, not proof of complete or verified realization. Ownership does not imply provision, and provision does not imply realization. Rounded standalone capabilities have neither a represented contributor nor provider. Keys are sorted within this snapshot. Features and runtime allocation belong to other viewpoints.

## Responsibility register

| Key | Functionality | Immediate owner | Capability contributions | Source facts |
| --- | --- | --- | --- | --- |
| R01 | [Display](sdl-source://s0/334/356) | [Desktop](sdl-source://s0/320/333) | [Delivery](sdl-source://s0/299/319) | [f0028](sdl-source://s0/1893/1914), [f0039](sdl-source://s0/2349/2375) |

## Capability register

| Key | Capability | Explicit providers | Contributing responsibilities | Source facts |
| --- | --- | --- | --- | --- |
| C01 | [Delivery](sdl-source://s0/299/319) | [Desktop](sdl-source://s0/320/333) | [Display](sdl-source://s0/334/356) | [f0031](sdl-source://s0/1971/1997), [f0039](sdl-source://s0/2349/2375) |
| C02 | [Unconnected](sdl-source://s0/773/796) | Not specified | Not specified | — |

Not specified means no such fact is declared; it does not assert impossibility.


