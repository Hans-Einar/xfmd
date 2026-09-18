---
id: FTR-011
kind: Feature
audience: User
role: Workflow
owner: application
status: Proposed
scope: Future
requirements: UR-043, SR-026
uses: FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-016, FUNC-018, FUNC-021, FUNC-025
---

# Feature-011: Interaktiv BoxUI i Markdown

## 1. Hensikt og avgrensning

Native XFMD-preview er første interaktive vert for BoxUI. Ingen HTML-leveranse.
Kilde, widgets og syntetisk domene kjøres via separate kontrakter; ingen automatisk
kodekjøring fra Markdown. Prosjektet avventer brukerens designgjennomgang.

## 2. Krav og akseptanse

UR-043 / SR-026, AT-068 / AT-069. Se [prosjektgrunnlaget](../../../docs/design/boxui-integration.md).

## 3. Kontrakter og eierskap

Forkens BX-HOST/0.1-draft1 er kandidat. Interpreter eier BoxUiModel; renderer
forbereder grafikk/geometri; application eier FOX, fokus, drafts, sessions og intent.
Fremtidige functionality-blueprints skal defineres før implementasjon; eksisterende
functionality gjenbrukes uten at features kaller hverandres private implementasjon.

## 4. Atferd, tilstand og feil

Bare kompatible identiteter beholder input. Stale/feilet forberedelse overskriver
ikke aktiv ramme. Binding/kontekstskifte sperrer gamle Commands umiddelbart.
PDF bruker aksepterte verdier og kjører ingen input eller scenario.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | Cmark ModelBuilder | `BoxUiBlockBuilder::build` | `src/interpreter/boxui/BoxUiBlockBuilder.cpp` | fence → typed modell | lokal diagnostikk | Planned |
| 2 | preview worker | `BoxUiLayout::prepare` | `src/renderer/boxui/BoxUiLayout.cpp` | modell/snapshot → frame | cancel/no-space | Planned |
| 3 | GUI completion | `BoxUiCoordinator::publish` | `src/application/boxui/BoxUiCoordinator.cpp` | aktuell frame → atomisk bytte | stale forkastes | Planned |
| 4 | host input | `BoxUiCoordinator::dispatch` | `src/application/boxui/BoxUiCoordinator.cpp` | event → typed intent | gammel binding/busy avvises | Planned |
| 5 | framebytte | `FoxBoxUiOverlay::reconcile` | `src/application/adapters/FoxBoxUiOverlay.cpp` | identitet → native felt/draft | inkompatibel state annulleres | Planned |

## 6. Gjenbruk og avhengigheter

FUNC-003/004/005/007 gir parsing/layout/host/scheduling. FUNC-016/018 gir tekst/PDF;
FUNC-021 må fortsatt virke utenfor widgets; FUNC-025 gjenbrukes for embedded
Mermaid-forberedelse. Ingen endring av eksisterende Mermaid ABI som snarvei.

## 7. Verifikasjon

Kun dokument- og blueprintkontroller i fase 044. AT-068/069 er planlagte;
implementasjon, GUI/PDF-akseptanse og C-ABI-livstid er ikke testet ennå.

## 8. Status, risiko og endringskonsekvenser

Proposed/Future. Stopper ved G-PARALLEL-REVIEW. Native IME, overlay-clipping og
endelig forbrukerprofil skal verifiseres, ikke utledes fra et gyldig JSON-skjema.
