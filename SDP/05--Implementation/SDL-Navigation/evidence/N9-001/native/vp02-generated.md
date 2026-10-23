# VP02 — Architecture and logical decomposition

Revision: `f08f981f22d579abe806b5da72052d23ea35c313d0d93ef1cb44b17782bb273c`.

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

Source facts: [f0017](sdl-source://s0/978/1001).

Declarations: [Desktop](sdl-source://s0/157/170), [Local](sdl-source://s0/241/257), [Remote](sdl-source://s0/331/348), [Standalone](sdl-source://s0/370/386).

## Scope and notation

All declared Systems, Containers and Units in the checked model are shown, including isolated roots. Arrows mean explicit contains. This is logical decomposition, not deployment, Functionality ownership or runtime communication. No parent or connection is inferred.

## Structural register

| Component | Kind | Explicit parent | Contains facts |
| --- | --- | --- | --- |
| [Local](sdl-source://s0/241/257) | container | Root (no declared parent) | — |
| [Remote](sdl-source://s0/331/348) | container | Root (no declared parent) | — |
| [Desktop](sdl-source://s0/157/170) | unit | Local | [f0017](sdl-source://s0/978/1001) |
| [Standalone](sdl-source://s0/370/386) | unit | Root (no declared parent) | — |


