---
id: FUNC-014
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-025, UR-027, UR-024, UR-015, UR-017, SR-002, SR-018
uses: none
---

# Functionality-014: Versjonerte applikasjonspreferanser

## 1. Hensikt og avgrensning

Eie preferanseskjema, aktiv profil, utkast, validering og lagring. Edit-menyen og FOX-dialogen er konsumenter; dokumenttekst og undo tilhører fortsatt FUNC-001/011.

## 2. Krav og akseptanse

UR-015, UR-017, SR-002, SR-018. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-012, AT-029, AT-031, AT-038.

P16: UR-024 / AT-044 utvider samme tjeneste med `browserProgram`. Manglende
nøkkel får xdg-open. Én executable, ingen argumentstreng; tomme verdier og verdier med kontrolltegn avvises. Programfeil ved oppstart vises uten å endre dokumentet.

## 3. Kontrakter og eierskap

`PreferencesService::begin/validate/commit/cancel` bruker `PreferencesDraft` og immutable `PreferencesSnapshot{version, scroll, marginMm, browserProgram}`. `PreferencesDialog` eier widgets og et isolert prøvefelt. `FoxPreferencesStore::load/save` bruker xfmds eksisterende FXRegistry med egne grupper `Scroll`/`Page`/`Programs`; `WorkPaths` bevares. FOX-/filformatdetaljer forblir i application.

Commit validerer først, lagrer gjennom store og publiserer nytt snapshot til levende scrollkonsumenter etter suksess. Callback sender preferanserevisjon; hver konsument nullstiller gammel bevegelsestilstand. Det finnes én service per Application. Framtidige ukjente nøkler bevares; nyere uforstått schema skrives ikke over.

## 4. Atferd, tilstand og feil

OK er commit; Cancel eller vinduskryss forkaster draft. Prøvefeltet bruker samme scrollmotor med eget state og påvirker ikke editor/preview. Ugyldige, ikke-endelige og out-of-range tall avvises; manglende fil får standarder. Skrivefeil holder dialogen åpen og den gamle aktive profilen intakt. P9 verifiserer FXRegistrys skrivefeil-/atomisitetskontrakt; om den ikke holder, implementeres eksplisitt staged publisering i store-adapteren uten nytt konkurrerende preferences-system.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreferencesDialog constructor` | `PreferencesService::begin` | `src/application/preferences/PreferencesService.h` | aktiv profil → draft | ingen bufferendring | Implemented |
| 2 | `PreferencesDialog OK` | `PreferencesService::commit` | `src/application/preferences/PreferencesService.cpp` | validerte verdier → snapshot | feil beholder draft | Implemented |
| 3 | `PreferencesService::commit` | `FoxPreferencesStore::save` | `src/application/adapters/FoxPreferencesStore.cpp` | schema → vedvarende profil | publiser først etter suksess | Implemented |
| 4 | `Application::openBrowser` | `ExternalBrowser::open` | `src/application/adapters/ExternalBrowser.cpp` | Aktiv browserProgram + URL → prosess | Feil vises; ingen shell | Implemented |

## 6. Gjenbruk og avhengigheter

Konsumenter: FTR-006, FUNC-015 og FUNC-010; papirdefaults deles med FTR-007. Eksisterende WorkPathHistory beholdes med sitt ansvar; ingen setting legges i DocumentSession.

## 7. Verifikasjon

Unit: schema, grenser, schema-migrering, feil og cancel. Diskadaptertest: lagre/les med ukjente nøkler og bevart WorkPaths. FOX-test: Edit → Preferences, prøvefelt, OK/Cancel og restart.

AT-012, AT-029, AT-031, AT-038: se [P10-bevis](../../../docs/evidence/P10.md); A4-integrasjonen fullføres i P11.

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.1, 2026-09-13. Kallene i kapittel 5 er implementert i P10-M2.
P9 avklarer lagringsadapter; implementeres i P10. Papirprofil bygges på samme service i P11.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.

P16: AT-044 dekkes av PreferencesTest, ExternalBrowserTest og BrowserPreferencesTest.

P17–P19: UR-025, UR-027 utvider dette ansvaret; se [faseplan](../../../implementationPlan.md).
Nye UI-klasser er planlagt under application/ui/style og controls. Stilendring
beholder dokument, arbeidsflyter og FOX-input. AT-045, AT-046, AT-047 får egne testbevis.

P17: Appearance lagres additivt i skjema 1 med theme, compact, buttons og fontSize.
ThemeProfiles laster validerte, begrensede overrides fra appearance.ini; ugyldig
reload beholder gammel profil. PreferencesTest, ThemeProfilesTest og UiControlsTest
dekker første foundation; toolbar/Preferences-integrasjon følger i P18/P19.
