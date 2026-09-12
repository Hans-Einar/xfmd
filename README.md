# xfmd — X File Markdown Viewer/Editor

Native Markdown-viser og editor for Linux, bygget på FOX som companion til `xfw`
og `xfi`. Første leveranse er implementert: typografisk visning, redigering med
undo/redo, live preview, lokal lenkehistorikk og synkronisert scrolling.

## Bygg og kjør

Krever C++17-kompilator, CMake ≥3.20, Ninja, pkg-config, FOX ≥1.6.57 (1.6 API),
libcurl-verktøyet `curl` og X11. Installer DejaVu Sans/Mono og gjerne Droid Sans
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

Markdown og `.txt` åpnes lokalt; `.txt` vises uformatert. Preview oppdateres etter
300 ms redigeringspause. Filgrensen er 8 MiB. UTF-8/BOM og eksisterende linjeslutt
bevares. Ekstern filendring gir konflikt; hardlenker krever Lagre som.

Lenker støtter lokale dokumentstier, inklusive relative stier og prosentkoding.
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
