# Fase 048 — verifikasjon og installasjon

Branch `sprint/003/phase/048-boxui-verification`. Autonomt fasebygg.

M1: Avslutt adaptervalidering, input-/ledger-grenser, klipping, native hendelsestester,
alle tre diagram-barn, frosset PDF og forfattereksempel. Oppdater krav, eierkart,
blueprints og faktisk profil. Produsenten er pinnet til 61a85b670dc1755b52c0fdf82c39497d0fe39512.

M2: Release-bygg, full CTest, visuell lys/mørk/PDF-kontroll og testbevis.
M3: Publiser fasebrancher og én sprint-PR; installer testet bygg etter brukerbestilling.
Main-merge er ikke del av denne bestillingen.

Fysisk IME, AT-SPI og integrasjon mot produksjonsdomene/transport inngår ikke i
beviset. Eksemplet er en eksplisitt aktivert lokal prototype. Kjøreresultater
føres etter test, ikke utledes fra kode eller bibliotekets egne tester.

## Utført fasekontroll

M1 er commit 92d0dcf (nummer 195). Release-bygg og alle 65 CTest-tester bestod.
Lys/mørk preview og PDF er visuelt kontrollert. Installert binær er identisk
med testet bygg; se [bevis og artefakter](evidence/README.md). M2 dokumenterer
dette uten nytt programbygg, slik arbeidsmåten tillater for dokumentcommits.

M3: Fasebranchene er publisert og sprint-PR #37 opprettet. Installert XFMD åpnet
boxui_evidence.md i isolert Xvfb uten stderr eller tidlig avslutning. Testregistreringen
gir den nye GUI-testen samme avgrensede FcInit-suppresjon som eksisterende native
tester i sanitizer-CI; applikasjonens egne lekkasjer forblir aktive feil.
