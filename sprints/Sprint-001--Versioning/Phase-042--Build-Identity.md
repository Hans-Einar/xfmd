# Fase 042 — Byggidentitet

Sprint: 001. Branch: `sprint/001/phase/042-build-identity`.
Byggmodus: autonom. Status: implementert; fasebygg gjenstår.

## Mål og akseptanse

UR-042 / SR-025, FUNC-026: samme versjon i CLI og vindustittel, numerisk
commitnummer, branch/PR-identitet, synlig dirty/unknown og full historikk i CI.
Prosjektreglene beskriver én branch per fase og én PR per sprint.

## Milestones

| ID | Leveranse | Commit | Kontroll |
| --- | --- | --- | --- |
| M01 | Arbeidsmåte, versjonskontrakt og plan | `a4f55ba` | Python-plumbingsti ble avvist; validator utvidet i M02 |
| M02 | Generator, CMake, CLI/vindu og tester | Denne implementasjonscommiten | 7 Git-tester, CMake-konfigurering og dokumentkontroll bestått |
| M03 | Fasebygg og verifikasjon | Planlagt | Bygg én gang etter M02; relevante CTests |

## Fasebygg

Ikke utført. Bygget commit og versjon dokumenteres i M03 etter kontroll.
Dokumentasjonscommit M03 utløser ikke et nytt lokalt programbygg.

## Overlevering

Én samlet sprint-PR til main. Ingen installasjon eller merge inngår automatisk.
