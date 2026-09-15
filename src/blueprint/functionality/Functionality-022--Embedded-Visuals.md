---
id: FUNC-022
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-036, UR-037
uses: FUNC-008
---

# Functionality-022: Embedded-Visuals

## 1. Hensikt og avgrensning

Forberede lokale bilder og LaTeX-formler for delt preview/PDF-presentasjon.

## 2. Krav og akseptanse

UR-036, UR-037. Normativ akseptanse i [kravene](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

Interpreter produserer ressursreferanser, aldri I/O. Application forbereder immutable VisualResource med mål; renderer plasserer ressursen. Application-adapteren tegner gjennom samme DisplayListPainter i preview og PDF. MicroTeX eier formelsats; GdkPixbuf dekoder bilder.

## 4. Atferd, tilstand og feil

Kun lokale regulære filer, maksimalt 16 MiB komprimert og 16 millioner piksler. Feil faller tilbake til alttekst/formelkilde. Ingen nettverk, TeX-input eller makrodefinisjoner. Ressurser deles innen samme forberedelse og lever så lenge frame/model trenger dem.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ParserWorker prepare callback / ExportPipeline` | `EmbeddedVisuals::prepare` | `src/application/media/EmbeddedVisuals.cpp` | Semantikk + dokumentsti → immutable visuelle ressurser | Avvis ugyldig input | Implemented |
| 2 | `EmbeddedVisuals` | `MathTypesetter::render` | `src/application/media/MathTypesetter.cpp` | LaTeX → vektorressurs | Avvis ugyldig input | Implemented |
| 3 | `EmbeddedVisuals` | `ImageDecoder::load` | `src/application/media/ImageDecoder.cpp` | Lokal sti → begrenset bilde | Avvis ugyldig input | Implemented |

| 4 | `EmbeddedVisuals::prepare` | `LinkResolver::resourcePath` | `src/application/navigation/LinkResolver.cpp` | Dokumentsti + bildereferanse → lokal sti | Bevarer eksisterende strengere dokumentpolicy i localPath | Implemented |

## 6. Gjenbruk og avhengigheter

Konsument: Markdown-presentasjon. Rene kontrakter beholder laggrensene.

## 7. Verifikasjon

AT-056, AT-057. Planlagt: målrettede unit- og native FOX-tester samt full regresjon.

## 8. Status, risiko og endringskonsekvenser

Implemented i brukerbestilt P23. M3 samler testbevis og kjente grenser.

ImageDecoder bruker GifBudget for samlet frame-budsjett før GIF-dekoding.
MathTypesetter serialiserer bibliotekets globale tilstand; CairoVisual holder
ferdig vektor-/pikselressurs uten videre MicroTeX-kall i GUI/PDF-paint.

P23: [testbevis og visuell kontroll](../../../docs/evidence/P23.md).
