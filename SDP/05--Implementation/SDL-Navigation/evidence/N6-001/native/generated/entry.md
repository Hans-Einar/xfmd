# VP01 — Use cases and traceability

Revision: `5220e973f4bb86e2ecba31e01441539e1b922579a39ba9679a8915020611bc6f`.

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

Source facts: [f0008](sdl-source://s0/543/565), [f0009](sdl-source://s0/566/588).

Declarations: [Browse](sdl-source://s0/34/49), [Export](sdl-source://s0/108/123), [Guest](sdl-source://s0/124/136), [Isolated](sdl-source://s0/137/154), [Reader](sdl-source://s0/219/232).

## Scope and notation

Complete declared actor/use-case inventory in this source snapshot, including isolated declarations. An association means SDL pursues; no System boundary or UML include/extend is inferred.

## Functionality realization

Features and Functionality are not use cases. Only modeled supports and contributes-to relations are listed.

| Source | Relation | Target | Fact |
| --- | --- | --- | --- |
| Display | contributes-to | Navigation | [f0004](sdl-source://s0/429/463) |
| Navigation | supports | Browse | [f0007](sdl-source://s0/515/542) |

Scenario sequences and pre/postconditions are not part of this overview; they are not inferred from static relations.


Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Export; mode=).

Model gap NO_ACTOR: No Actor pursues this use case. (Isolated; mode=).

Model gap NO_CONTRIBUTION: No direct or Feature-mediated Functionality contributions. (Isolated; mode=).

Model gap NO_GOAL: Actor has no pursues relation. (Guest; mode=).

