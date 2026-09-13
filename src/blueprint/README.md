# Blueprint-register

Status: Implemented. Fire features og elleve functionality-objekter er implementert
i første leveranse. Samlet bevis og begrensninger: [P7](../../docs/evidence/P7.md). [Arbeidsmåten](../../docs/working-method.md)
definerer begrepene; [malene](templates/README.md) brukes ved nye objekter.

## Designobjekter

| ID | Navn | Type / målgruppe | Eier | Bruker functionality |
| --- | --- | --- | --- | --- |
| [FTR-001](feature/Feature-001--Markdown-Presentation.md) | Lesbar Markdown-presentasjon | Feature / User | application | FUNC-001, FUNC-002, FUNC-003, FUNC-004, FUNC-005, FUNC-007 |
| [FTR-002](feature/Feature-002--Live-Preview.md) | Revisjonssikker live preview | Feature / User | application | FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006, FUNC-007, FUNC-011 |
| [FTR-003](feature/Feature-003--Document-Navigation.md) | Lokale dokumentreiser med historikk | Feature / User | application | FUNC-001, FUNC-002, FUNC-005, FUNC-008, FUNC-009, FUNC-010 |
| [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | Synkronisert scrolling mellom kilde og visning | Feature / User | application | FUNC-004, FUNC-005, FUNC-009, FUNC-010 |
| [FUNC-001](functionality/Functionality-001--Document-Session.md) | Dokumentøkt og transaksjoner | Functionality / System | application | FUNC-002 |
| [FUNC-002](functionality/Functionality-002--Local-File-Storage.md) | Lokal lagring og inputpolicy | Functionality / System | application | — |
| [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md) | Markdown-tolkning og semantisk modell | Functionality / System | interpreter | — |
| [FUNC-004](functionality/Functionality-004--Render-Layout.md) | Layout og visuell dokumentmodell | Functionality / System | renderer | — |
| [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md) | FOX-host for presentasjon | Functionality / System | application | FUNC-004 |
| [FUNC-006](functionality/Functionality-006--Event-Scheduling.md) | Debounce og hendelseslevetid | Functionality / System | application | — |
| [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md) | Preview-orkestrering | Functionality / System | application | FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006 |
| [FUNC-008](functionality/Functionality-008--Navigation-History.md) | Dokumentnavigasjon og historikk | Functionality / User | application | FUNC-001, FUNC-009 |
| [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md) | Kildeankre og viewport-koordinering | Functionality / System | application | FUNC-005 |
| [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | Arbeidsflate, kommandoer og sidepanel | Functionality / User | application | FUNC-001, FUNC-005, FUNC-007 |
| [FUNC-011](functionality/Functionality-011--Text-Editing.md) | Tekstredigering, undo og søk | Functionality / User | application | FUNC-001, FUNC-005, FUNC-007 |

| [FTR-005](feature/Feature-005--Work-Path-Navigation.md) | Arbeidsområder med historikk | Feature / User | application | FUNC-010, FUNC-012, FUNC-013 |
| [FUNC-012](functionality/Functionality-012--Work-Path-History.md) | Arbeidsrot og historikk | Functionality / User | application | none |
| [FUNC-013](functionality/Functionality-013--Filtered-File-Tree.md) | Filnavnfilter og treinnhold | Functionality / User | application | none |

Feature-eier er eier av ende-til-ende-koordineringen, ikke av all kode i featuren.
Hver functionality har ett lag som kontrakteier. Rendererens tjenester ligger i
renderer selv om orkestreringen ligger i application.

## Krav → designobjekter → akseptansetester

Definisjoner: [kravspesifikasjonen](../../xfmd_requirements.md). AT-er er
akseptansekriterier med bevis i faserapportene; les P7 for faktisk dekning. Future-rader er eksplisitt utsatt.

| Krav | Designobjekter | Akseptanse | Scope |
| --- | --- | --- | --- |
| UR-001 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-003](feature/Feature-003--Document-Navigation.md) | AT-001 | FirstRelease |
| UR-002 | [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-002 | FirstRelease |
| UR-003 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FTR-002](feature/Feature-002--Live-Preview.md) | AT-003 | FirstRelease |
| UR-004 | [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FTR-002](feature/Feature-002--Live-Preview.md) | AT-004 | FirstRelease |
| UR-005 | [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FTR-003](feature/Feature-003--Document-Navigation.md) | AT-005 | FirstRelease |
| UR-006 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-006 | FirstRelease |
| UR-007 | [FUNC-010](functionality/Functionality-010--Workspace-Controls.md) | AT-007 | FirstRelease |
| UR-008 | [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-008 | FirstRelease |
| UR-009 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md) | AT-009 | FirstRelease |
| UR-010 | Deferred: egen IPC-blueprint opprettes før arbeid | AT-010 | Future |
| UR-011 | [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md) | AT-025 | FirstRelease |
| UR-012 | [FTR-005](feature/Feature-005--Work-Path-Navigation.md), [FUNC-012](functionality/Functionality-012--Work-Path-History.md) | AT-026 | FirstRelease |
| UR-013 | [FTR-005](feature/Feature-005--Work-Path-Navigation.md), [FUNC-012](functionality/Functionality-012--Work-Path-History.md) | AT-027 | FirstRelease |
| UR-014 | [FTR-005](feature/Feature-005--Work-Path-Navigation.md), [FUNC-013](functionality/Functionality-013--Filtered-File-Tree.md) | AT-028 | FirstRelease |
| SR-001 | [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-011 | FirstRelease |
| SR-002 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-012 | FirstRelease |
| SR-003 | [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-013 | FirstRelease |
| SR-004 | [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md) | AT-014 | FirstRelease |
| SR-005 | [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-003](feature/Feature-003--Document-Navigation.md) | AT-015 | FirstRelease |
| SR-006 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md) | AT-016 | FirstRelease |
| SR-007 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md) | AT-017 | FirstRelease |
| SR-008 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-018 | FirstRelease |
| SR-009 | [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-019 | FirstRelease |
| SR-010 | [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FTR-002](feature/Feature-002--Live-Preview.md) | AT-020 | FirstRelease |
| SR-011 | [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md) | AT-021 | FirstRelease |
| SR-012 | [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-022 | FirstRelease |
| SR-013 | [FUNC-001](functionality/Functionality-001--Document-Session.md), [FUNC-002](functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](functionality/Functionality-004--Render-Layout.md), [FUNC-005](functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](functionality/Functionality-007--Preview-Pipeline.md), [FUNC-008](functionality/Functionality-008--Navigation-History.md), [FUNC-009](functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](functionality/Functionality-010--Workspace-Controls.md), [FUNC-011](functionality/Functionality-011--Text-Editing.md), [FTR-001](feature/Feature-001--Markdown-Presentation.md), [FTR-002](feature/Feature-002--Live-Preview.md), [FTR-003](feature/Feature-003--Document-Navigation.md), [FTR-004](feature/Feature-004--Synchronized-Scrolling.md) | AT-023 | FirstRelease |
| SR-014 | Deferred: egen IPC-blueprint opprettes før arbeid | AT-024 | Future |

## Hvor skal neste endring begynne?

- Ny Markdown-semantikk: FUNC-003, modellkontrakt og FUNC-004, deretter FTR-001.
- Ny tegne-/fontatferd: FUNC-004; FOX-binding/metrics: FUNC-005.
- Åpne/lagre eller ny dokumentkilde: FUNC-001/FUNC-002.
- Ny editorhandling: FUNC-011; preview-trigger: FUNC-007/FUNC-006.
- Lenker/historikk: FUNC-008; posisjonskonvertering/restore: FUNC-009.
- Ny knapp eller panelets oppførsel: FUNC-010; handlingen delegeres til riktig eier.

Søk først etter gjenbruk, deretter oppdater krav og berørte plumbing-tabeller.
Rene implementasjonsdetaljer trenger ikke eget blueprint. Nye ID-er reserveres her.
