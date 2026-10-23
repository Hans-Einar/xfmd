# VP02 — Architecture and logical decomposition

Revision: `5220e973f4bb86e2ecba31e01441539e1b922579a39ba9679a8915020611bc6f`.

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

Source facts: [f0006](sdl-source://s0/491/514).

Declarations: [Desktop](sdl-source://s0/71/84), [Local](sdl-source://s0/155/171), [Remote](sdl-source://s0/233/250), [Standalone](sdl-source://s0/251/267).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/155/171) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/233/250) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/71/84) | unit | Local | [f0006](sdl-source://s0/491/514) |
| [Standalone](sdl-source://s0/251/267) | unit | Root (no declared parent) | — |


