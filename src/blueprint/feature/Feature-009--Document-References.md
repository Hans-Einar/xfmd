---
id: FTR-009
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-023, SR-020
uses: FUNC-020, FUNC-008, FUNC-009, FUNC-010
---

# Feature-009: Dokumentreferanser

## 1. Hensikt og avgrensning

Finn lenkede Markdown-filer, deres hovedkapitler og nettlenker.

## 2. Krav og akseptanse

UR-023, SR-020. Se [kravene](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

DocumentIndex er verdi-basert semantisk metadata. ReferenceWorker får IInterpreter
og LocalFileStore fra composition root, og eier ingen widgets eller aktiv økt.
IndexPanel bruker NavigationTree med eide action-verdier og byteankre.

## 4. Atferd, tilstand og feil

Heading-hierarki følger nærmeste grunnere nivå; ingen tomme mellomnivåer.
References grupperer Markdown og Hyperlinks; filbarn lastes lazy, ett nivå dypt.
Gamle revisjoner deaktiverer klikk og forkaster jobber; feil vises under filen.
Enkeltklikk/Enter navigerer, piltaster velger. Dirty-cancel beholder aktiv fil.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator modelReady` | `DocumentIndex::build` | `src/application/index/DocumentIndex.cpp` | Semantikk → overskrifter/lenker | Ingen ny parsing | Implemented |
| 2 | `IndexPanel referenceRequested` | `ReferenceWorker::submit` | `src/application/index/ReferenceWorker.cpp` | Sti → lazy lesing | Køgrense og feilresultat | Implemented |
| 3 | `IndexPanel activated` | `NavigationCoordinator::openAt` | `src/application/navigation/NavigationCoordinator.cpp` | Sti/byte → navigasjon | Dirty-cancel før commit | Implemented |

## 6. Gjenbruk og avhengigheter

FUNC-003 leverer parserporten, FUNC-008/009 navigasjon og source mapping,
FUNC-010 arbeidsflate. FTR-008/009 er konsumentene; ingen feature-internkall.

## 7. Verifikasjon

AT-040, AT-041, AT-042, AT-043: DocumentIndexTest, ReferenceWorkerTest og IndexGuiTest.
Test semantiske nivåer, lenker i tabeller, lazy feil/stale og native enkeltklikk.

## 8. Status, risiko og endringskonsekvenser

Implemented P15, 2026-09-13. M1-modelltester og M2-GUI-tester er gjennomført; samlet evidens samles i M3. Referanser er øyeblikksbilder;
utvidelse leser filen igjen. Blokkerende filsystemkall kan ikke avbrytes midt i kall.

Evidence: [P15 — utførte tester, review og skjermbilde](../../../docs/evidence/P15.md).
