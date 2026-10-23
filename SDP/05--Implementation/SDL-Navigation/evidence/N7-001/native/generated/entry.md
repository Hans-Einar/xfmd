# VP01 — Use cases and traceability

Revision: `ec80ff038b0bcea3e58e88f4162e0b83ae754cc5b8539413cb615f948cd61699`.

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

Source facts: [f0009](sdl-source://s0/601/623), [f0010](sdl-source://s0/624/646).

Declarations: [Browse](sdl-source://s0/49/64), [Export](sdl-source://s0/123/138), [Guest](sdl-source://s0/139/151), [Isolated](sdl-source://s0/152/169), [Reader](sdl-source://s0/234/247).

## Scope and notation

Complete declared actor/use-case inventory in this source snapshot, including isolated declarations. An association means SDL pursues; no System boundary or UML include/extend is inferred.

## Functionality realization

Features and Functionality are not use cases. Only modeled supports and contributes-to relations are listed.

| Source | Relation | Target | Fact |
| --- | --- | --- | --- |
| Display | contributes-to | Navigation | [f0005](sdl-source://s0/487/521) |
| Navigation | supports | Browse | [f0008](sdl-source://s0/573/600) |

Scenario sequences and pre/postconditions are not part of this overview; they are not inferred from static relations.


Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Export; mode=).

Model gap NO_ACTOR: No Actor pursues this use case. (Isolated; mode=).

Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Isolated; mode=).

Model gap NO_GOAL: Actor has no pursues relation. (Guest; mode=).

