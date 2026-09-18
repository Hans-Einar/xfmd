# Bygg og distribusjon av Libavoid-integrasjonen

Gjeldende commit/SHA-256 står i [pin-manifestet](../../cmake/mermaid-source.json);
[støttematrisen](../../mermaid_coverage.md) beskriver profilene og integrasjonsstatusen.

## Bygg

XFMD bruker Rust 1.92.0, Cargo.lock, C++17, CMake/Ninja, FOX, Cairo/Pango og
librsvg ≥ 2.46. Mermaid-kilden hentes som SHA-256-kontrollert arkiv fra en
konkret commit i Hans-Einar/mermaid-rs-renderer. Ingen Node, nettleser eller
ekstern rutetjeneste inngår. Libavoid-kildene ligger i forkens `vendor/libavoid`,
låst til Adaptagrams `840ebcff20dbba36ad03a2160edf7cbaf9859984` med filmanifest.

```sh
python3 tools/bootstrap_mermaid.py
python3 .deps/mermaid-rs-renderer/tools/verify_libavoid.py
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j2
ctest --test-dir build --output-on-failure -j1
```

Etter første kilde-/Cargo-nedlasting kan samme bygg kjøres offline. Rust-build.rs
kompilerer de medfølgende C++-kildene og den lille C-ABI-wrapperen. Hvert kall eier
sin egen Router; ingen muterbar router deles mellom tråder. Exceptions fanges i
C++, og Rust-panic stopper ved eksisterende FFI-barriere.

## Lisens og relinking

Libavoid er LGPL-2.1-or-later, ikke MIT. Forkens øvrige kode er MIT.
`vendor/libavoid/LICENSE.LGPL` skal følge en binærdistribusjon sammen med
opphavsrettsmerknadene. Dette bygget lenker Libavoid statisk inn i Rust-arkivet.
En distributør må derfor også levere materialet som gjør det mulig å bygge om
og relinke med modifisert Libavoid; bare et ferdig XFMD-program og MIT-lisens er
ikke nok. Lever komplette tilhørende XFMD-/fork-kilder og byggeavhengigheter,
eller nødvendige objektfiler/arkiver og relink-oppskrift i samsvar med LGPL §6.
Ikke pålegg restriksjoner som forbyr modifisering eller reverse engineering for
feilsøking av slike modifikasjoner.

Rebuild fra tilsvarende kilde: behold `.xfmd-pin`, endre Libavoid under forkens
vendor-mappe i en separat checkout, bygg fork/crate på nytt og lenk XFMD på nytt.
For et XFMD-kildearkiv må den samme fork-kilden inkluderes under `.deps` med
pin-stempel; det er ikke nok å vise til en tilfeldig nyere upstream-versjon.
Slett Cargo-buildartefakter hvis bibliotekobjekter gjenbrukes fra et eldre bygg.
Det vanlige bootstrap-byggløpet endrer aldri eksisterende kilder automatisk.

Denne endringen publiserer kildebranches, ikke en ferdig binærdistribusjon.

## Arbeidsgrenser

Libavoid-strategien avviser mer enn 256 forbindelser eller 2048 kandidatporter
før native arbeid. Dette er strengere enn parserens modellgrense. Feilen vises
ved kilden og bytter ikke automatisk til Legacy. Native nudging kan overskride
den kooperative fristen mellom progresjonspunkter; en hard frist krever en annen
prosessmodell og er ikke implementert her.
