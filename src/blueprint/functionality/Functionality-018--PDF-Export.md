---
id: FUNC-018
kind: Functionality
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-040, SR-022, UR-036, UR-037, UR-018, SR-002, SR-005, SR-007, SR-010, SR-017
uses: FUNC-025, FUNC-022, FUNC-001, FUNC-003, FUNC-004, FUNC-006, FUNC-016, FUNC-017
---

# Functionality-018: Revisjonssikker lokal PDF-eksport

## 1. Hensikt og avgrensning

Eie eksportjobben fra frosset buffer/papirprofil til ferdig lokal PDF. Eksport bruker interpreter-/renderer-kontrakter og endrer ikke previewens interne arbeidsflyt eller dokumentets lagrede baseline.

## 2. Krav og akseptanse

UR-018, SR-002, SR-005, SR-007, SR-010, SR-017. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-012, AT-015, AT-017, AT-020, AT-032, AT-037.

## 3. Kontrakter og eierskap

`ExportCoordinator::start/cancel` tar `SourceSnapshot`, `PaperSpec`, `FontSetId` og mål med overskrivingsbeslutning. `ExportRequest` eier kopierte inputs og dokumenttoken; `ExportControl` eier cancel/progress. Injisert Work bygger egne backend-instanser i composition root. `PdfOutput::write(PageLayout,FontSet,target)` utfører FUNC-016s display list med Cairo PDF-kandidat. `PdfFilePublisher::commit` har eksplisitt tempfil/identitets-/feilkontrakt.

Dersom et komplett paged frame har eksakt samme FrameKey, gjenbrukes immutable frame. Ellers brukes IInterpreter::parse/IRenderer::layout gjennom offentlige kontrakter; eksport kaller aldri private PreviewCoordinator-metoder og venter ikke på preview-debounce. En separat interpreter-instans opprettes i composition root dersom parsing skjer parallelt; thread-safety antas ikke fra en delt instance.

## 4. Atferd, tilstand og feil

File → Export PDF… (Ctrl+Shift+E). Fra Window wrap brukes A4-profilen, aldri viewportbredde som PDF-sidebredde. Dialogen angir at gjeldende buffer eksporteres. Senere edits/dokumentbytte påvirker ikke den frosne jobben; status viser eksportert revisjon og om nyere edits finnes. Maks én eksportjobb; knapp deaktiveres til ferdig/cancel. Avbrudd før publisering sletter tempfil; etter commit rapporteres fullført, ikke «avbrutt».

GUI-tråden eier FOX/layout som krever GUI-ressurser. Worker kan parse og skrive PDF med egne ikke-FOX-contexts. Publisering skjer fra søsken-temp etter finalisering/statuskontroll og flush; eksisterende mål endres først ved commit. Målidentitet sjekkes ved commit, med samme dokumenterte eksterne-racebegrensning som vanlig lagring. PDF-jobben bruker ikke DocumentSession::markSaved. PDF inkluderer sidens innhold og marger, ikke toolbar/sidegap/markør. Ressursprofilen setter maks sider/outputstørrelse og cancel-sjekkpunkter etter P9-måling.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `CommandRouter ExportPdf` | `ExportCoordinator::start` | `src/application/export/ExportCoordinator.cpp` | snapshot/profil/mål → job | dirty og historikk uendret | Implemented |
| 2 | `ExportPipeline::run` | `IRenderer::layout` | `src/contracts/IRenderer.h` | immutable model/paged request → frame | riktig token og FontSetId | Implemented |
| 3 | `ExportPipeline::run` | `PdfOutput::write` | `src/application/adapters/PdfOutput.cpp` | hele PageLayout → temp-PDF | finalisering/cancel/feil | Implemented |
| 4 | `ExportPipeline::run` | `PdfFilePublisher::commit` | `src/application/io/PdfFilePublisher.cpp` | temp + forventet mål → publisering | ingen delvis målfil ved feil før commit | Implemented |


P23: Eksport av en ny layout bruker samme EmbeddedVisuals::prepare som preview. Eksisterende A4-frame gjenbruker immutable ressurser. Kopimarkering eksporteres ikke.

| 60 | `ExportPipeline::run` | `EmbeddedVisuals::prepare` | `src/application/media/EmbeddedVisuals.cpp` | Snapshot → forberedte PDF-ressurser | Feil gir plassholder | Implemented |

| 99 | `ExportPipeline::run` | `DiagramPreparation::prepare` | `src/application/diagrams/DiagramPreparation.cpp` | Implementert diagramutvidelse | Blokkfeil og stale-data følger Mermaid-designet | Implemented |

## 6. Gjenbruk og avhengigheter

Gjenbruk FUNC-001 snapshots, FUNC-003 parsing, FUNC-004 layout, FUNC-016 glyphreplay og FUNC-017 sider. FUNC-002s lagringsregler er forbilde; binær publiseringsmekanisme trekkes bare ut som felles privat primitiv når begge reelle konsumenter kan beholde metadata-/dirty-kontraktene.

## 7. Verifikasjon

Fake-output for cancel/feil ved hvert commit-steg; integrasjon leser PDF-sider, MediaBox og tekst med uavhengig leser. Rastrert PDF sammenlignes med samme paged frame ved samme DPI. Test ulagret buffer, edit/bytte under jobb, full disk og bounded shutdown.

AT-012, AT-015, AT-017, AT-020, AT-032, AT-037: se [P11](../../../docs/evidence/P11.md) og [P12](../../../docs/evidence/P12.md).

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.2: P11/P12 implementert; se [bevis](../../../docs/evidence/P12.md).
P12 etter side-/fontgate. Støtter først dagens Markdown-profil; tabeller/bilder/fragmentnavigasjon er ikke implisitt lagt til av eksport.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.

P23 akseptanse: AT-056, AT-057. Tester: RichPreviewTest og PreviewSelectionGuiTest.

P25 (Proposed): Eksport får samme injiserte prepare-kjede og diagramscene som preview; font-key og cancellation inngår. Ingen separat kilde→SVG-eksportvei. Krav: UR-040, SR-022; AT-060, AT-063.
Se [design](../../../docs/design/mermaid-integration.md). Eksisterende Implemented-rader
og eldre bevis gjelder baseline; ny plumbing er ikke implementert eller testet.
