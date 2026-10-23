# Git-based design baselines and change blueprints

| Field | Value |
| --- | --- |
| id | KB-XFMD-022 |
| project | XFMD |
| type | Idea |
| CardState | backlog |
| created | 2026-10-01T13:50:38.678415+00:00 |
| source | Owner conversation 2026-10-01, Session T005 |
| next_review | After the owner discusses baseline/diff support with the SDP process agent, before resuming model reconciliation |
| tags | SDL, SDUI, design, Git, releases, blueprints |

## Owner direction

Explore producing a change blueprint from SDL before implementing a feature.
Compare the design corresponding to the implemented/released product with the
proposed design. Prefer Git commits/tags as baselines over copying design folders
for every release. The owner will discuss process support with the SDP agent.

For the current #021 implementation, preserve authored .design/.sdui and generated
model views. Requirements and the textual FUNC-032 blueprint were already updated;
no model source was changed in this increment. This is an explicit temporary
exception, not removal of design-before-code obligations.

## Questions for the study

- How is an implemented release linked to its exact model commit, paths, profile,
  imports and tool version? A tag alone does not prove implementation conformance.
- Which proposal baseline is supported: another commit or a captured working-tree
  snapshot? How are untracked files, renames, deleted nodes and stable IDs handled?
- Which structural/semantic changes should the blueprint expose, and how should
  unchanged responsibilities, acceptance criteria and affected code be linked?
- How are SDUI changes included without promising native widget parity?
- What does XFMD display, and what remains producer/process responsibility?

Agent recommendation: record reproducible baseline identities and compare model
semantics where supported, retaining a textual Git diff as evidence. Distinguish
proposed design, implementation evidence and owner acceptance. No new SDL syntax,
diff command, directory migration or release policy is adopted by this note.

## Relationships and next action

[KB-XFMD-017](../active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md) owns model
coverage. [PLAN-XFMD-0006](../../05--Implementation/SDP-Navigation/Plan.md) M5 keeps
its outstanding semantic reconciliation. [KB-XFMD-021](../active/%23021--Change--Dynamic-SDP-tabs-labels-and-source-opening.md)
continues its authorized native implementation under the temporary source hold.
[Session T005](../../Sessions/session-%230001--SDP-project-navigation.md) records
the steering. This idea is separate from source/preview #015; no card is superseded.

Review the SDP discussion outcome, select a bounded study and define baseline and
change-blueprint acceptance before updating the held model sources. Registration
does not authorize implementation or cross-project changes.

## Worklog

- 2026-10-01T13:50:38.678415+00:00 — Captured owner direction and open questions in backlog; preserved the
  existing model baseline. No upstream files or other agents were changed/contacted.

## 2026-10-02 — Local SVN design-history alternative

### Owner proposal and intended workflow

Use a local Subversion repository for .design files under SDP/SDL, carried with
main-project Git commits. Check out a separate design workspace, generate a
change blueprint against the implemented model baseline, implement the native
code and commit the accepted design to SVN afterwards. Preserve the ability to
regenerate blueprints between any two committed design revisions. This is a
feasibility question, not selection or authorization to install SVN or migrate
sources. It refines the baseline discussion without lifting the T005 source hold.

### Documented capabilities

- SVN can access a repository directly on local disk without a network server.
- svn diff compares a working copy against BASE or an explicit revision, and
  compares two repository revisions. svn export retrieves a clean historical
  model tree. Checkout creates a working copy, not an exclusive edit lock.
- A portable svnadmin dump can preserve versioned repository history for later
  loading. A source export alone does not preserve that history. Dump/load does
  not by itself preserve repository hooks, configuration or working-copy state.
- A normal Git commit neither commits SVN changes nor coordinates an SVN backup.
  A mutable repository directory is not a reliable Git synchronization contract;
  SVN documents consistent backup via dump/hotcopy instead of arbitrary live copy.

Primary references:
[local repository access](https://svnbook.red-bean.com/en/1.8/svn.basic.in-action.html),
[revision comparison](https://svnbook.red-bean.com/en/1.8/svn.ref.svn.c.diff.html),
[historical export](https://svnbook.red-bean.com/en/1.8/svn.ref.svn.c.export.html),
[portable dump](https://svnbook.red-bean.com/en/1.8/svn.ref.svnadmin.c.dump.html),
[consistent backup](https://svnbook.red-bean.com/en/1.8/svn.reposadmin.maint.html).
The capabilities are documentation-backed; svn and svnadmin were not found on
this host's PATH. No executable proof of concept or blueprint generation ran.

### Feasible integration boundary (agent proposal, not adopted)

Keep the live SVN store and disposable working copies Git-ignored. If Git must
carry all history, prepare a consistent dump artifact and a small manifest before
the main Git commit. Version those artifacts together with native code and the
accepted plain SDL source snapshot, so normal Git inspection remains possible.
A fresh Git clone can restore SVN history from the artifact; checkout restores the
requested workspace separately. Validate that the accepted SDL tree equals the
recorded SVN revision. Absolute machine paths must not be the portable identity.

Use an explicit prepare/check operation; a Git hook may check freshness but does
not provide a distributed transaction or portable enforcement by itself. If SVN
commit succeeds and Git commit fails, retain a recoverable prepared state. A
normal intermediate Git code commit need not mean design acceptance; the selected
completion boundary must state when model promotion is allowed.

SVN has no SDL semantic blueprint generator. The producer must load both complete
model closures and generate the intended structural/semantic comparison. Pin base
and target source identities/hashes, repository/archive identity and paths, SDL
profile, producer version, imported dependencies, comparison options and renderer
where visual reproduction is required. Capture a proposed workspace snapshot if
its blueprint must remain reproducible before a model commit; preserve added,
deleted and renamed files, not just the currently tracked subset.

### Material cost: parallel branches and agents

Restoring the same SVN snapshot in two Git branches allows both local repositories
to create revision r18 with different content, potentially retaining the same
repository UUID. UUID plus revision alone is then ambiguous across branches.
Git cannot semantically merge two SVN databases or history dumps. A single shared
writer could avoid this collision but changes the proposed independent-local
workflow; separate lineages need explicit reconciliation and artifact identities.
Concurrent local working copies also need conflict/update and baseline policies.
SVN checkout alone does not enforce that only authorized design workspaces change
models, or prove that native implementation matches a committed model.

### Alternatives and bounded next review

1. Git-carried local SVN history: supports separate numbered design revisions,
   but requires artifact synchronization, restore and divergent-history policy.
2. Existing Git with explicit implemented-model baseline and a proposal branch/
   worktree: uses the current history/merge machinery. SDP still needs to distinguish
   proposed design from verified implementation; a commit alone is not acceptance.

Agent recommendation: retain both as study alternatives and prefer the existing
Git mechanism unless independent SVN history solves a concrete unmet need. Before
selection, demonstrate fresh-clone restore, old/new model extraction, deterministic
blueprint generation, interrupted preparation and two agents diverging from one
baseline. Decide who owns the implemented/proposed model boundary and how XFMD
chooses which source tree discovery should show. No new product requirement,
folder migration, hook, producer command or model edit is adopted here.
