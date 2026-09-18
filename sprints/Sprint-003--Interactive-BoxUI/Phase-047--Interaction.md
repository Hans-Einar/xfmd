# Fase 047 — native kontroller og lokal prototype

Branch sprint/003/phase/047-boxui-interaction. Autonomt fasebygg.

M1: Tilpass BX-HOST-adapteren til den parallelle produsenten 61a85b6.
Reelle tekstmål inkluderer baseline. Native input, fokus, clipboard, draft og
knapper følger kontrollkartet gjennom dokument-/viewport-transformen.
BoxUiSession sjekker identitet/revisjon/kontekst/type før lokal dispatch.
Synkron deltaker, dedupliseringsledger og eksplisitt restart; ingen automatisk start.
PDF får separat frosset snapshot med aksepterte verdier.

M2: Fasebygg og målrettede core/session/GUI-tester. Resultater føres etter kjøring.
Produksjonsdomain, transport, AT-SPI og fysisk IME-testing er ikke levert her.

Fasekontroll: Release-bygget bestod. Core og session bestod; første GUI-forsøk
avdekket at hostens 480 px minimumsviewport var for lav for reelle tekstmål og
simuleringsfooter. Minimumshøyden er nå 640 px. GUI-testen består etter retting:
aktivering, Unicode-utkast, resize/theme, Escape, suspend/resume, kontekstkonflikt,
A4/zoom/scroll, PDF og kildeendring. PDF-tekstekstraksjon viser akseptert C2 og
simulated snapshot, uten UNSUBMITTED-DRAFT. Skjermbilder ble faktisk inspisert.
Eksemplets standard grow-verdier ga unødvendig høye knapper; layoutvalg i
forfattereksemplet og utvidede kontroller avsluttes i fase 048.
