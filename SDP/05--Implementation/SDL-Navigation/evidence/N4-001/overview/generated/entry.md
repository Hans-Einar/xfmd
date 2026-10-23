# VP01 — Use cases and traceability

Revision: `9bdf257d7dd88f39825fe82522955ed8a94df2f790add690ced1c615edf4bd30`.

pursues, supports and contributes-to; modeled scope without an invented System boundary.

## All declared actors and use cases

```mermaid
usecase-beta
direction LR
actor u0("Guest")
actor u1("Reader")
u2("Browse")
u3("Export")
u4("Isolated")
u1 -- u2
u1 -- u3
```

Source facts: [f0005](sdl-source://s0/403/425), [f0006](sdl-source://s0/426/448).

Declarations: [Browse](sdl-source://s0/34/49), [Export](sdl-source://s0/87/102), [Guest](sdl-source://s0/103/115), [Isolated](sdl-source://s0/116/133), [Reader](sdl-source://s0/198/211).

## Scope and notation

Complete declared actor/use-case inventory in this source snapshot, including isolated declarations. An association means SDL pursues; no System boundary or UML include/extend is inferred.

## Functionality realization

Features and Functionality are not use cases. Only modeled supports and contributes-to relations are listed.

| Source | Relation | Target | Fact |
| --- | --- | --- | --- |
| Display | contributes-to | Navigation | [f0003](sdl-source://s0/340/374) |
| Navigation | supports | Browse | [f0004](sdl-source://s0/375/402) |

Scenario sequences and pre/postconditions are not part of this overview; they are not inferred from static relations.


Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Export; mode=).

Model gap NO_ACTOR: No Actor pursues this use case. (Isolated; mode=).

Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Isolated; mode=).

Model gap NO_GOAL: Actor has no pursues relation. (Guest; mode=).

