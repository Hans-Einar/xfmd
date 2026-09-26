# Sprint 009 — Favorites

Owner-selected KB-XFMD-018, based on cf11709. Version remains 0.3.
[P057](Phase-057--Favorites.md) adds persistent folder/file favorites to the Files
sidebar. One phase, one implementation milestone; branch
`sprint/009/phase/057-favorites`. Existing SDL/SDP proposals remain separate.

P057-M1 is locally complete; [acceptance evidence](evidence/P057.md) records tests
and screenshots. The combined PR is identified by
[its phase head](https://github.com/Hans-Einar/xfmd/pulls?q=is%3Apr+head%3Asprint%2F009%2Fphase%2F057-favorites).
It is stacked on `maintenance/phase-sdp1-process-bootstrap` to isolate this delivery
from earlier unmerged work. Local installation continues the owner's UI trial;
keep existing application processes running and preserve an installation backup.

P057-M2 completes the owner's context-menu correction: Favorites is a list without
buttons/hint; tree/history right-clicks add and Favorites right-clicks remove.
The same [PR #41](https://github.com/Hans-Einar/xfmd/pull/41) carries the follow-up.
[M2 evidence](evidence/M2/README.md) supersedes M1's interaction acceptance;
persistence/grouping and the historical M1 record remain intact.
