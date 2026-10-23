# VP02 — Architecture and logical decomposition

Revision: `9fb5d8d1fc95f22386d89e6ebcd0c283be4fb59d7a6782f56a9f0ae735759f79`.

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

Source facts: [f0044](sdl-source://s0/2409/2432).

Declarations: [Desktop](sdl-source://s0/320/333), [Local](sdl-source://s0/439/455), [Remote](sdl-source://s0/575/592), [Standalone](sdl-source://s0/614/630).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/439/455) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/575/592) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/320/333) | unit | Local | [f0044](sdl-source://s0/2409/2432) |
| [Standalone](sdl-source://s0/614/630) | unit | Root (no declared parent) | — |


