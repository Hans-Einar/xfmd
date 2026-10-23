# VP02 — Architecture and logical decomposition

Revision: `1da94717409fc03ce14a88a2de6245db0f15915310ae72e8fd720d276c97a730`.

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

Source facts: [f0009](sdl-source://s0/644/667).

Declarations: [Desktop](sdl-source://s0/86/99), [Local](sdl-source://s0/170/186), [Remote](sdl-source://s0/260/277), [Standalone](sdl-source://s0/299/315).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/170/186) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/260/277) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/86/99) | unit | Local | [f0009](sdl-source://s0/644/667) |
| [Standalone](sdl-source://s0/299/315) | unit | Root (no declared parent) | — |


