# xfmd — X File Markdown Viewer/Editor

Native Markdown-viser og editor for Linux, bygget på FOX som companion til `xfw`
og `xfi`. Første leveranse er implementert: typografisk visning, redigering med
undo/redo, live preview, lokal lenkehistorikk og synkronisert scrolling.

## Bygg og kjør

Krever C++17-kompilator, CMake ≥3.20, Ninja, pkg-config, FOX ≥1.6.57 (1.6 API),
libcurl-verktøyet `curl`, X11, Cairo og PangoCairo/Fontconfig (inkludert utviklingsfiler).
PDF-verifikasjon bruker Poppler-verktøyene `pdfinfo` og `pdftotext`. Installer DejaVu Sans/Mono og gjerne Droid Sans
Fallback eller Noto Sans CJK. Tester krever Python 3 og Xvfb.

```sh
./tools/bootstrap_dependencies.sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
./build/xfmd tests/fixtures/markdown/basics.md
```

Bootstrap laster eksplisitt ned hashkontrollert cmark 0.31.1 til `.deps/`.
Configure/build laster ikke ned dependencies. Bruk `-DBUILD_TESTING=OFF` for bare
applikasjonen; `-DXFMD_SANITIZERS=ON` i separat Debug-bygg for ASan/UBSan.

```sh
cmake --install build --prefix "$HOME/.local"
```

Installasjon inkluderer binærfil, desktop-entry, ikon, man-side og tredjepartsnotis.
Prosjektlisens er fortsatt ikke valgt av eieren; ingen formell release er publisert.
[cmark-notisen](LICENSES/cmark.txt) gjelder den statisk lenkede parseren.

## Bruk

| Handling | Tast |
| --- | --- |
| Åpne / lagre / lagre som | Ctrl+O / Ctrl+S / Ctrl+Shift+S |
| Angre / gjør om / søk | Ctrl+Z / Ctrl+Y / Ctrl+F |
| Preview / editor / delt visning | Ctrl+1 / Ctrl+2 / Ctrl+3 |
| Sidepanel | F10 |
| Tilbake / frem | Alt+Venstre / Alt+Høyre |

Sidepanelet beholdes ved mappevalg og filåpning; skjul/vis det selv med F10.
Markdown og `.txt` åpnes lokalt; `.txt` vises uformatert. Preview oppdateres etter
300 ms redigeringspause. Filgrensen er 8 MiB. UTF-8/BOM og eksisterende linjeslutt
bevares. Ekstern filendring gir konflikt; hardlenker krever Lagre som.

Lenker støtter lokale dokumentstier, inklusive relative stier og prosentkoding.
Relative Markdown-lenker merkes med `#`, absolutte med `/#`, og nettlenker med
en jordklode. Relative stier regnes fra mappen til den åpne filen.
Nettverkslenker, fragment-/querylenker, skript og HTML-eksekvering støttes ikke.
Bilder vises som alternativtekst. xfw-IPC, bilder og Markdown-utvidelser er senere
scope. Preview har fontfallback; editorens glyphdekning avhenger av valgt systemfont.

## Utvikling og design

- [Krav og use cases](xfmd_requirements.md)
- [Arkitektur og filkart](softwareArchitecture.md)
- [Arbeidsmåte](docs/working-method.md) og [blueprints](src/blueprint/README.md)
- [Faser og milepæler](implementationPlan.md)
- [Oppdatert bidragsguide](CONTRIBUTING.md) og [opprinnelige agentregler](AGENTS.md)
- [Samlet verifikasjon](docs/evidence/P7.md)

AGENTS.md er bevart etter instruksen om ikke å overskrive en eksisterende fil.
Dens designfasestatus og planlagte byggkommandoer er historiske; denne README-en
og CONTRIBUTING.md beskriver implementasjonen. Arkitektur- og arbeidsreglene gjelder.

## Arbeidsområder og filfilter

- `xfmd` starter med utvidet hjemmekatalog (`~`) i sidetreet.
- `xfmd .` setter treets rot til gjeldende arbeidsmappe. `xfmd /sti/til/mappe`
  velger en annen rot; `xfmd fil.md` åpner filen med dens mappe som arbeidsrot.
- Dobbeltklikk rotnoden for å utvide til `~`, og igjen for å utvide til `/`.
  Vanlig mappeutvidelse holder seg innenfor valgt rot.
- Høyreklikk en mappe og velg **Set work path**. Tidligere arbeidsstier finnes
  under treet; ett klikk aktiverer en sti. De 32 nyeste unike stiene lagres mellom
  oppstarter. Start uten argument bruker fortsatt `~`.
- Filterknappene kombineres slik: `(*.md OR *.txt) AND navnefilter` når begge er
  på. Ingen aktiv knapp betyr alle filtyper. Knappene er av som standard.
  Tekst uten wildcard matches som delstreng; `?` matcher ett Unicode-tegn og `*`
  null eller flere tegn i hele filnavnet. `report?.*` matcher `report1.md` og
  `reportø.txt`, men ikke `report12.md`. ASCII-matching er case-insensitive.
- Aktivt filter søker i undermapper i bakgrunnen og viser bare filer med treff
  og mappene som leder til dem. **Refresh** leser treet på nytt etter filendringer.
  Status viser søk og uleselige mapper. Skjulte filer tas med; symbolske
  mappelenker traverseres ikke, og filsymlinker utenfor arbeidsroten utelates.

Arbeidsrot og historikk endrer ikke dokumentbuffer, prosessens PWD eller
sidepanelets synlighet. Dokumentlenker og Åpne-dialogen flytter ikke arbeidsroten.
Filer av andre typer kan vises i treet, men bare `.md`/`.txt` kan åpnes i editoren.

## Planlagt utvidelse

[Designrevisjon 1.1](softwareDesign.md) beskriver Preferences med scrollhastighet/
akselerasjon, A4-preview, PDF-eksport, fullscreen og [ikonforslag](docs/design/xfmd-icon-proposal.svg).
Dette er **Proposed**, ikke tilgjengelige funksjoner ennå. Se [P9–P13-planen](implementationPlan.md#5-planlagt-utvidelse-p9p13)
og oppdatert [blueprint-register](src/blueprint/README.md).
