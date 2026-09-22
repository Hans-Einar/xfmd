---
id: FUNC-031
kind: Functionality
audience: Integration
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-044, SR-027
uses: FUNC-001, FUNC-005, FUNC-007, FUNC-008
---

# Functionality-031: Generert dokumentnavigasjon

## 1. Hensikt og avgrensning

Vis navigator og hoveddokument med eksisterende Markdown-pipeline. SDL eier
språk, projeksjon og SVG. XFMD eier vindu/panel og lokale lenker.

## 2. Krav og akseptanse

UR-044, SR-027 og AT-070, AT-071 i [kravene](../../../xfmd_requirements.md).
Numrene mellom baseline og disse nye ID-ene er reservert av parallelt BoxUI-arbeid.

## 3. Kontrakter og eierskap

NavigationPanel har egen session, preview, scrolling og historikk. DocumentViews
registrerer SDL-program, kilde og prosjekt ved oppstart. WindowEndpoint eier en
privat Unix SOCK_SEQPACKET-adresse, én per vindu. OPEN inneholder versjon,
vindu, klient, monoton sekvens, panel og lokal dokumentsti. INFO er lesestatus.

## 4. Atferd, tilstand og feil

Klikk fanger denne vertens identitet før asynkront verktøykall. Nytt klikk
kansellerer gammelt; feil beholder dokumentet. Ingen fokusoppslag ved levering.
Vindulukking stopper eide jobber og fjerner endpoint. Genererte pakker beholdes
hele vinduets levetid; lease-broker integreres separat. Maks én jobb per vindu.
Bare registrert prosjekt og program tillates; exec argv uten shell.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Application` | `DocumentViews::follow` | `src/application/navigation/DocumentViews.cpp` | URI → eksplisitt jobb | feil gir status | Implemented |
| 2 | `DocumentViews` | `NavigationPanel::open` | `src/application/navigation/NavigationPanel.cpp` | fil → navigator | sist gyldige beholdes ved lesefeil | Implemented |
| 3 | `DocumentViews` | `WindowEndpoint::poll` | `src/application/navigation/WindowEndpoint.cpp` | XFMD1 → vindu/panel | gammel/ukjent avvises | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](Functionality-001--Document-Session.md),
[FUNC-005](Functionality-005--FOX-Presentation-Host.md),
[FUNC-007](Functionality-007--Preview-Pipeline.md),
[FUNC-008](Functionality-008--Navigation-History.md).
Ingen ny Markdown-parser, SDL-parser eller SVG-renderer i denne tjenesten.

## 7. Verifikasjon

Planlagt: CTest native lenkeklikk og panelbevaring, endpoint og prosessprøver.
Faktiske resultater føres i Sprint 004 før leveranse.

## 8. Status, risiko og endringskonsekvenser

Implemented 2026-09-22. Linux lokal IPC er første profil. Vanlig XFMD-oppstart
har uendret dokumentflyt; navigasjonsmodus velges eksplisitt med CLI.

Fase 050: optional broker-socket registreres ved oppstart. SDL-verktøyets
leseradapter leverer lease og brokeradresse sammen med OPEN. XFMD frigjør
forrige lease først etter vellykket dokumentbytte, og aktiv lease ved lukking.
Feilet release beholder ressursene i tjenesten; ingen tidsbasert sletting.
