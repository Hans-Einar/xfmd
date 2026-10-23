# PLAN-XFMD-0007 — Standalone Mermaid, heading links and Release build

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0007 |
| project | XFMD |
| state | active |
| PlanType | ImplementationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| Source cards | KB-XFMD-023, KB-XFMD-024 |
| Session | [SESSION-XFMD-0002](../../Sessions/session-%230002--Standalone-Mermaid-files.md) |

## Outcome and authority

Owner Session 0002 T002, 2026-10-04, authorizes implementation of #023 and building
a release. Reuse T001 source analysis. Deliver direct .mmd opening, raw source
editing/saving, preview and shared PDF rendering; build an optimized local release
candidate/package. Publication, remote tags and unrelated #020 syntax expansion
are outside this assignment. Product version remains the owner's selected 0.4;
package version is 0.4.0 with explicit development provenance.

## Selected design before code

Recognize .mmd case-insensitively; .mermaid remains ordinary text until separately
selected. The application identifies input type; the interpreter receives an
explicit standaloneMermaid flag in LoadedDocument/SourceSnapshot and does not
inspect paths or access files. Retain the existing plainText contract for other
consumers. Application-generated flags are mutually exclusive. Save As recomputes
both flags and advances the document revision if either changes, invalidating
cached results even for .md to .mmd (both non-plain-text).

CmarkInterpreter creates one source-anchored block and calls existing
MermaidBlockBuilder directly with the raw text (skip BOM for parsing only). No
synthetic Markdown fence or byte-offset adjustment is written to the document.
The source range covers original bytes; parser failure retains raw source plus
existing diagnostics. Diagram preparation/layout/preview/PDF continue unchanged.

Successful .mmd document opening selects Preview in the shared document-open
callback, covering CLI, dialogs, sidebar/history/favorites, navigation and SDP
open targets. Dirty Cancel leaves the old document and view mode untouched.
Editor and Split remain user choices after opening. The .md-only sidebar filter
keeps its existing meaning; .mmd is visible with that filter off. Open dialog adds
a Mermaid filter. No new desktop MIME registration or external renderer.

## Git and delivery policy

Current branch sprint/009/phase/057-favorites is an existing work branch. Preserve
shared uncommitted SDP-navigation/toolkit/model work and the staged #020 card.
Milestone commits remain intended for coherent integration; do not commit unrelated
baseline bytes merely to label this candidate clean. Record HEAD plus baseline,
exact source hashes and dirty artifacts. Local Release build/package is authorized;
no public-release gate, clean commit, merge, install or publication is implied.

## Phase and milestones

| Milestone | Observable acceptance | State |
| --- | --- | --- |
| M1 | Requirements and textual blueprints reconciled, bounded input contract selected | completed before code |
| M2 | .mmd native open/edit/save/preview plus T003 heading links | completed; MM-001 |
| M3 | Exact-candidate unit/native/PDF/regression evidence and Release TGZ | completed locally; MM-001 |

UR-047 / AT-076 extend FUNC-002 and FUNC-023 with native opening through existing
application plumbing. Owner's T005 model-source hold (Session 0001 / #022) remains
explicit: no authored SDL/SDUI or generated model views are edited. This is an
acknowledged model reconciliation debt, not a conformance claim.

## Verification and closeout

Use the owner's captured VP01 fixture. Check raw bytes/ranges/BOM, lowercase and
uppercase extension, invalid syntax fallback, .txt/.md regression, Save As format
changes, dirty Cancel and native entry points/history. Confirm actual diagram
scene and PDF rendering, not just parser exit. Full configured CTest and layer/
blueprint/board checks follow the focused tests. Build type must be Release;
extract package and smoke its executable in isolated Xvfb with a temporary HOME.
Record package/binary/source hashes, environment and build identity.

No independent reviewer is currently authorized by host delegation rules. Same-
context verification cannot satisfy the separate independent-review obligation;
retain that boundary before public release. Do all authorized implementation and
local packaging now. Record actual dispositions/evidence in MM-001 and Session.

## T003 scope extension — local heading links, selected before code

Owner 2026-10-04 requests continuation and `/somePath/someFile.md#SomeLocationInTheFile`.
KB-XFMD-024 / UR-048 / AT-077 joins this candidate. Existing resourcePath rejects
fragments; the navigator has a separate restricted matcher. Share link splitting
and heading resolution without changing filesystem opening APIs: split at literal
# before percent-decoding path and fragment separately. Encoded %23 remains a
filename character. Queries and unsafe schemes/control bytes remain rejected.

Resolve fragments against parsed headings: exact displayed title (including
CamelCase), or a lowercase ASCII slug with whitespace replaced by hyphens,
punctuation removed and repeated IDs suffixed -1, -2. Preserve Unicode characters
without claiming full Unicode case folding or HTML explicit-ID support. Empty #
means document start. Missing heading reports a diagnostic after opening the file.
Same-file hops use the live buffer; another file uses the existing dirty transaction
and history. A pending anchor belongs to the accepted document token and is cleared
on another open/edit so asynchronous parsing cannot apply it to the wrong document.
Native HTTP(S)/SDP action dispatch and system-default opening remain separate.
The index References list retains its documented file-grouping/source-location
semantics; normal preview links and navigator links follow the fragment.

## T003 delivery checkpoint

Implementation and local optimized Release package complete. [MM-001](evidence/MM-001/README.md)
records 85/85 CTest PASS, focused native/PDF checks, package extraction/startup,
source/binary/package hashes and preserved model/concurrent work. Current plan
state remains active for independent/owner review; cards #023/#024 are gate-review.
Milestone integration commits are deferred to avoid absorbing unrelated shared
baseline work; candidate-source archive and tracked patch preserve the actual
result. No clean commit, installation, publication or model completeness claimed.

## T004 installation — 2026-10-05

Owner authorized installation of the prepared candidate. [MM-002](evidence/MM-002/README.md)
records checksum-verified package installation, all-file hash verification, backup
and installed native startup. Prior no-install statements describe T003's boundary;
T004 supersedes that boundary only. Review/acceptance remains pending; no public
release or product/model source change occurred.
