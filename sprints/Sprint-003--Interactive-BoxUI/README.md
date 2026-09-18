# Sprint 003 — Interaktiv BoxUI

Implementasjon autorisert etter gjennomgang av BX-HOST/0.1-draft1.
UR-043 / SR-026 / FTR-011. Én samlet sprint-PR, én branch per fase.

| Fase | Leveranse |
| --- | --- |
| 044 | Designgrunnlag og felles kontrakt (fullført) |
| 045 | Forkens BoxUI-parser, modell, layout/SVG og kontrakttester |
| 046 | XFMD typed tolkning, Rust-bro, preview og statisk PDF |
| 047 | Native input, fokus/drafts, syntetisk session og command ledger |
| 048 | Mixed Markdown, GUI/PDF, regressjoner, dokumentasjon og review |

Autonom byggmodus: ett planlagt programbygg per fase; nødvendige feilrettinger
kontrolleres på nytt. Ingen automatisk merge/installasjon før brukerbestilling.

Alle faser er implementert. Endelig produsent er forkcommit 61a85b6; den foreløpige
045-kjernen ble erstattet, ikke lagt oppå den parallelle implementasjonen.
[Fase 048 og faktisk installasjon](Phase-048--Verification.md) har testresultater
og skjermbilder. Ingen fysisk IME- eller produksjonsdomeneakseptanse hevdes.

Samlet leveranse: [PR #37](https://github.com/Hans-Einar/xfmd/pull/37).
