# Release Notes

## [Unreleased]

Release-Date: unreleased

### Added

- KB-XFMD-023 / UR-047: open raw `.mmd` files directly as Mermaid, retaining
  source editing/saving and the shared native/PDF rendering pipeline.
- KB-XFMD-024 / UR-048: follow absolute, relative and same-file Markdown heading
  links, with title/slug resolution and the existing dirty/history transaction.

### Changed

- KB-XFMD-028 / PLAN-XFMD-0009 N2: show each SDL viewpoint once per source,
  flatten healthy phase/abstraction display wrappers, preserve reference-specific
  actions, and omit inert cycle/depth markers while retaining actionable links and
  genuine diagnostics. Aggregate viewpoint projections and source-span links are
  still pending producer/native work; no support claim follows from tree cleanup.

- KB-XFMD-025 / PLAN-XFMD-0008: generated SDP views use private user runtime
  storage, allocated on demand. Parent viewpoints expand to individual diagrams;
  obsolete bundles are cleaned up without a cumulative 64-view admission stop.
  Producer and per-result limits remain pending; native runtime verification is
  passed for the 0.6 successor (N2-001).

These entries describe the owner-selected local 0.6 candidate, combining
PLAN-XFMD-0007, the implemented increment of PLAN-XFMD-0008 and
PLAN-XFMD-0009 N2.
No public release date, tag or acceptance is implied; previous sprint/install
history remains authoritative under docs/versioning.md.

## [0.9.0] - 2026-10-21

Release-Date: 2026-10-21

### Added

- KB-XFMD-025 / PLAN-XFMD-0009 N15: retain all discovered SDL sources and load
  each model graph when expanded; negotiate producer support and reject stale or
  mismatched expansions. Preserve current documents and source navigation.
- KB-XFMD-028 / N4–N13: focused use cases with explicit scenarios, exact composed
  source spans, and complete VP02–VP06 and VP08–VP11 overview projections through
  the accompanying development SDPTool. Existing VP01/VP07 workflow retained.
- Private matching SDPTool beside existing SDUI tools. Explicit XFMD_SDP_TOOL
  overrides it; gh sdp remains the fallback when no private producer is installed.

### Fixed

- KB-XFMD-020 / N14: narrow packet field labels use numbered full legends;
  measured endpoint positions preserve bit proportions and separate bit numbers.

### Scope

Owner-authorized local development build/install with branch/count/+dirty identity.
Candidate and checks: N15b-001. This is not a public tag or signed SDP release.
Existing gh-sdp signed default and project process receipt are unchanged. Remaining
per-model/resource guards, broad large-print acceptance and independent review
remain explicit follow-ups; no claim of all KB020/025/028 scope completed.

## [0.8.0] - 2026-10-10

Release-Date: 2026-10-10

### Added

- KB-XFMD-028 / PLAN-XFMD-0009 N3: explicit producer-declared overview activation,
  native usecase-beta diagrams with actors, boundaries and typed relationships.
  VP01/VP07 aggregate pages are available when the selected SDP producer supplies
  the corresponding overview targets and projections.

### Fixed

- KB-XFMD-020: preserve quoted mindmap labels, native use-case attachment and
  bounded Mermaid caption routing, including DocumentReferences and ChapterNavigation.
  All ten current XFMD VP07 feature details render in the verified candidate.
- KB-XFMD-020/028: keep height-limited diagrams within printable PDF bounds despite
  floating-point rounding; retain all declared nodes, relations and labels.

### Scope

Owner-authorized local development installation, retaining branch/count/+dirty
identity. Includes the previously installed0.7 workflow. Large-overview readability,
focused scenario projections, exact SDL source links and remaining Session0005
acceptance are open. No public publication, independent approval, producer/gh
upgrade or authored-model update is claimed. R08-001 records the installed bytes.

## [0.7.0] - 2026-10-07

Release-Date: 2026-10-07

### Added

- KB-XFMD-027 / PLAN-XFMD-0010: SDUI composition diagram, existing static text
  render and source/binding table in one on-demand page. Hidden/reused instances
  retain source identity; source links highlight exact revision-matched spans.
- SDUI View source for valid/invalid sources and frames, using normal edit handling.
- Explicit Fyne prototype launch with producer preflight, revision recheck, local
  prototype/unbound-callback labels, diagnostics and independent window lifetime.
- KB-XFMD-029 SDUI portion: persistent SDP/KanBan/SDUI/Sessions toolbar groups in
  SDP sidebar mode. Missing/inapplicable actions are disabled with reasons.
- Private SDUI-owned preview/check and Fyne binaries included in the local package.

### Scope

Owner-authorized local build/install, not a public tag or signed SDP release.
No SDL viewpoint redesign, native embedding or automatic SDL callback execution.
KanBan Add/Move remain disabled pending their separate mutation-service contract.
The existing gh sdp installation and authored model files are unchanged.
