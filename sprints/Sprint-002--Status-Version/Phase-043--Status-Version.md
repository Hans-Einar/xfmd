# Fase 043 — Høyrejustert byggidentitet

## Mål og plumbing

UR-042 / AT-066: XfmdWindow::buildUi bruker buildVersion() i et eget høyrejustert
FXLabel nederst. Eksisterende statusmeldinger og lenkehover beholder venstre felt.
Ingen ekstra rad. Vindusbredder 450, 640, 1100 og 1900 kontrolleres i GUI-testen.

## Milestones

- M01: krav, blueprint, statusfelt og GUI-kontroll.
- M02: GUI-test avdekket feil layoutflagg; bruk LAYOUT_RIGHT i FXHorizontalFrame.
- M03: registrer faktisk bygg og testresultat før sprint-merge/installasjon.

## Verifikasjon

Bygget `3bb28f5`: `0.1 sprint/002/phase/043-status-version:185`.
Release-bygg bestått; seks relevante CTests bestått på 7,00 sekunder.
CompactWorkspaceTest bekrefter høyrekant og separat meldingsfelt ved alle fire
vindusbredder, samt eksisterende lenkehover. Skjermbildet er visuelt kontrollert.
[Tester](evidence/tests.txt), [metadata](evidence/build.json), [skjermbilde](evidence/status-version.png).

Den tidligere full-suite-kjøringen på PR35 ble avbrutt under oppfølgingen;
den brukes ikke som fullstendig testbevis. Ingen ny full-suite-påstand.
M03 dokumenterer bevis uten nytt bygg av uendret kode.
