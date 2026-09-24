---
id: FUNC-014
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-032, UR-030, UR-031, UR-025, UR-027, UR-024, UR-015, UR-017, SR-002, SR-018
uses: none
---

# Functionality-014: Versioned application preferences

## 1. Purpose and scope

Own the preference schema, active profile, drafts, validation and persistence.
The Edit menu and FOX dialog consume this service; document text and undo remain
owned by FUNC-001/011.

## 2. Requirements and acceptance

UR-032, UR-030, UR-031, UR-025, UR-027, UR-024, UR-015, UR-017 and SR-002, SR-018; see the
[requirements](../../../xfmd_requirements.md). AT-012, AT-029, AT-031, AT-038 cover the original
service. P16 adds browserProgram and AT-044: absent keys default to xdg-open;
a value names one executable, without an argument string. Empty/control-byte values
are rejected; launch errors do not change the document.

## 3. Contracts and ownership

PreferencesService::begin/validate/commit/cancel uses PreferencesDraft and immutable
PreferencesSnapshot, including version, scroll, marginMm and browserProgram.
PreferencesDialog owns widgets and an isolated sample field. FoxPreferencesStore
loads/saves through XFMD's existing FXRegistry groups Scroll/Page/Programs while
preserving WorkPaths. FOX and file-format details stay in application.

Commit validates, saves, then publishes a new snapshot to live consumers. Callbacks
carry the preference revision; consumers reset old movement state. One service
exists per Application. Unknown future keys are preserved; an unsupported newer
schema is not overwritten.

## 4. Behavior, state and failures

OK commits; Cancel/window close discards the draft. The sample field uses the same
scroll engine with independent state and does not affect editor/preview. Reject
invalid, nonfinite and out-of-range values; missing files use defaults. A write
failure keeps the dialog open and old profile intact. P9's original plan required
checking FXRegistry write/atomicity behavior and staged publication if needed,
without introducing a competing preference system.

P053 browser routing: ordinary HTTP(S) and local HTML use the configured executable;
Ctrl uses xdg-open. Local paths are canonicalized and encoded as absolute file URLs.
ExternalBrowser passes a single target argv without a shell, limits outstanding
children to 16, and reports startup or asynchronous nonzero/signal failure through
Application. Handoff leaves document/root/history unchanged.

P054 implementation: the separate path/filter row, compact Files/Index and recent
layout, active-tab Refresh and theme popup follow the [phase specification](../../../sprints/Sprint-007--Workspace-UI/Phase-054--Workspace-Layout.md).
Older toolbar/header placement descriptions are historical; persistence and document
contracts remain. Native acceptance is recorded in the P054 evidence.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreferencesDialog constructor` | `PreferencesService::begin` | `src/application/preferences/PreferencesService.h` | Active profile → draft | No buffer change | Implemented |
| 2 | `PreferencesDialog OK` | `PreferencesService::commit` | `src/application/preferences/PreferencesService.cpp` | Validated values → snapshot | Failure retains draft | Implemented |
| 3 | `PreferencesService::commit` | `FoxPreferencesStore::save` | `src/application/adapters/FoxPreferencesStore.cpp` | Schema → stored profile | Publish only after success | Implemented |
| 4 | `Application::openBrowser` | `ExternalBrowser::open` | `src/application/adapters/ExternalBrowser.cpp` | URL/program → process | Report errors; no shell | Implemented |
| 5 | `PreferencesDialog appearance preview` | `UiContext::setAppearance` | `src/application/ui/style/UiContext.cpp` | Draft → live UI | Cancel/close/failure restores active appearance | Implemented |
| 6 | `PreferencesDialog reload style` | `UiContext::reload` | `src/application/ui/style/UiContext.cpp` | Validated appearance.ini → profile | Retain old profile on error | Implemented |
| 7 | `PreviewColorControls changed callback` | `Application::changeReadingColors` | `src/application/ApplicationAppearance.cpp` | Colors → repaint/profile | No document mutation | Implemented |
| 8 | `Application::applyAppearance` | `PreviewColorControls::sync` | `src/application/ui/controls/PreviewColorControls.cpp` | Reading colors → controls | No document mutation | Implemented |
| 9 | `Application::changeReadingColors` | `PreferencesService::commit` | `src/application/preferences/PreferencesService.cpp` | Release/key/reset → Light/Dark profile | Write failure restores palette/sliders | Implemented |
| 10 | `Application::openTarget` | `ExternalBrowser::openFile` | `src/application/adapters/ExternalBrowser.cpp` | Existing local HTML → encoded file URL | Reject nonregular target; argv launch | Implemented |
| 11 | `Application::pollBrowser` | `ExternalBrowser::poll` | `src/application/adapters/ExternalBrowser.cpp` | Child status → completion/error callback | Erase completed children before reporting failure | Implemented |
| 12 | ThemeButton alternate gesture | `ReadingColorPopup::showAt` | `src/application/ui/controls/ReadingColorPopup.cpp` | button anchor → palette controls | uses existing commit/rollback callbacks | Implemented |

## 6. Reuse and dependencies

Consumers: FTR-006, FUNC-015 and FUNC-010; paper defaults are shared with FTR-007.
WorkPathHistory retains its existing responsibility. No preferences are stored in
DocumentSession. FTR-003 consumes the browser choice via Application.

## 7. Verification

Original unit coverage: schema, bounds, migration, errors and cancel. Store coverage:
round-trip with unknown keys and WorkPaths retained. FOX coverage: Edit → Preferences,
sample field, OK/Cancel and restart. AT-012, AT-029, AT-031, AT-038: [P10](../../../docs/evidence/P10.md);
A4 integration continued in P11. P16's AT-044 uses PreferencesTest,
ExternalBrowserTest and BrowserPreferencesTest.

P053 adds local HTML, configured/OS distinction and asynchronous errors to native
routing checks. Evidence: [P053 opening evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md).

## 8. Status, risks and change impact

Revision 1.1 was recorded 2026-09-13; original plumbing was implemented in P10-M2.
P9 scoped the store and P11 built the paper profile on this service. See
[integration design](../../../softwareDesign.md) and [implementation plan](../../../implementationPlan.md).
P053 revises external opening without changing preference storage.

P17–P19 extended UR-025, UR-027 and AT-045, AT-046, AT-047 through application/ui/style and controls
while preserving documents, workflows and FOX input. P17 stored theme/compact/buttons/
fontSize additively in schema 1. ThemeProfiles validates bounded appearance.ini
overrides; invalid reload retains the old profile. PreferencesTest, ThemeProfilesTest
and UiControlsTest covered the foundation; toolbar/dialog integration followed.

P19: Appearance/Scrolling/Document/Programs share one draft. UiForm and DialogActions
standardize spacing and actions. UiContext previews appearance; OK persists,
Cancel/close/write failure restores the active profile. Errors preserve the draft
for retry. AppearancePreferencesTest covers preview/commit/cancel/close/write failure
and toolbar changes preserving other choices.
P17–P19 evidence: [tests/screenshots](../../../docs/evidence/P17-P19.md),
[UI classes](../../../docs/design/fox-ui-layer.md). Status remains Implemented;
physical user experience and other DPI were not automatically verified.

P21: UR-030, UR-031 and AT-050, AT-051 add reading preferences through existing service,
UiRow and UiContext. Only the FOX screen host supplies colors to DisplayListPainter;
PDF defaults remain unchanged. DecorationRole survives PageComposer without FOX
contract types. ReadingLight/ReadingDark store additive profiles. Live changes
repaint; release commits with rollback on write error. Evidence: [P21](../../../docs/evidence/P21.md).

P22: compact shared toolbar, UR-032, UR-033, UR-034; AT-052, CompactWorkspaceTest and existing
regressions. Evidence: [P22](../../../docs/evidence/P22.md).

P054 local acceptance: [workspace evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P054.md)
records native interaction/visual checks, focused ASan/UBSan checks and the final
source/binary manifest. Earlier phase placement descriptions retain their dated
scope. Status remains Implemented; this is not blanket physical-display verification.
