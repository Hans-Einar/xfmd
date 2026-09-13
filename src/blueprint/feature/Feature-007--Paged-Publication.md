---
id: FTR-007
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: Future
requirements: UR-017, UR-018, SR-001, SR-008, SR-016, SR-017, SR-019
uses: FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-009, FUNC-010, FUNC-014, FUNC-016, FUNC-017, FUNC-018
---

# Feature-007: Sidevisning og trofast PDF-utgivelse

## 1. Hensikt og avgrensning

Gi brukeren en sidevisning som faktisk forutsier den eksporterte PDF-en. Papirpreview og eksport samles som én feature fordi den sentrale akseptansen er samsvar mellom dem. FTR-001 utvides fortsatt med en ny lesemodus.

## 2. Krav og akseptanse

UR-017, UR-018, SR-001, SR-008, SR-016, SR-017, SR-019. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-011, AT-018, AT-031, AT-032, AT-036, AT-037, AT-039.

## 3. Kontrakter og eierskap

Bruker velger Window wrap eller A4. FUNC-004/017 produserer layout, FUNC-016 sikrer felles glyphgrunnlag, FUNC-005 transformerer til viewport og FUNC-018 publiserer PDF. SourceSnapshot, PaperSpec og FontSetId fryses ved eksportstart; ingen ny dokumentøkt eller automatisk lagring.

## 4. Atferd, tilstand og feil

A4-visning har page gap og valg av fit-width/100 %; zoom endrer kun visning. Window wrap reflower ved breddeendring. Eksport fra begge moduser bruker samme A4-profil. Live preview og sync fortsetter å virke; lagret kildeanker overlever formatbytte. Feilet layout/eksport beholder editoren og gir forklaring.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `View A4 / Window wrap` | `PreviewCoordinator::setLayoutProfile` | `src/application/preview/PreviewCoordinator.cpp` | profil → ny FrameKey | gammelt frame gjøres ikke interaktivt | Implemented |
| 2 | `MarkdownRenderer::layout` | `PageComposer::compose` | `src/renderer/PageComposer.cpp` | flyt/papir → paged frame | A4-constraints og sideankre | Implemented |
| 3 | `File Export PDF` | `ExportCoordinator::start` | `src/application/export/ExportCoordinator.cpp` | frosset buffer/profil → jobb | eksport endrer ikke dirty | Implemented |
| 4 | `Page frame + viewport` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | FrameKey → view transform | zoom er ikke papir-reflow | Implemented |

## 6. Gjenbruk og avhengigheter

Gjenbruk de eksisterende interpreter-/renderer-portene og source mapping. Eksport bruker dokumenttjenester direkte, ikke previewens private parserjobb. FUNC-016/017 er felles for sidevisning og PDF og hindrer to typografi-/pagineringmotorer.

## 7. Verifikasjon

WYSIWYG betyr samme sideformat, linje-/sideskift, glyphposisjoner og innhold ved samme profil/token/fontsett. Skjermens antialiasing trenger ikke være pikselidentisk med en PDF-leser. Test hele dokumentet, ikke bare synlig side.

AT-011, AT-018, AT-031, AT-032, AT-036, AT-037, AT-039: se [P11](../../../docs/evidence/P11.md) og [P12](../../../docs/evidence/P12.md).

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.2: P11/P12 implementert; se [bevis](../../../docs/evidence/P12.md).
P9 proof, P11 sidevisning, P12 eksport. Ingen Verified-status før uavhengig PDF-leser og font-/layoutkontraktene er testet.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.
