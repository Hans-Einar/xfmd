# Fase 046 — typed tolkning, preview og statisk PDF

Branch: sprint/003/phase/046-boxui-preview. Autonom modus.

M1: Separate BoxUI-kontrakter og ABI, interpreterinjeksjon, child-tolkning,
arbeiderforberedelse, SVG-plassering og statisk PDF-tegning. Eksisterende diagram-
ABI er uendret. Privat JSON-kodek ligger under contracts/boxui/private; den er
wire-serialisering, ikke funksjonell eier. Offentlige kontrakter inneholder ikke JSON.

M2: Fasebygg, parser-/ABI-/previewtest og eksisterende diagramregresjoner.
Resultater føres inn etter kontroll; ingen full GUI-akseptanse i denne fasen.

Oppdaget parallelt fork-arbeid i mermaid-rs-renderer-boxui-implementation etter
oppstart. Vår kjerne er isolert på phase/boxui-045-core, ikke integrert i master.
Valg av endelig produsentgren avklares før endelig leveranse, uten å endre den
andre arbeidsmappens filer. Vertens BX-HOST-grense er lik på begge grenene.
