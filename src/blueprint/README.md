# Blueprint-register

P0–P23 omfatter 9 features og 22 functionality-objekter.
Registerstatus speiler metadata i blueprint-filene. Implemented betyr at koden
finnes; [P15-bevis](../../docs/evidence/P15.md) angir faktisk testdekning og
begrensninger. UR-010/SR-014 (xfw-IPC) er fortsatt Future.

Se [designrevisjonen](../../softwareDesign.md), [arbeidsmåten](../../docs/working-method.md),
[malene](templates/README.md), [P7-bevis](../../docs/evidence/P7.md) og [P8-bevis](../../docs/evidence/P8.md).

## Designobjekter

| ID | Type / målgruppe | Eier | Status | Bruker functionality |
| --- | --- | --- | --- | --- |
| [FTR-001](feature/Feature-001--Markdown-Presentation.md) | Feature / User | application | Implemented | FUNC-001, FUNC-002, FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-010, FUNC-016, FUNC-017, FUNC-021, FUNC-022 |
| [FTR-002](feature/Feature-002--Live-Preview.md) | Feature / User | application | Implemented | FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006, FUNC-007, FUNC-011 |
| [FTR-003](feature/Feature-003--Document-Navigation.md) | Feature / User | application | Implemented | FUNC-001, FUNC-002, FUNC-005, FUNC-008, FUNC-009, FUNC-010 |
| [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | Feature / User | application | Implemented | FUNC-004, FUNC-005, FUNC-009, FUNC-010, FUNC-015 |
| [FTR-005](feature/Feature-005--Work-Path-Navigation.md) | Feature / User | application | Implemented | FUNC-010, FUNC-012, FUNC-013 |
| [FTR-006](feature/Feature-006--Configurable-Scrolling.md) | Feature / User | application | Implemented | FUNC-005, FUNC-009, FUNC-010, FUNC-011, FUNC-014, FUNC-015 |
| [FTR-007](feature/Feature-007--Paged-Publication.md) | Feature / User | application | Implemented | FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-009, FUNC-010, FUNC-014, FUNC-016, FUNC-017, FUNC-018 |
| [FUNC-001](functionality/Functionality-001--Document-Session.md) | Functionality / System | application | Implemented | FUNC-002 |
| [FUNC-002](functionality/Functionality-002--Local-File-Storage.md) | Functionality / System | application | Implemented | none |
| [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md) | Functionality / System | interpreter | Implemented | none |
| [FUNC-004](functionality/Functionality-004--Render-Layout.md) | Functionality / System | renderer | Implemented | FUNC-017 |
| [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md) | Functionality / System | application | Implemented | FUNC-004, FUNC-015, FUNC-016, FUNC-021 |
| [FUNC-006](functionality/Functionality-006--Event-Scheduling.md) | Functionality / System | application | Implemented | none |
| [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md) | Functionality / System | application | Implemented | FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006, FUNC-022 |
| [FUNC-008](functionality/Functionality-008--Navigation-History.md) | Functionality / User | application | Implemented | FUNC-001, FUNC-009 |
| [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md) | Functionality / System | application | Implemented | FUNC-005 |
| [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | Functionality / User | application | Implemented | FUNC-001, FUNC-005, FUNC-007, FUNC-012, FUNC-013, FUNC-014, FUNC-015, FUNC-018, FUNC-019 |
| [FUNC-011](functionality/Functionality-011--Text-Editing.md) | Functionality / User | application | Implemented | FUNC-001, FUNC-005, FUNC-007, FUNC-015 |
| [FUNC-012](functionality/Functionality-012--Work-Path-History.md) | Functionality / User | application | Implemented | none |
| [FUNC-013](functionality/Functionality-013--Filtered-File-Tree.md) | Functionality / User | application | Implemented | FUNC-012 |
| [FUNC-014](functionality/Functionality-014--Application-Preferences.md) | Functionality / User | application | Implemented | none |
| [FUNC-015](functionality/Functionality-015--Scroll-Motion.md) | Functionality / System | application | Implemented | FUNC-014, FUNC-006 |
| [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md) | Functionality / System | application | Implemented | none |
| [FUNC-017](functionality/Functionality-017--Page-Composition.md) | Functionality / System | renderer | Implemented | none |
| [FUNC-018](functionality/Functionality-018--PDF-Export.md) | Functionality / User | application | Implemented | FUNC-001, FUNC-003, FUNC-004, FUNC-006, FUNC-016, FUNC-017, FUNC-022 |
| [FUNC-019](functionality/Functionality-019--Application-Identity.md) | Functionality / Integration | application | Implemented | none |
| [FUNC-020](functionality/Functionality-020--Document-Index.md) | Functionality / System | application | Implemented | FUNC-002, FUNC-003, FUNC-008 |
| [FTR-008](feature/Feature-008--Chapter-Navigation.md) | Feature / User | application | Implemented | FUNC-020, FUNC-008, FUNC-009, FUNC-010 |
| [FTR-009](feature/Feature-009--Document-References.md) | Feature / User | application | Implemented | FUNC-020, FUNC-008, FUNC-009, FUNC-010 |

## Krav → designobjekter → akseptansetester

[Normative krav](../../xfmd_requirements.md). Akseptanse er ikke automatisk testbevis.

| Krav | Designobjekter | Akseptanse | Scope |
| --- | --- | --- | --- |
| UR-001 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-001 | FirstRelease |
| UR-002 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md) | AT-002 | FirstRelease |
| UR-003 | [FTR-002](feature/Feature-002--Live-Preview.md), [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md) | AT-003 | FirstRelease |
| UR-004 | [FTR-002](feature/Feature-002--Live-Preview.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md) | AT-004 | FirstRelease |
| UR-005 | [FTR-003](feature/Feature-003--Document-Navigation.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md) | AT-005 | FirstRelease |
| UR-006 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-006 | FirstRelease |
| UR-007 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-007 | FirstRelease |
| UR-008 | [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md) | AT-008 | FirstRelease |
| UR-009 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md) | AT-009 | FirstRelease |
| UR-010 | Deferred: egen IPC-blueprint før arbeid | AT-010 | Future |
| UR-011 | [FTR-006](feature/Feature-006--Configurable-Scrolling.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-015](functionality/Functionality-015--Scroll-Motion.md) | AT-025 | FirstRelease |
| UR-012 | [FTR-005](feature/Feature-005--Work-Path-Navigation.md), [FUNC-012](functionality/Functionality-012--Work-Path-History.md) | AT-026 | FirstRelease |
| UR-013 | [FTR-005](feature/Feature-005--Work-Path-Navigation.md), [FUNC-012](functionality/Functionality-012--Work-Path-History.md) | AT-027 | FirstRelease |
| UR-014 | [FTR-005](feature/Feature-005--Work-Path-Navigation.md), [FUNC-013](functionality/Functionality-013--Filtered-File-Tree.md) | AT-028 | FirstRelease |
| UR-015 | [FTR-006](feature/Feature-006--Configurable-Scrolling.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md), [FUNC-015](functionality/Functionality-015--Scroll-Motion.md) | AT-029 | FirstRelease |
| UR-016 | [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FTR-006](feature/Feature-006--Configurable-Scrolling.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-015](functionality/Functionality-015--Scroll-Motion.md) | AT-030 | FirstRelease |
| UR-017 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md) | AT-031 | FirstRelease |
| UR-018 | [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md) | AT-032 | FirstRelease |
| UR-019 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-033 | FirstRelease |
| UR-020 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-019](functionality/Functionality-019--Application-Identity.md) | AT-034 | FirstRelease |
| SR-001 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md) | AT-011 | FirstRelease |
| SR-002 | [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FTR-006](feature/Feature-006--Configurable-Scrolling.md), [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md), [FUNC-015](functionality/Functionality-015--Scroll-Motion.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md) | AT-012 | FirstRelease |
| SR-003 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md) | AT-013 | FirstRelease |
| SR-004 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md) | AT-014 | FirstRelease |
| SR-005 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md) | AT-015 | FirstRelease |
| SR-006 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md) | AT-016 | FirstRelease |
| SR-007 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md) | AT-017 | FirstRelease |
| SR-008 | [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md) | AT-018 | FirstRelease |
| SR-009 | [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md) | AT-019 | FirstRelease |
| SR-010 | [FTR-002](feature/Feature-002--Live-Preview.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md) | AT-020 | FirstRelease |
| SR-011 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md) | AT-021 | FirstRelease |
| SR-012 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-022 | FirstRelease |
| SR-013 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-019](functionality/Functionality-019--Application-Identity.md) | AT-023 | FirstRelease |
| SR-014 | Deferred: egen IPC-blueprint før arbeid | AT-024 | Future |
| SR-015 | [FTR-006](feature/Feature-006--Configurable-Scrolling.md), [FUNC-015](functionality/Functionality-015--Scroll-Motion.md) | AT-035 | FirstRelease |
| SR-016 | [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md) | AT-036 | FirstRelease |
| SR-017 | [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md) | AT-037 | FirstRelease |
| SR-018 | [FUNC-014](functionality/Functionality-014--Application-Preferences.md) | AT-038 | FirstRelease |
| SR-019 | [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md), [FTR-006](feature/Feature-006--Configurable-Scrolling.md), [FTR-007](feature/Feature-007--Paged-Publication.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-015](functionality/Functionality-015--Scroll-Motion.md), [FUNC-017](functionality/Functionality-017--Page-Composition.md) | AT-039 | FirstRelease |
| UR-021 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-020](functionality/Functionality-020--Document-Index.md), [FTR-008](feature/Feature-008--Chapter-Navigation.md) | AT-040 | FirstRelease |
| UR-022 | [FUNC-020](functionality/Functionality-020--Document-Index.md), [FTR-008](feature/Feature-008--Chapter-Navigation.md) | AT-041 | FirstRelease |
| UR-023 | [FUNC-020](functionality/Functionality-020--Document-Index.md), [FTR-009](feature/Feature-009--Document-References.md) | AT-042 | FirstRelease |
| SR-020 | [FUNC-020](functionality/Functionality-020--Document-Index.md), [FTR-009](feature/Feature-009--Document-References.md) | AT-043 | FirstRelease |
| UR-024 | [FUNC-014](functionality/Functionality-014--Application-Preferences.md) | AT-044 | FirstRelease |
| UR-025 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md) | AT-045 | FirstRelease |
| UR-026 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-046 | FirstRelease |
| UR-027 | [FUNC-014](functionality/Functionality-014--Application-Preferences.md) | AT-047 | FirstRelease |

| UR-028 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-048 | FirstRelease |
| UR-029 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-049 | FirstRelease |

## P21 kravdekning

| Krav | Designobjekter | Akseptanse | Scope |
| --- | --- | --- | --- |
| UR-030 | [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md) | AT-050 | FirstRelease |
| UR-031 | [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md) | AT-051 | FirstRelease |

P22: UR-032 → FUNC-010/014; UR-033 → FUNC-010; UR-034 → FUNC-005. AT-052–054.

| Krav | Implementerende objekter | Akseptanse | Scope |
| --- | --- | --- | --- |
| UR-032 | [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-014](functionality/Functionality-014--Application-Preferences.md) | AT-052 | FirstRelease |
| UR-033 | [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-053 | FirstRelease |
| UR-034 | [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md) | AT-054 | FirstRelease |

## P23

| ID | Type / målgruppe | Eier | Status | Bruker functionality |
| --- | --- | --- | --- | --- |
| [FUNC-021](functionality/Functionality-021--Preview-Text-Selection.md) | Functionality / System | application | Implemented | none |
| [FUNC-022](functionality/Functionality-022--Embedded-Visuals.md) | Functionality / System | application | Implemented | FUNC-008 |

| Krav | Designobjekter | Akseptanse | Scope |
| --- | --- | --- | --- |
| UR-035 | [FUNC-021](functionality/Functionality-021--Preview-Text-Selection.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-055 | FirstRelease |
| UR-036 | [FUNC-022](functionality/Functionality-022--Embedded-Visuals.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-056 | FirstRelease |
| UR-037 | [FUNC-022](functionality/Functionality-022--Embedded-Visuals.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-016](functionality/Functionality-016--Typography-And-Drawing.md), [FUNC-018](functionality/Functionality-018--PDF-Export.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-057 | FirstRelease |
