---
id: FTR-011
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-043, SR-026
uses: FUNC-027, FUNC-028, FUNC-029, FUNC-030, FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-016, FUNC-018, FUNC-021, FUNC-025
---

# Feature-011: Interaktiv BoxUI i Markdown

## 1. Hensikt og avgrensning

Native XFMD-preview er første interaktive vert for BoxUI. Ingen HTML-leveranse.
Kilde, widgets og syntetisk domene kjøres via separate kontrakter; ingen automatisk
kodekjøring fra Markdown. Brukeren har godkjent implementasjonen.

## 2. Krav og akseptanse

UR-043 / SR-026, AT-068 / AT-069. Se [prosjektgrunnlaget](../../../docs/design/boxui-integration.md).

## 3. Kontrakter og eierskap

Forkens BX-HOST/0.1-draft1 er implementert mot den pinnede produsenten. Interpreter eier BoxUiModel; renderer
forbereder grafikk/geometri; application eier FOX, fokus, drafts, sessions og intent.
FUNC-027–030 beskriver de fire eierrollene. Eksisterende functionality gjenbrukes
uten at features kaller hverandres private implementasjon.

## 4. Atferd, tilstand og feil

Bare kompatible identiteter beholder input. Stale/feilet forberedelse overskriver
ikke aktiv ramme. Binding/kontekstskifte sperrer gamle Commands umiddelbart.
PDF bruker aksepterte verdier og kjører ingen input eller scenario.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | Cmark ModelBuilder | `BoxUiBlockBuilder::build` | `src/interpreter/boxui/BoxUiBlockBuilder.cpp` | fence → typed modell | lokal diagnostikk | Implemented |
| 2 | preview worker | `BoxUiLayout::prepare` | `src/renderer/boxui/BoxUiLayout.cpp` | modell/snapshot → frame | cancel/no-space | Implemented |
| 3 | GUI completion | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | aktuell frame → atomisk bytte | stale forkastes | Implemented |
| 4 | host input | `BoxUiSession::dispatch` | `src/application/boxui/BoxUiSession.cpp` | event → typed intent | gammel binding/busy avvises | Implemented |
| 5 | framebytte | `FoxBoxUiOverlay::reconcile` | `src/application/adapters/FoxBoxUiOverlay.cpp` | identitet → native felt/draft | inkompatibel state annulleres | Implemented |

## 6. Gjenbruk og avhengigheter

FUNC-003/004/005/007 gir parsing/layout/host/scheduling. FUNC-016/018 gir tekst/PDF;
FUNC-021 må fortsatt virke utenfor widgets; FUNC-025 gjenbrukes for embedded
Mermaid-forberedelse. Ingen endring av eksisterende Mermaid ABI som snarvei.

## 7. Verifikasjon

BoxUiCoreTest, BoxUiSessionTest, BoxUiGuiTest og BoxUiPdfTest dekker kontrakt,
syntetiske deltakere, native hendelser og frosset eksport. Faktisk kjøring og
begrensninger registreres i sprint 003 fase 048.

## 8. Status, risiko og endringskonsekvenser

Implemented/FirstRelease. G-PARALLEL-REVIEW er godkjent. Fysisk IME og AT-SPI
er ikke verifisert; native Unicode/clipboard er ikke bevis for disse egenskapene.
