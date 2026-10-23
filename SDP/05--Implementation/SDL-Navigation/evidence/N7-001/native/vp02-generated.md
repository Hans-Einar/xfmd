# VP02 — Architecture and logical decomposition

Revision: `ec80ff038b0bcea3e58e88f4162e0b83ae754cc5b8539413cb615f948cd61699`.

Container/Unit and contains; library structure is not deployment allocation.

## Complete logical architecture

```mermaid
flowchart LR
    n_Desktop["Desktop (unit)"]
    n_Local["Local (container)"]
    n_Remote["Remote (container)"]
    n_Standalone["Standalone (unit)"]
    n_Local -->|contains| n_Desktop
```

Source facts: [f0007](sdl-source://s0/549/572).

Declarations: [Desktop](sdl-source://s0/86/99), [Local](sdl-source://s0/170/186), [Remote](sdl-source://s0/248/265), [Standalone](sdl-source://s0/287/303).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/170/186) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/248/265) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/86/99) | unit | Local | [f0007](sdl-source://s0/549/572) |
| [Standalone](sdl-source://s0/287/303) | unit | Root (no declared parent) | — |


