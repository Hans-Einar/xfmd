# Fase 042 — Byggidentitet

Sprint: 001. Branch: `sprint/001/phase/042-build-identity`.
Byggmodus: autonom. Status: fullført og kontrollert.

## Mål og akseptanse

UR-042 / SR-025, FUNC-026: samme versjon i CLI og vindustittel, numerisk
commitnummer, branch/PR-identitet, synlig dirty/unknown og full historikk i CI.
Prosjektreglene beskriver én branch per fase og én PR per sprint.

## Milestones

| ID | Leveranse | Commit | Kontroll |
| --- | --- | --- | --- |
| M01 | Arbeidsmåte, versjonskontrakt og plan | `a4f55ba` | Python-plumbingsti ble avvist; validator utvidet i M02 |
| M02 | Generator, CMake, CLI/vindu og tester | `620c7be` | 7 Git-tester, CMake-konfigurering og dokumentkontroll bestått |
| M03 | Fasebygg og verifikasjon | Denne verifikasjonscommiten | Ett fasebygg, 6/6 CTests, isolert installasjon og visuell kontroll |

## Fasebygg

Bygget commit: `620c7bea5b82cc3e23aabe684159a52478542183` (M02).
Synlig identitet: `0.1 sprint/001/phase/042-build-identity:181`.
Produktversjon: 0.1; SHA er kun auditreferanse.

Kommandoer:

```sh
cmake -S . -B build
cmake --build build -j2
ctest --test-dir build --output-on-failure -R 'BuildIdentityTest|BuildVersionTest|CompactWorkspaceTest|BlueprintStructure|BlueprintSymbols|LayerBoundaries'
```

Ett Release-fasebygg bestått. Seks CTests bestått på 6,70 sekunder; dette
inkluderer sju isolerte Git-scenarier og faktisk FOX-tittelkontroll.
[Logg](evidence/phase-042-tests.txt), [byggmetadata](evidence/phase-042-build.json),
[skjermbilde](evidence/phase-042-window.png). Ingen full-suite- eller ny sanitizer-
kjøring påstås for denne avgrensede endringen.

Isolert installasjon under `/tmp/xfmd-s001-p042-install` bekrefter samme
`--version`, installert metadata og man-side med produktversjon 0.1.
Binærens SHA-256: `040c8b720343b65a319de768059b4e0160c8cec825a1dd54ad40d2f1271c21f8`.

Dokumentasjonscommit M03 endrer ikke programkode og utløser ikke nytt lokalt
programbygg. Binæren beholder identiteten til M02 selv om Git får en nyere
commit for bevisene. Et senere eksplisitt bygg oppdaterer identiteten automatisk.

## Overlevering

[Sprint-PR #35](https://github.com/Hans-Einar/xfmd/pull/35) samler hele sprinten.
Brukerinstallasjonen er ikke erstattet; main-merge/installasjon er en egen bestilling.
