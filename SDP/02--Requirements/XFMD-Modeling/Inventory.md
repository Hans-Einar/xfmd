# Generated XFMD modeling inventory

Regenerate with `python3 SDP/02--Requirements/XFMD-Modeling/build_inventory.py`.
See [Baseline](Baseline.md) for interpretation and [inventory.json](inventory.json)
for every identity occurrence, source file and input hash. This is an inventory,
not an SDL-generated viewpoint or a semantic completeness claim.

Baseline HEAD: `41fa4948c5377edd792ba3f503e6b92b0fd4717a`; current input hashes are in the JSON.

38 blueprints; 293 C++/Rust source and manifest files.

| Blueprint | Title | Declared owner | Requirement IDs | Model status |
| --- | --- | --- | --- | --- |
| [FTR-001](../../../src/blueprint/feature/Feature-001--Markdown-Presentation.md) | Markdown-Presentation | application | UR-035, UR-036, UR-037, UR-001, UR-002, UR-009, SR-001, SR-003, SR-004, SR-005, SR-011, SR-012, SR-013, UR-017 | structural-model-linked |
| [FTR-002](../../../src/blueprint/feature/Feature-002--Live-Preview.md) | Live-Preview | application | UR-003, UR-004, UR-009, SR-002, SR-008, SR-010, SR-011, SR-012, SR-013, SR-019 | structural-model-linked |
| [FTR-003](../../../src/blueprint/feature/Feature-003--Document-Navigation.md) | Document-Navigation | application | UR-001, UR-005, UR-009, UR-024, UR-028, SR-002, SR-005, SR-008, SR-012, SR-013, SR-019 | structural-model-linked |
| [FTR-004](../../../src/blueprint/feature/Feature-004--Synchronized-Scrolling.md) | Synchronized-Scrolling | application | UR-008, SR-002, SR-008, SR-009, SR-012, SR-013, UR-016, SR-019 | structural-model-linked |
| [FTR-005](../../../src/blueprint/feature/Feature-005--Work-Path-Navigation.md) | Work-Path-Navigation | application | UR-001, UR-012, UR-013, UR-014 | structural-model-linked |
| [FTR-006](../../../src/blueprint/feature/Feature-006--Configurable-Scrolling.md) | Configurable-Scrolling | application | UR-011, UR-015, UR-016, SR-002, SR-015, SR-019 | structural-model-linked |
| [FTR-007](../../../src/blueprint/feature/Feature-007--Paged-Publication.md) | Paged-Publication | application | UR-017, UR-018, SR-001, SR-008, SR-016, SR-017, SR-019 | structural-model-linked |
| [FTR-008](../../../src/blueprint/feature/Feature-008--Chapter-Navigation.md) | Chapter-Navigation | application | UR-021, UR-022 | structural-model-linked |
| [FTR-009](../../../src/blueprint/feature/Feature-009--Document-References.md) | Document-References | application | UR-023, SR-020 | structural-model-linked |
| [FTR-010](../../../src/blueprint/feature/Feature-010--Mermaid-Diagrams.md) | Mermaid-Diagrams | application | UR-039, UR-040, UR-041, SR-021, SR-022, SR-023, SR-024 | structural-model-linked |
| [FUNC-001](../../../src/blueprint/functionality/Functionality-001--Document-Session.md) | Document-Session | application | UR-001, UR-003, UR-009, SR-002, SR-006, SR-007, SR-008, SR-013 | structural-model-linked |
| [FUNC-002](../../../src/blueprint/functionality/Functionality-002--Local-File-Storage.md) | Local-File-Storage | application | UR-001, UR-003, UR-009, UR-028, SR-005, SR-006, SR-007, SR-011, SR-013 | structural-model-linked |
| [FUNC-003](../../../src/blueprint/functionality/Functionality-003--Markdown-Interpretation.md) | Markdown-Interpretation | interpreter | UR-039, UR-036, UR-037, UR-005, UR-002, SR-001, SR-003, SR-004, SR-005, SR-009, SR-011, SR-013 | structural-model-linked |
| [FUNC-004](../../../src/blueprint/functionality/Functionality-004--Render-Layout.md) | Render-Layout | renderer | UR-040, UR-041, UR-035, UR-036, UR-037, UR-005, UR-002, UR-008, SR-001, SR-003, SR-005, SR-008, SR-009, SR-011, SR-013, UR-017, UR-018, SR-016, SR-019 | structural-model-linked |
| [FUNC-005](../../../src/blueprint/functionality/Functionality-005--FOX-Presentation-Host.md) | FOX-Presentation-Host | application | UR-040, UR-041, UR-035, UR-034, UR-030, UR-031, UR-011, UR-002, UR-005, UR-008, UR-028, SR-001, SR-008, SR-009, SR-010, SR-013, UR-017, SR-016, SR-019 | structural-model-linked |
| [FUNC-006](../../../src/blueprint/functionality/Functionality-006--Event-Scheduling.md) | Event-Scheduling | application | UR-004, SR-008, SR-010, SR-013 | structural-model-linked |
| [FUNC-007](../../../src/blueprint/functionality/Functionality-007--Preview-Pipeline.md) | Preview-Pipeline | application | SR-022, UR-036, UR-037, UR-002, UR-004, SR-001, SR-002, SR-003, SR-008, SR-010, SR-011, SR-013, UR-017, SR-019 | structural-model-linked |
| [FUNC-008](../../../src/blueprint/functionality/Functionality-008--Navigation-History.md) | Navigation-History | application | UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-013 | structural-model-linked |
| [FUNC-009](../../../src/blueprint/functionality/Functionality-009--Source-Anchor-Mapping.md) | Source-Anchor-Mapping | application | UR-005, UR-008, SR-002, SR-008, SR-009, SR-013, UR-017, SR-019 | structural-model-linked |
| [FUNC-010](../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md) | Workspace-Controls | application | UR-045, UR-038, UR-032, UR-033, UR-028, UR-029, UR-025, UR-026, UR-021, UR-011, UR-001, UR-006, UR-007, SR-002, SR-008, SR-013, UR-015, UR-017, UR-018, UR-019, UR-020, SR-019 | structural-model-linked |
| [FUNC-011](../../../src/blueprint/functionality/Functionality-011--Text-Editing.md) | Text-Editing | application | UR-032, UR-033, UR-011, UR-003, UR-004, UR-009, SR-002, SR-006, SR-008, SR-013, UR-016, SR-019 | structural-model-linked |
| [FUNC-012](../../../src/blueprint/functionality/Functionality-012--Work-Path-History.md) | Work-Path-History | application | UR-012, UR-013 | structural-model-linked |
| [FUNC-013](../../../src/blueprint/functionality/Functionality-013--Filtered-File-Tree.md) | Filtered-File-Tree | application | UR-014 | structural-model-linked |
| [FUNC-014](../../../src/blueprint/functionality/Functionality-014--Application-Preferences.md) | Application-Preferences | application | UR-032, UR-030, UR-031, UR-025, UR-027, UR-024, UR-015, UR-017, SR-002, SR-018 | structural-model-linked |
| [FUNC-015](../../../src/blueprint/functionality/Functionality-015--Scroll-Motion.md) | Scroll-Motion | application | UR-011, UR-015, UR-016, SR-002, SR-015, SR-019 | structural-model-linked |
| [FUNC-016](../../../src/blueprint/functionality/Functionality-016--Typography-And-Drawing.md) | Typography-And-Drawing | application | UR-040, UR-041, UR-036, UR-037, UR-030, UR-031, UR-002, UR-017, UR-018, SR-001, SR-010, SR-016 | structural-model-linked |
| [FUNC-017](../../../src/blueprint/functionality/Functionality-017--Page-Composition.md) | Page-Composition | renderer | UR-040, UR-017, UR-018, SR-001, SR-008, SR-009, SR-016, SR-019 | structural-model-linked |
| [FUNC-018](../../../src/blueprint/functionality/Functionality-018--PDF-Export.md) | PDF-Export | application | UR-040, SR-022, UR-036, UR-037, UR-018, SR-002, SR-005, SR-007, SR-010, SR-017 | structural-model-linked |
| [FUNC-019](../../../src/blueprint/functionality/Functionality-019--Application-Identity.md) | Application-Identity | application | UR-020, SR-013 | structural-model-linked |
| [FUNC-020](../../../src/blueprint/functionality/Functionality-020--Document-Index.md) | Document-Index | application | UR-021, UR-022, UR-023, SR-020 | structural-model-linked |
| [FUNC-021](../../../src/blueprint/functionality/Functionality-021--Preview-Text-Selection.md) | Preview-Text-Selection | application | UR-041, UR-035 | structural-model-linked |
| [FUNC-022](../../../src/blueprint/functionality/Functionality-022--Embedded-Visuals.md) | Embedded-Visuals | application | UR-036, UR-037 | structural-model-linked |
| [FUNC-023](../../../src/blueprint/functionality/Functionality-023--Mermaid-Interpretation.md) | Mermaid-Interpretation | interpreter | UR-039, UR-041, SR-021, SR-022, SR-023 | structural-model-linked |
| [FUNC-024](../../../src/blueprint/functionality/Functionality-024--Diagram-Layout.md) | Diagram-Layout | renderer | UR-039, UR-040, UR-041, SR-021, SR-022, SR-023, SR-024 | structural-model-linked |
| [FUNC-025](../../../src/blueprint/functionality/Functionality-025--Diagram-Preparation.md) | Diagram-Preparation | application | UR-039, UR-040, UR-041, SR-021, SR-022, SR-023 | structural-model-linked |
| [FUNC-026](../../../src/blueprint/functionality/Functionality-026--Build-Identity.md) | Build-Identity | application | UR-042, SR-025 | structural-model-linked |
| [FUNC-031](../../../src/blueprint/functionality/Functionality-031--Generated-Document-Navigation.md) | Generated-Document-Navigation | application | UR-044, SR-027, SR-028 | structural-model-linked |
| [FUNC-032](../../../src/blueprint/functionality/Functionality-032--SDP-Project-Navigation.md) | SDP-Project-Navigation | application | UR-046, SR-029 | structural-model-linked |

## Requirement identity inventory

Repeated definitions may be amendments; they are not silently discarded.
Acceptance references are specifications, not proof that checks ran.

| Family | Referenced identities | Table definitions |
| --- | --- | --- |
| UC | 12 | 12 |
| UR | 46 | 45 |
| SR | 29 | 28 |
| AT | 74 | Embedded acceptance clauses |

150 source files have no literal filename mention in the blueprint bodies.
These are candidates for ownership review (GAP-XFMD-DOC-001), not proven missing designs.
Model links cover every existing blueprint structurally. They do not establish full behavioral coverage.
See [Coverage](Coverage.md) for all requirement concerns, amendments and source-family responsibilities.
