---
id: FUNC-NNN
kind: Functionality
audience: <User|System|Integration>
role: <Workflow|Service|Adapter|Mechanism>
owner: <application|interpreter|renderer>
status: Proposed
scope: <FirstRelease|Future>
requirements: <UR-NNN, SR-NNN>
uses: <FUNC-NNN or none>
---

# Functionality-NNN: <Name>

## 1. Purpose and scope

Describe the coherent outcome or bounded service, its owner, use cases and limits.
Do not create an object for each implementation method.

## 2. Requirements and acceptance

List stable UR/SR IDs and link requirements. Explain observable outcomes and AT IDs;
a use case provides context, not a replacement for requirements.

## 3. Contracts and ownership

State public entry points, inputs/results, invariants, side effects, lifetimes,
units, revisions and state ownership. Features reference functionality contracts
instead of duplicating APIs or requiring a feature class.

## 4. Behavior, state and failures

Describe normal flow, pre/postconditions, commit point, cancellation/rollback,
stale data, repeated events and resource limits. Explain inapplicable categories.

## 5. Plumbing

Describe actual planned or implemented calls, including callback direction.
Split into named flows when needed. Validate library calls against the selected version.

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `<Caller::method / Event>` | `<Owner::method>` | `src/<owner>/<File>.cpp` | <Input → output> | <Failure/side effect> | Planned |

Mark Implemented only when the file/symbol exists and call direction is reviewed.

## 6. Reuse and dependencies

Link consumed functionality and identify consumers, public/internal boundaries and
reuse justification. Do not call another feature's private implementation.

## 7. Verification

List AT IDs, fixtures, expected outcomes and unit/contract/integration/GUI checks.
Distinguish planned tests from execution. For execution, record command, environment,
commit and result with evidence links. Verified requires `Evidence: <identified evidence>`.

## 8. Status, risks and change impact

Identify revision/date, open decisions, dependencies and consequences for consumers.
Use working-method status gates, not template completion. Preserve older evidence
and identify the scope that the new revision supersedes.
