---
id: FUNC-014
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-032, UR-030, UR-031, UR-025, UR-027, UR-024, UR-015, UR-017, SR-002, SR-018
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
| 5 | `PreferencesDialog appearance preview` | `UiContext::setAppearance` | `src/application/ui/style/UiContext.cpp` | draft → levende UI | Cancel/close/lagringsfeil gjenoppretter aktivt utseende | Implemented |
| 6 | `PreferencesDialog reload style` | `UiContext::reload` | `src/application/ui/style/UiContext.cpp` | validert appearance.ini | gammel profil beholdes ved feil | Implemented |
| 4 | `Application::openBrowser` | `ExternalBrowser::open` | `src/application/adapters/ExternalBrowser.cpp` | Aktiv browserProgram + URL → prosess | Feil vises; ingen shell | Implemented |
| 20 | `PreviewColorControls changed callback` | `Application::changeReadingColors` | `src/application/ApplicationAppearance.cpp` | Lesefarger → repaint/profil | Ingen dokumentmutasjon | Implemented |
| 21 | `Application::applyAppearance` | `PreviewColorControls::sync` | `src/application/ui/controls/PreviewColorControls.cpp` | Lesefarger → repaint/profil | Ingen dokumentmutasjon | Implemented |
| 22 | `Application::changeReadingColors` | `PreferencesService::commit` | `src/application/preferences/PreferencesService.cpp` | Release/tastatur/reset → aktiv Light- eller Dark-profil | Skrivefeil gjenoppretter lagret palett og sliders | Implemented |

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

P19: Preferences har Appearance, Scrolling, Document og Programs med ett draft.
UiForm og DialogActions standardiserer spacing og OK/Cancel. Appearance forhåndsvises
gjennom UiContext; OK lagrer, mens Cancel, vinduskryss og lagringsfeil gjenoppretter
aktiv Appearance. Feil beholder utkastet for nytt forsøk. AppearancePreferencesTest
dekker preview, commit, cancel, close, lagringsfeil og toolbar som bevarer øvrige valg.


P17–P19 er implementert og kontrollert mot AT-045–047. Se
[testbevis og produksjonsskjermbilder](../../../docs/evidence/P17-P19.md) og
[konkrete UI-klasser](../../../docs/design/fox-ui-layer.md). Blueprint-status
beholdes som Implemented; fysisk brukeropplevelse/andre DPI er ikke automatisert verifisert.

P21 utvider samme eier med UR-030/031, AT-050, AT-051. ReadingColors er rene
lesepreferanser; PreviewColorControls bruker UiRow/UiContext. Bare FOX-host gir
DisplayListPainter en skjermpalett; PDF beholder standardfargene. DecorationRole
bevarer semantisk rolle gjennom PageComposer, uten FOX-typer i renderer/kontrakter.
Profiler lagres additivt i ReadingLight/ReadingDark via eksisterende preferences-service.
Live endring er repaint; commit ved release, med rollback ved skrivefeil.

P21: [AT-050/051, regresjoner og skjermbilder](../../../docs/evidence/P21.md).

P22: felles kompakt topplinje; UR-032/033/034 beskriver endret scope.

P22 verifikasjon: AT-052, CompactWorkspaceTest og eksisterende regresjoner.

P22: [testbevis og visuell kontroll](../../../docs/evidence/P22.md).
