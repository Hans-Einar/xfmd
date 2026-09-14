# FOX UI-laget i produksjon

Implementert i P17–P19, 2026-09-14. Alt ligger i application; interpreter og
renderer får ingen stilavhengighet. Opprinnelig [UI-forslag](../../fox_ui_improvements.md)
og [UX-forslag](../../fox_ux_improvements.md) er beholdt som designhistorikk.

## Ressurser og avhengigheter

Application eier én UiContext og injiserer den i XfmdWindow, WorkspacePanel,
IndexPanel og PreferencesDialog. UiContext eier ThemeProfiles og IconCatalog;
FOX-appens normale fontobjekt gjenbrukes. Editorens monospace-font er separat.
Application destruerer vinduet før UI-ressursene.

ThemeProfiles inneholder de sentrale Light/Dark-palettene og Comfortable/Compact-
målene, med begrensede INI-overrides. PreferencesService eier de valgte verdiene,
validering og persistens. UiContext anvender dem på eksisterende widgets. Den
hopper over X-root-vinduet og endrer bare appens egne vinduer og ressurser.

| Produksjonsklasse | Filområde | Ansvar |
| --- | --- | --- |
| ThemeProfiles, UiContext | application/ui/style | Farger, mål, profilvalidering, ressursoppdatering og native restyling |
| IconCatalog | application/ui/style | Originale linjesymboler rasterisert til stabile FXIcon-objekter; enabled/disabled |
| UiButton | application/ui/controls | FOX-input, mål, ikon/tekst/fokus, eksplisitt checked-state |
| ButtonPainter | application/ui/controls | Utskiftbar Flat/Classic-tegning av rolle og knappetilstand |
| UiFactory | application/ui/controls | Injiser ressurser; valgfri builder for en annen UiButton-subklasse |
| UiRow, UiForm, PanelHeader, DialogActions | application/ui/controls | Delte mellomrom, formkolonner, overskrifter og OK/Cancel |
| PreviewControls | application/ui/controls | Lokal format/zoom-presentasjon av eksisterende CommandRouter-handlinger |

## Utvidelsespunkter

Endre standardfarger/mål i ThemeProfiles eller prøv dem i appearance.ini.
Bytt ButtonPainter når bare uttrykket skal endres. UiFactory::buttonClass kan
velge en alternativ UiButton-subklasse før vinduet bygges; UiControlsTest
verifiserer dette med en test-subklasse. Levende widgets skifter painter og
ressurser, ikke C++-klasse. Rolle er Toolbar, Normal eller Primary; Flat/Classic
er uavhengig av Light/Dark. Det opprinnelig foreslåtte outlined-uttrykket dekkes
av Normal/Primary-rollen i Flat-painteren.

Ikon, selector, native vindus-ID og fokus bevares ved temabytte. Ikoner genereres
uten fil-I/O i paint-handleren. CommandRouter er fortsatt eneste kilde for
handlinger og enabled/checked i produktets toolbar; ikke en separat UI-tilstand.
UiButton skiller checked fra FOXs engaged-state, slik at samme toggle fortsatt
kan klikkes eller aktiveres med Space.

PreferencesPages bygger fanene; PreferencesDialog eier ett draft og profilkopier.
Utseende-preview går til UiContext, ikke PreferencesService::commit. OK lagrer
alle faner samtidig. Cancel/close/feil gjenoppretter aktive verdier og den tidligere
levende profilkopien. Eksternt endret INI-fil skrives aldri om av dialogen.

## Bevisste grenser

FOX beholder menypopup, felter, tabbook, trær, scrollbars, splitter og input.
De får palett/font/mål gjennom en lokal adapter; native fil- og feildialoger er
fortsatt FOX-dialoger. Dette er et lite application-lag, ikke en generell
Window-baseklasse, et CSS-system eller et alternativt toolkit. Standarddialogenes
interne knapper erstattes ikke globalt. Ingen Xfe-kode, browser engine eller
runtime-plugin-loader er introdusert.

Smal toolbar skjuler først modusgruppen, deretter navigasjon, med tilgang via
meny og hurtigtaster. Det er en eksplisitt fallback; ikke en dynamisk overflowmeny.
Automatisk kapittelfølging og sammenleggbar historikk er fortsatt utsatt som
beskrevet i faseplanen. Produksjonsskjermbildene og testbevisene ligger i
[P17–P19](../evidence/P17-P19.md).
