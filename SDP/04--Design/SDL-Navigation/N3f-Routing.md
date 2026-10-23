# N3f — Measured caption space in the existing placement retry

Session0005 T009. Governing SR-024 / UR-039/041 / FUNC-024 and existing strict
attachment geometry remain unchanged. N3f-001 reproduces DocumentReferences.
With exact native metrics (including the existing 1/0.75 point-to-SVG conversion),
the fork test reproduces the same edge11 attachment intrusion. Approximate/smaller
measurements pass and are not evidence for this defect.

The current one expanded-placement retry uses constant minimum spacing. The
failing measured layout bunches parallel feature-contribution edges eight units
apart; edge14 crosses edge11's attachment zone. Evaluate a measurement-derived
lower bound for that existing retry: room for two opposed padded captions plus
clearance, along and across the graph direction. Preserve the existing floor,
three route transactions, four label orders, final validation, cancellation and
one fresh-placement retry. No alternate engine, extra unlimited retries, guard
relaxation or application-side geometry repair. Candidate adoption requires the
exact regression, original VP07, backend/attachment tests and native/PDF evidence.

## Verification disposition

Adopted after exact native/fork and PDF verification in N3f-002. The existing
ChapterNavigation ambiguous-caption fixture remains a separate open obligation.
The correction does not establish universal routing success or complete Session
acceptance; the guards still reject invalid geometry.
