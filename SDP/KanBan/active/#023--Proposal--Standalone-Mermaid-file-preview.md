# Open and render standalone Mermaid files

| Field | Value |
| --- | --- |
| id | KB-XFMD-023 |
| project | XFMD |
| type | Proposal |
| CardState | gate-review |
| created | 2026-10-03T22:01:27.959980+00:00 |
| source | Owner conversation 2026-10-03; VP01-feature-ModelGovernance.mmd from SDP-vNow |
| next_review | Owner review of MM-001 local Release candidate |
| tags | mermaid, files, preview, editor, SDP |

## Need and source

The owner expects XFMD to render a standalone .mmd file and asks whether support
already exists, to capture missing work in KanBan, and to start a new Session.
The supplied example is ModelGovernance's generated VP01 feature diagram.
[Captured source and inspection](../evidence/2026-10-03-standalone-mermaid/README.md)
retain the exact input and hash without requiring an upstream checkout.

T001 authorized investigation/capture. T002 subsequently selected implementation
and Release build; T003 completed the local candidate. See current outcome below.

## T001 inspected behavior and affected boundary

- src/application/io/InputPolicy.cpp: supportedPath lists .md/.txt; plainText is
  true for every extension except .md, including .mmd.
- src/application/io/FileOpenPolicy.cpp: readable unknown extensions become Text.
  ApplicationOpening.cpp's openTarget opens Text in Editor mode.
- src/interpreter/CmarkInterpreter.cpp: plainText snapshots become literal Code
  blocks before Markdown parsing. Switching to Preview does not make raw .mmd
  into a Mermaid diagram.
- src/interpreter/ModelBuilder.cpp and mermaid/MermaidBlockBuilder.cpp: Mermaid
  interpretation is reached through an explicitly labeled Markdown code fence.
- tests/application/DiagramReadingTest.cpp wraps .mmd fixture contents inside a
  mermaid fence. This tests diagrams embedded in Markdown, not opening .mmd files.

UR-039/FUNC-023 explicitly scope existing Mermaid entry to fenced blocks; this is
an additional document-input workflow, not proof of a broken Mermaid parser.
Source inspection is sufficient to identify the missing entry path; no fresh GUI
run or claim about exact supplied-diagram rendering fidelity was made.

## Proposed scope and acceptance

1. Recognize standalone .mmd as Mermaid source and display its diagram using the
   existing interpreter/contracts/render pipeline. Decide whether .mermaid and
   case-insensitive extension aliases are included in the bounded delivery.
2. Keep the actual file path and raw source as document identity. Source editing,
   saving, dirty handling and history must retain the original content, without
   writing synthetic Markdown fences into the .mmd file.
3. Cover supported entry points: CLI, Open dialog, Files tree/history/favorites,
   local links and applicable SDP open targets. Resolve initial view mode, filters
   and resource-base handling consistently. Do not silently extend every file type.
4. Preserve source fallback and useful diagnostics for incomplete/unsupported
   Mermaid. Keep ordinary .txt literal and fenced Markdown behavior unchanged.
5. Exercise the captured VP01 fixture through actual native opening and preview,
   then source edit/save/reopen, dirty Cancel, Unicode/space paths, and malformed
   input. Verify shared Wrap/A4/zoom and PDF behavior if included in the selected
   design; separate input support from broader syntax/rendering compatibility.

Before code, reconcile requirements and affected file-opening, preview and
Mermaid blueprints. Preserve the current owner hold on SDL/SDUI model-source edits
unless a later instruction changes it. No new parser, native UI toolkit, producer
command or external rendering executable is selected by this proposal.

## Overlap review and next action

[KB-XFMD-020](../active/%23020--Proposal--Mermaid-compatibility-across-pipeline.md) covers
Mermaid grammar/configuration and semantic/render compatibility, not standalone
file admission. It remains a related card and is not superseded. Its staged
concurrent content was left unchanged.
[KB-XFMD-015](../backlog/%23015--Proposal--SDL-design-file-preview.md) covers .design source
paired with generated preview; [KB-XFMD-019](../backlog/%23019--Proposal--Shared-rendering-and-Kitty-output.md)
covers other rendering consumers. Neither specifies direct .mmd opening.
Completed #007 owns the existing text-file routing baseline, which must remain
compatible. No existing primary card directly covers this request.

Continue in [SESSION-XFMD-0002](../../Sessions/session-%230002--Standalone-Mermaid-files.md).
Current next action: review the implemented local candidate and its evidence.
The original registration did not authorize implementation; T002 did.

## Worklog

- 2026-10-03T22:01:27.959980+00:00 — codex; EVT-KB-XFMD-000099: Registered the direct .mmd opening gap,
  captured the supplied source, compared #020/#015/#019/#007 and created Session
  0002. Inspection only; no native implementation or product acceptance claimed.

## T002 execution — 2026-10-04

Owner authorizes implementation and a local Release build. Execute
[PLAN-XFMD-0007](../../05--Implementation/Standalone-Mermaid/Plan.md); prior planning-
only paragraphs describe T001. Case-insensitive .mmd is selected, .mermaid is not.
Source identity/save bytes and the owner model-source hold remain invariants.

## T003 outcome — 2026-10-04

Implemented case-insensitive .mmd admission, raw source preservation, shared native
preview/PDF and format-changing Save As invalidation. [MM-001](../../05--Implementation/Standalone-Mermaid/evidence/MM-001/README.md)
records 85/85 regression checks, focused workflows and exact optimized package.
Native diagram and PDF inspected. Active / gate-review pending owner and
independent review; same-context tests are not approval. No installation or public
release. #020 remains unchanged; model-source reconciliation remains held.

## T004 installation — 2026-10-05

Owner authorized installation of the prepared candidate. [MM-002](../../05--Implementation/Standalone-Mermaid/evidence/MM-002/README.md)
records checksum-verified package installation, all-file hash verification, backup
and installed native startup. Prior no-install statements describe T003's boundary;
T004 supersedes that boundary only. Review/acceptance remains pending; no public
release or product/model source change occurred.
