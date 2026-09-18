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
