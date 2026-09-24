# Phase 051 — Tooling boundaries

Sprint: 005. Branch: `sprint/005/phase/051-tooling-boundaries`.
Mode: collaborative, tooling-only checks. Status: implemented and locally checked;
uncommitted, no PR or remote CI run.

## Scope and acceptance

1. CI checks formatting in all four XFMD workspace packages without traversing
   the pinned renderer dependency. XFMD formatting failures must still fail.
2. Blueprint local-link validation ignores Markdown owned by `.deps` and
   `third_party`. Broken links in project documents, including KanBan, still fail;
   metadata, chapter, matrix and plumbing checks retain their current behavior.

No runtime UR/SR, public contract or blueprint plumbing changes. The developer
validation contract is documented in [the working method](../../docs/working-method.md).
Source hunks: `ffb98e86a386cdc8881f3306e67a905b60fc8889` (CI package selection)
and `c81b50838d66e3107fa289dd43088ce08714dcab` (dependency Markdown exclusion).
Both commits mix unrelated BoxUI work; copy only these hunks.

## Milestone

| ID | Delivery | Commit | Verification |
| --- | --- | --- | --- |
| M01 | Tooling scope, isolated fixes and evidence | Pending; working tree only | Formatting scope, negative controls, validators and board replay. |

## Check evidence

Executed 2026-09-24 on HEAD `47a245a` plus the working-tree changes described
above. [Verification identity](evidence/verification.json) records tool versions
and SHA-256 hashes of the changed tools, workspace manifest, dependency pin and
probe script. Local Cargo/rustc 1.92.0 are distribution binaries; rustfmt is 1.8.0.
This is not a clean-commit test or a GitHub Actions run.

| Check | Result and evidence |
| --- | --- |
| `cargo fmt --all --check --verbose` | Pass on current sources, but includes dependency targets despite workspace exclusion; [target list](evidence/format-all.txt). This is a reproduced scope problem, not a reproduced current formatting failure. |
| CI's explicit four-package command with `-- --check` | Pass; verbose equivalent selects only the four XFMD crate roots, [target list](evidence/format-selected.txt). |
| `python3 sprints/Sprint-005--Tooling-Boundaries/evidence/check-boundaries.py` from repository root | 16 checks pass in isolated temporary projects; [script](evidence/check-boundaries.py), [output](evidence/boundary-probes.txt). |
| `python3 tools/validate_blueprints.py` | Pass: 37 blueprints, 70 requirements. |
| `python3 tools/check_blueprint_symbols.py` | Pass: 250 implemented callees found. |
| `python3 tools/check_layers.py` | Pass: pure-layer include boundaries. |
| Board replay and `git diff --check` | Pass after completion, including current card paths and incoming Markdown links. |

The boundary probes cover dependency-only Rust format failure under `--all`,
its exclusion under the selected command, and retained failure for each of the
four XFMD packages. They compare CI's explicit package names to Cargo workspace
members. Markdown probes cover ignored dependency links (including nested
third_party), rejected project/board links, and a rejected project link into a
missing dependency file. Invalid metadata, chapter name, plumbing source
and acceptance-matrix coverage remain rejected.

No application binary build or new GUI acceptance is claimed: this phase changes
a CI command, a Python documentation checker and documentation only. Existing
runtime tests were inspected for candidate assessment, not rerun as new evidence.

## Handoff

Keep the BoxUI branch/commits as historical references. Conditional runtime
candidate decisions remain in KB-XFMD-002; this phase does not activate them.
No PR closure, branch deletion, merge or installation is part of this delivery.
