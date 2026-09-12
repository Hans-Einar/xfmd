---
id: FUNC-NNN
kind: Functionality
audience: <User|System|Integration>
role: <Workflow|Service|Adapter|Mechanism>
owner: <application|interpreter|renderer>
status: Proposed
scope: <FirstRelease|Future>
requirements: <UR-NNN, SR-NNN>
uses: <FUNC-NNN eller none>
---

# Functionality-NNN: <Navn>

## 1. Hensikt og avgrensning

Beskriv én tjeneste eller mekanisme, ansvar og hvorfor den trenger egen kontrakt. Oppgi eierlag; unngå et dokument per elementær metode.

## 2. Krav og akseptanse

Oppgi stabile UR-/SR-ID-er og lenk til kravspesifikasjonen. Beskriv hvilke observerbare
resultater objektet bidrar til; et use case er kontekst, ikke erstatning for krav.

## 3. Kontrakter og eierskap

Oppgi offentlige innganger/signaturer, input/resultater, invariants, sideeffekter,
levetid og tilstandseier. Features refererer functionality-kontrakter; de dupliserer
ikke API-er eller krever egen featureklasse. Oppgi enheter, revisjoner og koordinater.

## 4. Atferd, tilstand og feil

Beskriv normalforløp, pre-/postconditions, commit-punkt, cancel/rollback, feil,
stale data, gjentatte hendelser og relevante ressursgrenser. Skriv «ikke relevant»
med begrunnelse når en kategori ikke gjelder; ikke la uklarhet stå implisitt.

## 5. Plumbing

Vis den faktiske planlagte eller implementerte kallveien, inkludert event-/callback-
retning. Del i navngitte forløp dersom flere offentlige innganger trenger det.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `<Caller::method / Event>` | `<Owner::method>` | `src/<owner>/<File>.cpp` | <Input → output> | <Feil/sideeffekt> | Planned |

Kall til bibliotek må verifiseres mot valgt versjon. Marker `Implemented` først
når fil/symbol finnes og kallretningen er kontrollert.

## 6. Gjenbruk og avhengigheter

Lenk til konsumerte functionality-objekter. Oppgi hvem som bruker denne kontrakten,
hva som er offentlig, og hva som forblir internt. Begrunn nye tjenester og hvordan
duplikasjon fjernes. Feature-til-feature internkall er ikke tillatt.

## 7. Verifikasjon

Oppgi AT-ID-er, konkrete scenarioer/fixtures, forventet resultat og planlagte
unit-/kontrakt-/integrasjons-/GUI-tester. Skill testplan fra utført bevis.
Ved utført test: kommando, miljø, commit, utfall og lenke til rapport/test.
Ved Verified kreves en linje `Evidence: <lenke og identifisert testbevis>`.

## 8. Status, risiko og endringskonsekvenser

Oppgi revisjon/dato, åpne beslutninger, avhengige plansteg og konsekvenser for
konsumenter ved kontraktsendring. Status endres etter arbeidsmåten, ikke fordi
malen er fylt ut. Skriv hvilken eldre revisjon/bevis en endring erstatter.
