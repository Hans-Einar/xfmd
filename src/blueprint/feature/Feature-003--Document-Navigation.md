---
id: FTR-003
kind: Feature
audience: User
role: Workflow
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-012, SR-013
uses: FUNC-001, FUNC-002, FUNC-005, FUNC-008, FUNC-009, FUNC-010
---

# Feature-003: Lokale dokumentreiser med historikk

## 1. Hensikt og avgrensning

Brukeren følger lokale dokumentlenker og kommer tilbake/frem til riktig fil og leseposisjon. Lenker alene er functionality; lenker med transaksjonell historikk gir den samlede featureverdien.

## 2. Krav og akseptanse

Krav: UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Innganger: LinkActivated, Back og Forward samt øvrige vellykkede dokumentåpninger. Utgang: aktivt dokument med konsistent historikkcursor og gjenopprettet anker. NavigationCoordinator er eneste eier av arbeidsflyten; funksjonene detaljeres i FUNC-008.

## 4. Atferd, tilstand og feil

Hit-test må komme fra gyldig frame. Mål resolves lokalt, nåværende anker tas vare på og vanlig dokumentbytte utføres. Bare suksess committer history. Ny lenke etter tilbake trunkerer frem-gren. Avbrutt dirty-dialog, brutt lenke eller uleselig fil beholder alt. Gjenopprett posisjon først når målfilens frame er klart. Ingen shell/nettverksåpning eller fragmentnavigasjon i første versjon.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FoxRenderHost::onPointer` | `IRenderer::hitTest` | `src/contracts/IRenderer.h` | Gyldig frame + punkt → LinkActivated | Ingen navigasjon fra stale frame. | Planned |
| 2 | `LinkActivated / CommandRouter back/forward` | `NavigationCoordinator::followLink / goBack / goForward` | `src/application/navigation/NavigationCoordinator.cpp` | Mål/retning → pending request | FUNC-008 eier resolve og history. | Planned |
| 3 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Request → OpenResult | Samme dirty/I/O-vei som all åpning. | Planned |
| 4 | `DocumentOpened` | `NavigationCoordinator::commitVisit` | `src/application/navigation/NavigationCoordinator.cpp` | Suksess + token → history commit | Ingen commit ved Failed/Cancelled. | Planned |
| 5 | `FrameReady` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Riktig frame + anker → posisjon | FUNC-009 clampler endret fil. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-002](../functionality/Functionality-002--Local-File-Storage.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-008](../functionality/Functionality-008--Navigation-History.md), [FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](../functionality/Functionality-010--Workspace-Controls.md)

Historikk og scrollsync deler kildeanker; sidebar/CLI bruker samme dokumentbytte. Ingen parser, filstore eller widgetkopi inne i feature.

## 7. Verifikasjon

AT-001, AT-005, AT-009, AT-012, AT-015, AT-018, AT-022, AT-023: A/B/C/back/D, tilbake ved dirty, slettet mål, URL-schemes, ett history-commit og restore etter resize. Planlagt `tests/acceptance/DocumentNavigationTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Første versjon har foreslått 100 poster, kun sesjon. Fragmenter og ekstern nettleser krever senere krav; ikke implementer skjult ekstra scope.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
