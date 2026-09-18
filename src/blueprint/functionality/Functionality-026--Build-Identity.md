---
id: FUNC-026
kind: Functionality
audience: Integration
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-042, SR-025
uses: none
---

# Functionality-026: Byggidentitet

## 1. Hensikt og avgrensning

Gi CLI, vindustittel og installasjon én stabil identitet for faktisk bygget kode.
Byggverktøy eier Git-lesing; applikasjonen bruker kun innbakte verdier. Ingen ny feature.

## 2. Krav og akseptanse

UR-042, SR-025; AT-066/067 i [kravene](../../../xfmd_requirements.md).
[Versjonsreglene](../../../docs/versioning.md) definerer telling og særtilfeller.

## 3. Kontrakter og eierskap

`VERSION` eier major/minor. `tools/build_identity.py` lager header og JSON i
build/generated. `application/build/BuildVersion` eksponerer bare `buildVersion()`.
Ingen Git, Python, nettverk eller mutable teller trengs ved programkjøring.

## 4. Atferd, tilstand og feil

Full Git-telling, main-PR fra first-parent merge-tekst, branch ellers. Dirty er
synlig; shallow historie feiler; kildearkiv gir source:unknown. Samme innhold
skrives ikke på nytt. JSON har SHA for audit, mens brukeren ser et heltall.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | CMake build target | `identity` | `tools/build_identity.py` | VERSION + Git → identitet | shallow/ugyldig versjon feiler | Implemented |
| 2 | CLI main / Application::updateUi | `buildVersion` | `src/application/build/BuildVersion.cpp` | innbakt tekst → CLI/vindu | ingen runtime-I/O | Implemented |

## 6. Gjenbruk og avhengigheter

CLI og vindustittel bruker samme application-tjeneste. CPack/man-side bruker
major/minor fra VERSION. Interpreter og renderer får ingen ny avhengighet.

## 7. Verifikasjon

Sju isolerte Git-fixtures for AT-067 består. CLI-/vindustittelkontroll for AT-066
kjøres i fasebygget. Blueprint-validatoren støtter nå navngitt Python-plumbing
under tools/, i tillegg til C++/Rust under src/.
Fasebyggets faktiske resultat føres i sprintens fasefil.

## 8. Status, risiko og endringskonsekvenser

Implemented. Merge kan gi tallhopp; parallelle brancher kan dele tall. Historieomskriving
endrer identiteten og er forbudt for publiserte fasecommits. Ikke full global sekvens.
