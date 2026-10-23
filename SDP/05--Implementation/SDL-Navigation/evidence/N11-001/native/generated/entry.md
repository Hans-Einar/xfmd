# VP01 — Use cases and traceability

Revision: `9fb5d8d1fc95f22386d89e6ebcd0c283be4fb59d7a6782f56a9f0ae735759f79`.

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

Source facts: [f0048](sdl-source://s0/2526/2548), [f0049](sdl-source://s0/2549/2571).

Declarations: [Browse](sdl-source://s0/120/135), [Export](sdl-source://s0/375/390), [Guest](sdl-source://s0/391/403), [Isolated](sdl-source://s0/421/438), [Reader](sdl-source://s0/561/574).

## Scope and notation

Complete declared actor/use-case inventory in this source snapshot, including isolated declarations. An association means SDL pursues; no System boundary or UML include/extend is inferred.

## Functionality realization

Features and Functionality are not use cases. Only modeled supports and contributes-to relations are listed.

| Source | Relation | Target | Fact |
| --- | --- | --- | --- |
| Display | contributes-to | Navigation | [f0037](sdl-source://s0/2147/2181) |
| Navigation | supports | Browse | [f0046](sdl-source://s0/2464/2491) |

Scenario sequences and pre/postconditions are not part of this overview; they are not inferred from static relations.


Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Export; mode=).

Model gap NO_ACTOR: No Actor pursues this use case. (Isolated; mode=).

Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Isolated; mode=).

Model gap NO_GOAL: Actor has no pursues relation. (Guest; mode=).

