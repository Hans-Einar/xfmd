# VP02 — Architecture and logical decomposition

Revision: `0d032e724dc923219596fc5a1cc304cef87aa7d6d0f91f52c5ce2b274bd742e8`.

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

Source facts: [f0036](sdl-source://s0/1917/1940).

Declarations: [Desktop](sdl-source://s0/282/295), [Local](sdl-source://s0/401/417), [Remote](sdl-source://s0/491/508), [Standalone](sdl-source://s0/530/546).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/401/417) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/491/508) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/282/295) | unit | Local | [f0036](sdl-source://s0/1917/1940) |
| [Standalone](sdl-source://s0/530/546) | unit | Root (no declared parent) | — |


