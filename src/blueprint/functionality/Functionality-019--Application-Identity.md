---
id: FUNC-019
kind: Functionality
audience: Integration
role: Adapter
owner: application
status: Proposed
scope: Future
requirements: UR-020, SR-013
uses: none
---

# Functionality-019: Applikasjonsikon og desktop-identitet

## 1. Hensikt og avgrensning

Eie den samme visuelle identiteten i installasjon, desktop-entry og FOX-vindu. Dette er en integrasjonsfunctionality, ikke en egen produktfeature eller rendererfunksjon.

## 2. Krav og akseptanse

UR-020, SR-013. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-023, AT-034.

## 3. Kontrakter og eierskap

SVG-master med dokumentark og et geometrisk Markdown-M/linjemotiv. Designutkast: [ikonforslag](../../../docs/design/xfmd-icon-proposal.svg). Avledede 16/24/32/48/64/128 px PNG og FOX-ressurs bruker samme motiv; 16 px kan ha forenklet detaljnivå. `IconResources::load` eies av application og holder ikonressurser levende til FOX-vinduene er destruert.

Packaging bruker fortsatt `Icon=xfmd` og eksisterende desktop-/MIME-identitet. Systemets Window Maker-dock får vanlig appikon; ingen global WMState-redigering eller særskilt dockapp-host legges til XFMD.

## 4. Atferd, tilstand og feil

Manglende ikon må ikke hindre oppstart. Bygget bruker eksplisitt, dokumentert konvertering med kjente inputfiler; ingen nettverkshenting eller runtime-generering. Endelig småstørrelsesvariant avgjøres ved visuell kontroll før installasjon.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Application::initialize` | `IconResources::load` | `src/application/ui/IconResources.cpp` | ressursstørrelse → FOX-ikon | fallback uten krasj | Planned |
| 2 | `XfmdWindow constructor` | `XfmdWindow::setApplicationIcons` | `src/application/ui/XfmdWindow.cpp` | eide store/små ikoner → vindu | GUI-ressurslevetid | Planned |

## 6. Gjenbruk og avhengigheter

Konsumenter: FUNC-010 og packaging. Deler ingen tegnekode med dokumentrenderer; dette er appidentitet, ikke dokumentinnhold.

## 7. Verifikasjon

Visuell kontroll på lys/mørk bakgrunn og alle størrelser; native FOX-window property og ren installasjon av desktop-entry kontrolleres. SVG-utkastet i denne dokumentendringen er et forslag, ikke installert ikon.

AT-023, AT-034: planlagt verifikasjon; ingen implementasjonsbevis for utvidelsen.

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.1, 2026-09-13. Alle nye kall i kapittel 5 er Planned.
P13. Eksisterende packaging/xfmd.svg beholdes frem til implementasjon/visuell godkjenning av varianten.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.
