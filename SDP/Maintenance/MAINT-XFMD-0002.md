# MAINT-XFMD-0002 — Reconcile project helpers after SDP 0.2.0 adoption

| Field | Value |
| --- | --- |
| id | MAINT-XFMD-0002 |
| project | XFMD |
| state | completed |
| PlanType | MaintenancePlan |
| BranchPolicy | current |
| CommitPolicy | milestone |

## Authority and scope

Owner authorization on 2026-09-27 selects the released gh-sdp installation and
upgrade of this active worktree. MAINT-XFMD-0001 records the installer transaction.
This bounded follow-up reconciles project-owned path prose and the board verifier
with the shared ledger, and excludes local recovery journals from Git. Application
code, existing requirements and blueprints remain unchanged. Use the current
working branch and a single milestone commit after verification.

## Milestone XA2-M1

Update the local board verifier to read the descriptor-selected shared ledger,
validate management payloads/predecessors/current document states, and retain card
lineage, CardState, index and placement checks. Rebase current prose/command paths;
retain dated history, original ledger bytes and frozen examples. Run positive and
negative board checks, production gh discovery/tree and repeat upgrade preview.
The original helper and instructions remain available at pre-adoption Git HEAD;
installer backups/journal remain local under SDP/.sdp-operations.

## Remaining scope

KB-XFMD-017 still owns the practical SDL-modeling adoption. No .design models,
native SDP sidebar or new XFMD UI behavior are claimed by this process upgrade.

## Evidence

Live board validation passed: 18 cards, 77 card events and two management records.
All 15 lineage negative cases pass. Released gh-sdp discovery reports valid; repeat
upgrade before and after reconciliation proposes zero changes. All 350 recorded
application/build/tooling file hashes remain unchanged.

The tree inspection reports KanBan unavailable for the existing external primary
KB-SDP-014. This is a separate SDPTool navigation limitation recorded as KB-SDP-034;
no external reference was deleted or relabeled to conceal it. SDL/SDUI model roots
remain absent because no source models are registered. Native UI acceptance is not
claimed. Copy rehearsal
passed 18 cards/76 card events and 15 lineage negative cases; the helper rejected
broken management predecessors and false completion states.

Exact installed SDP source: 738e6c882daed85591311f18248dab2a48ce2076.
Local operation/manifest/verification evidence is retained under the owner state
directory, sdp/adoptions/xfmd-20260927.
