# VP01 — Use cases and traceability

Revision: `b3011e73ee1eece4b0df6861ea14435f312f5e54fcccec544f2b0ceebfe7012e`.

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

Source facts: [f0070](sdl-source://s0/3550/3572), [f0071](sdl-source://s0/3573/3595).

Declarations: [Browse](sdl-source://s0/120/135), [Export](sdl-source://s0/375/390), [Guest](sdl-source://s0/391/403), [Isolated](sdl-source://s0/421/438), [Reader](sdl-source://s0/683/696).

## Scope and notation

Complete declared actor/use-case inventory in this source snapshot, including isolated declarations. An association means SDL pursues; no System boundary or UML include/extend is inferred.

## Functionality realization

Features and Functionality are not use cases. Only modeled supports and contributes-to relations are listed.

| Source | Relation | Target | Fact |
| --- | --- | --- | --- |
| Display | contributes-to | Navigation | [f0037](sdl-source://s0/2269/2303) |
| Navigation | supports | Browse | [f0048](sdl-source://s0/2669/2696) |

Scenario sequences and pre/postconditions are not part of this overview; they are not inferred from static relations.


Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Export; mode=).

Model gap NO_ACTOR: No Actor pursues this use case. (Isolated; mode=).

Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Isolated; mode=).

Model gap NO_GOAL: Actor has no pursues relation. (Guest; mode=).

