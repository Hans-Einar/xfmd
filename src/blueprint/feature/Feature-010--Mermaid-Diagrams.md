---
id: FTR-010
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023, SR-024
uses: FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-016, FUNC-017, FUNC-018, FUNC-021, FUNC-023, FUNC-024, FUNC-025
---

# Feature-010: Mermaid diagrams

## 1. Purpose and scope

UC-010: read/edit diagram source and publish supported diagrams to PDF. This is
a coherent presentation capability with syntax/failure policy and acceptance,
not a feature class. Flowchart was the first profile; later typed profiles are
listed in the current coverage matrix. Not all Mermaid syntax is supported.
UC-010 corrects the accidental reuse of the existing A4/PDF use case UC-008.

## 2. Requirements and acceptance

UR-039, UR-040, UR-041, SR-021, SR-022, SR-023, SR-024;
AT-059, AT-060, AT-061, AT-062, AT-063, AT-064, AT-065.
See [requirements](../../../xfmd_requirements.md), the
[original integration design](../../../docs/design/mermaid-integration.md) and
[current bounded coverage](../../../mermaid_coverage.md).

## 3. Contracts and ownership

Use IDiagramInterpreter, IDiagramLayout and DiagramPreparation. DiagramModel is
a semantic value, DiagramScene a visual value; application owns jobs and FOX/Cairo.
cmark keeps the Markdown role. Features do not call each other's private methods.
Library-internal types never cross layer boundaries.

## 4. Behavior, state and failures

Edit → debounce → interpretation → preparation → document layout → presentation.
Publish only complete results for the current token/font/profile. Block errors
show source and explanation while other content survives. Color changes repaint;
PDF uses the same scene geometry. Since P31, library SVG replaces separate native
diagram-label drawing/selection; ordinary Markdown selection remains available.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ModelBuilder::appendNode` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | mermaid fence → source/model | ordinary code remains literal | Implemented |
| 2 | `MermaidBlockBuilder::build` | `IDiagramInterpreter::parse` | `src/contracts/diagram/IDiagramInterpreter.h` | UTF-8 → DiagramModel/diagnostic | local fallback | Implemented |
| 3 | DiagramServices prepare chain | `DiagramPreparation::prepare` | `src/application/diagrams/DiagramPreparation.cpp` | model/request key → prepared scene | discard stale/canceled results | Implemented |
| 4 | `DiagramPreparation::prepare` | `IDiagramLayout::layout` | `src/contracts/diagram/IDiagramLayout.h` | clean model/measured labels → scene | no source parsing | Implemented |
| 5 | `BlockLayout::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | SVG scene → visual run/source anchor | preserve aspect and page boundaries | Implemented |
| 6 | `DisplayListPainter::paint` | `DiagramPainter::paint` | `src/application/adapters/DiagramPainter.cpp` | SVG/palette → librsvg/Cairo | shared preview/PDF presentation | Implemented |

## 6. Reuse and dependencies

FUNC-003/004/005/007 provide interpretation/layout/host/preview; FUNC-016/017/018
provide typography/pages/PDF; FUNC-021 provides ordinary text selection. Dedicated
owners are [interpretation](../functionality/Functionality-023--Mermaid-Interpretation.md),
[diagram layout](../functionality/Functionality-024--Diagram-Layout.md) and
[preparation](../functionality/Functionality-025--Diagram-Preparation.md).

## 7. Verification

Rust profile/ABI checks, DiagramLayoutTest, DiagramPreparationTest, DiagramWorkerTest,
DiagramReadingTest, MermaidGuiTest and MermaidPdfTest cover bounded scenarios.
[P29](../../../docs/evidence/P29.md) is historical baseline evidence;
[P32](../../../docs/evidence/P32.md) covers SVG/routing including AT-065;
[P36](../../../docs/evidence/P36.md) covers the first typed sequence profile.
[P41](../../../docs/evidence/P41.md) and its
[integration report](../../../docs/evidence/P41-integration.md) record later coverage.
Each report retains its actual revision/environment and limitations.

## 8. Status, risks and change impact

Implemented, not full Mermaid compatibility. P31/P32's
[SVG/routing decision](../../../docs/design/mermaid-svg-routing.md) supersedes the
original native-label presentation. P33–P35 refine wrapping, attachment and leaders;
P36–P41 extend typed diagram profiles, including Sequence 2. P41 records 23 bounded
families/deliverable profiles and 29 gallery examples. Preparation limits are 64
blocks, 8 MiB per scene and 64 MiB total scene response. Commits/toolchain/dependencies
are pinned; later documentation does not convert earlier evidence into a new run.
