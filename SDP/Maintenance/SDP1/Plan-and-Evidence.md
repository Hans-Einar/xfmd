# SDP1 — XFMD process bootstrap

Base: f3c2c31; branch: maintenance/phase-sdp1-process-bootstrap.
Owner authorization: 2026-09-25. Primary:
[KB-XFMD-016](../../Agents/KanBan/completed/%23016--Change--SDP-process-bootstrap.md).

| Milestone | Scope | State |
| --- | --- | --- |
| SDP1-M1 | Initialize phase entry points, relocate and align KanBan, preserve history, update instructions and register adoption handoff | Completed |

## Migration map and compatibility

Agents/KanBan/** moved to SDP/Agents/KanBan/**. Card IDs and board-relative
addresses are unchanged, so historical ledger paths remain correct relative to
the relocated board. This is a board-root relocation, not 15 card lifecycle moves.
Git retains earlier content at the old root. Each existing card gains a current
CardState and an appended review event; no past event bytes are rewritten.
Evidence images and other binary records are preserved byte-for-byte.

[Upstream](../../Agents/KanBan/Upstream.md) pins SDP commit `1a3f5e88364ee219b063dd4615fac19ced33d89b`.
The new-event payload is 0.2; board metadata stays 0.1 and the envelope 1.0.
Current definitions are local and usable without the SDP checkout. A globally
released overall KanBan version remains upstream work in KB-SDP-014.

## Acceptance checks

Verify old ledger prefix, complete migration inventory, binary hashes, schemas,
replay and index/physical placement; exactly one valid CardState per card; local
links and baseline definition fingerprints. Run lineage negative examples and
XFMD blueprint/link/symbol checks. Product binaries are unchanged; no new GUI
verification or build is claimed for this documentation/process delivery.

## Handoff

[KB-XFMD-017](../../Agents/KanBan/backlog/%23017--Proposal--Adopt-SDP-and-model-XFMD.md)
owns full process adoption and .design models. Requirements/architecture/blueprints
and sprint evidence stay authoritative during that work. Native sidebar/preview
features remain separately tracked. No runtime discovery metadata is invented.

## Executed checks — SDP1-M1

- Migration inventory: 29 original board files at new destinations; original
  51-event ledger prefix, board descriptor, .gitkeep files and two PNG images
  preserved byte-for-byte. Old board root no longer exists.
- Local board validation: 17 cards and 69 events; envelope/payload schemas,
  predecessor replay, monotonic event IDs, CardState, placement and index passed.
- Lineage examples: full/partial merge and split, historical 0.1, and 15 negative
  cases passed using only local schemas (Python 3/jsonschema).
- Markdown target verification: 1,333 local links passed before closure, followed
  by 1,335 passing links after closure and the root README entry. Cross-repository references were updated in SDP.
- XFMD validators: 37 blueprints/70 requirements passed metadata, matrix, Plumbing
  and local-file-link checks; 279 implemented Plumbing callees were found.
- git diff --check passed. No src/tests/build configuration or executable changed;
  no application rebuild, GUI trial or SDL model validation is claimed.

Upstream definitions were read from the pinned Git commit. Unmodified schema,
template and example copies are byte-identical; documented local workflow/link
adaptations are not represented as upstream changes. KB-XFMD-012 remains backlog
for an eventual released contract, KB-XFMD-014/015 retain native features and
KB-XFMD-017 owns the next adoption/design work. No held card was selected.
