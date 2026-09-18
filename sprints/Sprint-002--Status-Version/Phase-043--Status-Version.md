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

Planlagt: fasebygg, CompactWorkspaceTest, BuildVersionTest og blueprint-kontroller.
