---
id: FUNC-008
kind: Functionality
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-005, UR-009, UR-048, SR-002, SR-005, SR-008, SR-013
uses: FUNC-001, FUNC-009
---

# Functionality-008: Document navigation and history

## 1. Purpose and scope

Give local document switching and Back/Forward one history policy. Application
coordinates link routing; renderer only supplies targets. NavigationCoordinator
owns document transactions, LinkResolver URI/path policy, HeadingAnchor heading
IDs and HistoryStore the bounded visit list.

## 2. Requirements and acceptance

UR-001, UR-005, UR-009, UR-048; SR-002, SR-005, SR-008, SR-013.
AT-001, AT-005, AT-009, AT-012, AT-015, AT-018, AT-023, AT-077.
See [requirements](../../../xfmd_requirements.md), including AT-077.

## 3. Contracts and ownership

NavigationCoordinator exposes openTarget, openAt, followLink, goBack/goForward.
Only the committed document callback commits history. Relative paths are resolved
against the source document's directory, never the process working directory.
LinkResolver::resourcePath remains a path-only interface. documentLink
splits a literal # before decoding each component, preserving encoded filename #.
HeadingAnchor::find maps an exact heading title or unique generated slug
to its original byte anchor; it performs no I/O and uses no renderer state.

## 4. Behavior, state and failures

Document load and unsaved-change resolution must succeed before history commit.
A new visit after Back truncates the forward branch; same-file navigation does not
duplicate visits. History retains up to 100 entries and restores source anchors.
Save As renames the current entry. Schemes/network paths, query syntax, malformed
escapes and control bytes remain rejected by local resolution.

Selected T003 extension: Application and NavigationPanel resolve heading links.
Exact displayed titles and lowercase ASCII slugs are supported; spaces become
hyphens, punctuation is removed, duplicate IDs receive -1, -2. Unicode bytes are
preserved; explicit HTML IDs and full Unicode case folding are not selected.
Empty fragments mean start. Missing headings report an error in the opened file.
Same-document jumps retain the live dirty buffer. Cross-file Cancel preserves
buffer, history and viewport. Pending main anchors apply only to the accepted
document token, and another open/edit invalidates them.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Application::open` | `NavigationCoordinator::openTarget` | `src/application/navigation/NavigationCoordinator.cpp` | Path to transaction | Reentrant request rejected | Implemented |
| 2 | `Application::followLink` | `LinkResolver::documentLink` | `src/application/navigation/LinkResolver.cpp` | URI to path/optional fragment | Invalid local URI rejected | Implemented |
| 3 | `Application::followLink` | `NavigationCoordinator::openAt` | `src/application/navigation/NavigationCoordinator.cpp` | Same-file anchor | Preserves live edits | Implemented |
| 4 | `NavigationCoordinator::goBack / goForward` | `HistoryStore::propose` | `src/application/navigation/HistoryStore.cpp` | Direction to visit | No-op at boundary | Implemented |
| 5 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path to committed document | Cancel/failure preserves history | Implemented |
| 6 | `DocumentCoordinator opened callback` | `NavigationCoordinator::commitVisit` | `src/application/navigation/NavigationCoordinator.cpp` | Document to visit | Exactly one commit | Implemented |
| 7 | `NavigationCoordinator::commitVisit` | `HistoryStore::commit` | `src/application/navigation/HistoryStore.cpp` | Previous anchor to history | Truncates forward branch | Implemented |
| 8 | `NavigationCoordinator::commitVisit` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Byte anchor to deferred viewport | Waits for current frame | Implemented |
| 9 | `Application::applyLinkAnchor / NavigationPanel model callback` | `HeadingAnchor::find` | `src/application/navigation/HeadingAnchor.cpp` | Parsed headings to source anchor | Missing heading diagnostic | Implemented |

## 6. Reuse and dependencies

[FUNC-001](Functionality-001--Document-Session.md) owns document state;
[FUNC-009](Functionality-009--Source-Anchor-Mapping.md) maps anchors to viewports.
CLI/dialog/sidebar/links share the document transaction. FUNC-031 reuses navigation
per panel. T003 replaces its restricted fragment matcher with the same heading-ID
policy as the main panel. External HTTP(S) remains an argv-based browser adapter.

## 7. Verification

NavigationTest covers branching history, dirty cancellation, broken paths, bounds,
escapes/schemes and same-buffer openAt. Native tests exercise actual link hits.
Historical evidence: [P6](../../../docs/evidence/P6.md) and
[native links](../../../docs/evidence/document-links.md). T003 acceptance and
candidate evidence belong to PLAN-XFMD-0007 / MM-001; not yet verified here.

## 8. Status, risks and change impact

Navigation implemented in P6; source-anchor navigation added in P15. T003 heading
links implement the design selected before code. Index reference groups retain their
existing file/source-location semantics and are not an HTML anchor engine.
Model sources remain under the owner's hold. Update current textual contracts,
plumbing and evidence with this change; historical evidence keeps its dated scope.
