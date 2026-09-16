# Mermaid-avhengigheter

Rust 1.92.0; konkret fork-commit og arkiv-SHA256 står i `cmake/mermaid-source.json`.
Libavoid `840ebcff20dbba36ad03a2160edf7cbaf9859984` er LGPL-2.1-or-later.
Se `docs/design/mermaid-libavoid-build.md` for statisk relinking og distribusjon.

Bare bibliotekprofilen brukes; CLI, PNG, browser og JavaScript inngår ikke.
Cargo.lock låser transitive versjoner og kildekontrollsummer. Tabellen nedenfor
er hentet fra Cargo metadata for denne låsen. Lokale XFMD-crates er prosjektkode.

| Crate | Versjon | Lisens |
| --- | --- | --- |
| aho-corasick | 1.1.5 | Unlicense OR MIT |
| anyhow | 1.0.104 | MIT OR Apache-2.0 |
| cc | 1.2.54 | MIT OR Apache-2.0 |
| find-msvc-tools | 0.1.11 | MIT OR Apache-2.0 |
| shlex | 1.3.0 | MIT OR Apache-2.0 |
| core_maths | 0.1.1 | MIT |
| fontconfig-parser | 0.5.8 | MIT |
| fontdb | 0.23.0 | MIT |
| itoa | 1.0.18 | MIT OR Apache-2.0 |
| json5 | 1.3.1 | MIT |
| libc | 0.2.189 | MIT OR Apache-2.0 |
| libm | 0.2.16 | MIT |
| log | 0.4.34 | MIT OR Apache-2.0 |
| memchr | 2.8.3 | Unlicense OR MIT |
| memmap2 | 0.9.11 | MIT OR Apache-2.0 |
| mermaid-rs-renderer | 0.3.1 | MIT AND LGPL-2.1-or-later |
| once_cell | 1.21.4 | MIT OR Apache-2.0 |
| proc-macro2 | 1.0.107 | MIT OR Apache-2.0 |
| quote | 1.0.47 | MIT OR Apache-2.0 |
| regex | 1.13.1 | MIT OR Apache-2.0 |
| regex-automata | 0.4.18 | MIT OR Apache-2.0 |
| regex-syntax | 0.8.11 | MIT OR Apache-2.0 |
| roxmltree | 0.20.0 | MIT OR Apache-2.0 |
| serde | 1.0.229 | MIT OR Apache-2.0 |
| serde_core | 1.0.229 | MIT OR Apache-2.0 |
| serde_derive | 1.0.229 | MIT OR Apache-2.0 |
| serde_json | 1.0.151 | MIT OR Apache-2.0 |
| slotmap | 1.1.1 | Zlib |
| syn | 3.0.5 | MIT OR Apache-2.0 |
| thiserror | 2.0.20 | MIT OR Apache-2.0 |
| thiserror-impl | 2.0.20 | MIT OR Apache-2.0 |
| tinyvec | 1.13.3 | Zlib OR Apache-2.0 OR MIT |
| ttf-parser | 0.25.1 | MIT OR Apache-2.0 |
| ucd-trie | 0.1.7 | MIT OR Apache-2.0 |
| unicode-ident | 1.0.24 | (MIT OR Apache-2.0) AND Unicode-3.0 |
| version_check | 0.9.5 | MIT/Apache-2.0 |
| zmij | 1.0.23 | MIT |
