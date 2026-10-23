# R08-001 — XFMD 0.8 local development installation

Owner SESSION-XFMD-0005 T011, 2026-10-10. PLAN-XFMD-0009 / KB020/028.
Prior installed version: 0.7. Selected product version: 0.8; CPack: 0.8.0.

## Scope and identity

Install the existing development candidate through N3g, with only VERSION changed
among its captured source inputs. Native use-case diagrams, explicit producer
overview activation, mindmap/caption-routing and PDF-height corrections are included.
VP01/VP07 aggregate content requires compatible producer targets/projections;
XFMD installation does not install a producer. Existing gh sdp reports SDPTool 2.1.0
(producer-version.log) and was not upgraded. SDUI helpers are byte-identical to the
verified 0.7 package (helper-check.log).

[candidate.json](candidate.json), [source.tar.gz](source.tar.gz) and
[package.json](package.json) identify the exact shared dirty source and artifacts.
The renderer remains pinned to 55eff5da93e99420e110a1dce2568423578dbd58; its static
FFI archive is byte-identical to N3g. No broad staging/commit or model edit.

## Verification

- [Build](build.log) and full [CTest](tests.log): 91/91 passed, including 44 GUI tests.
  [Inventory](test-inventory.json) identifies selected tests and external tool paths.
- [Package](package.log): 210 files. Extracted executable reports 0.8; application and
  both private SDUI helper hashes match the tested build.
- [Release notes](Release-0.8.0.md), [single check](release-log-check.log),
  [all-section check](release-log-all-check.log), and preserved
  [0.7 check](prior-release-log-check.log). These projections do not prove publication.
- [Installed Mermaid startup](installed-mermaid-smoke.log) and
  [installed SDUI/Fyne workflow](installed-sdui-smoke.log) pass in isolated Xvfb/HOME.
- Existing [N3g native/PDF evidence](../N3g-001/README.md) covers the unchanged
  renderer: all ten VP07 details, native screenshot, vector PDF and caption geometry.
- [Document checks](document-checks.log) and [release checklist](ReleaseChecklist.md).

## Installation

[Installer](install.py) verifies archive SHA256, replaces each file atomically,
backs up previous files and rolls back on failure. [Install log](install.log) and
[receipt](installation-receipt.json) record the actual version, timestamp and backup.
All 210 installed file hashes match the package inventory. No existing user process
was stopped; newly started XFMD uses 0.8. User configuration is not part of the package.

Large-overview readability, focused scenario projections, exact SDL source links
and remaining Session0005 acceptance stay open. Local installation is complete;
KB020/028 remain active. No public tag/release, clean-tree claim, signature, CI,
independent review, owner visual acceptance or SDP/project upgrade is claimed.
