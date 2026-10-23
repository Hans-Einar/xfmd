# VP02 — Architecture and logical decomposition

Revision: `b3011e73ee1eece4b0df6861ea14435f312f5e54fcccec544f2b0ceebfe7012e`.

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

Source facts: [f0046](sdl-source://s0/2614/2637).

Declarations: [Desktop](sdl-source://s0/320/333), [Local](sdl-source://s0/439/455), [Remote](sdl-source://s0/697/714), [Standalone](sdl-source://s0/736/752).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/439/455) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/697/714) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/320/333) | unit | Local | [f0046](sdl-source://s0/2614/2637) |
| [Standalone](sdl-source://s0/736/752) | unit | Root (no declared parent) | — |


