# Discoverable SDPTool menu and personal toolbar

| Field | Value |
| --- | --- |
| id | KB-XFMD-030 |
| project | XFMD |
| type | Proposal |
| CardState | onHold |
| created | 2026-10-09T09:44:29.345109+00:00 |
| source | Owner SDP Session0008 T013, 2026-10-09 |
| next_review | On the qualifying SDPTool release; administrative checkpoint 2026-10-17 if no release is announced (not a delivery estimate) |
| Session | [SESSION-XFMD-0006](../../Sessions/session-%230006--Discoverable-SDPTool-actions.md) |

## Owner outcome

Automatically expose supported SDPTool actions in an SDPTool menu. Add a ToolBar
tab in Preferences to select/reorder toolbar actions. Store personal configuration
as stable action IDs, ordering and optional icon overrides so users can also edit
it directly. XFMD owns an icon library keyed by action ID/icon key and supplies a
generic icon when no specific icon exists. New supported actions should not require
hardcoded menu/toolbar changes merely to become available.

Hide actions not supported by the selected older SDPTool, retaining dormant config
entries so they return on upgrade. This differs from an advertised operation that
is temporarily unavailable in the current context: keep that disabled with a reason,
consistent with [KB029](../active/%23029--Change--Contextual-SDP-toolbar.md).
This proposal extends #029; it does not close its unfinished KanBan mutation scope.

## Inspected current integration

SdpToolJob.cpp launches gh sdp by default, or one executable from XFMD_SDP_TOOL.
SdpNavigation requests JSON; SdpProtocol decodes it; SdpPanel builds tabs from
navigation roots with kind tab. These provide the foundation for the upstream
Blueprints root, but not automatic action discovery/forms. No direct C++ linkage to
Go services or generic action catalogue was found in this path.

External producer work: `external:KB-SDP-051`. Blueprint source implementation:
`external:KB-SDP-050`, sdp/blueprint-implementation at d2cc760 before this handoff.
External document locator: SDP repository, SDPTool/XFMD-Blueprint-Integration.md.
Local convenience link (not a required checkout or authoritative external identity):
[Blueprint integration handoff](file:///tmp/sdp-blueprint-implementation/SDPTool/XFMD-Blueprint-Integration.md).
See its producer README, consumer contract and Assignment-Lifecycle links for exact
commands and trial fixtures. No installed/published blueprint capability is assumed.

## Design and acceptance

1. Consume the released PLAN-SDP-0024 JSON API and versioned catalogue with stable IDs, groups, labels, descriptions,
   icon keys, typed inputs/outputs and context/availability information. Use the
   same validated invocation path from menu and toolbar; do not scrape --help.
2. Bind project/model/blueprint/assignment context from the actual selection. Build
   generic forms for supported parameter types; explain unsupported interactions.
   Complex future workflows may require adapters, not just icons.
3. Include Blueprints generation/retention, refresh/list, assessment and supported
   assignment transitions. Display work state separately from readiness/freshness.
   Handle optimistic-revision errors and idempotent retries; do not edit bundle
   files or canonical history directly. Tool metadata is not user authorization.
4. Persist personal toolbar configuration outside project sources. Preserve unknown
   IDs across older-runtime use; validate malformed config; define duplicate/reset
   behavior. Menus/buttons share availability and keyboard behavior.
5. Invoke typed operations/argument vectors, never shell snippets supplied by a
   document. Preserve diagnostics/results, including blocked-assessment JSON.
6. Verify real navigation and actions through the released JSON API and signed
   gh-sdp distribution path after the producer release gate below. Test refresh/runtime downgrade,
   unsupported schema/input, generic icons and failure without losing selection.

## Worklog and boundary

2026-10-09: created and staged for the XFMD agent. Board index/history updated;
existing dirty application changes preserved. This card registers the owner request,
not completed UI work or an implementation plan. Related #014/#021 navigation and
#029 toolbar work should be reused rather than replaced.

## Queue — 2026-10-10

Owner selects this topic as the new Session focus after installing XFMD 0.8 and
explicitly pausing Session0005. Predecessor: Session0005 T012; no dependency on
finishing its diagram work. Prerequisites: recover the actual gh sdp catalogue and
producer KB-SDP-051/050 contracts, reconcile KB029 availability, then select a
bounded plan. Next: Session0006 S1. CardState backlog → queued; planning selection
only, not claimed producer support or product implementation.

## Owner dependency gate — Session0006 T002, 2026-10-10

Defer XFMD implementation until the producer API is ready and the intended
SDP-vNow integration has been merged to main and released. XFMD must use the
JSON API for action discovery and invocation; do not develop a competing contract,
scrape help text, or implement producer-owned operations directly in XFMD.

The owner reports that external Session0011 contains a contract proposal and
PLAN-SDP-0024 for the action catalogue, JSON calls and blueprint operations, with
commit a9edf28 pushed. This is reported upstream planning progress, not independent
evidence that the API is implemented, merged, released or installed.

Restart conditions:

1. The required PLAN-SDP-0024 catalogue, JSON invocation and blueprint-operation
   contract is implemented and verified by its producer.
2. The intended SDP-vNow integration is merged to main.
3. A new SDPTool release includes that contract and is available through gh sdp.
4. At resumption, identify the actual selected release/schema, inspect its JSON
   responses and compatibility behavior, then select XFMD's bounded consumer plan.

No target release number or release date is assumed. Review on the release handoff;
2026-10-17 is an administrative reminder only, not an automated task or deadline.
CardState queued → onHold. Session0006 is paused awaiting this dependency; the
separate diagram Session0005 stays paused. No external-repository edits, messages,
implementation, installation or automatic monitoring are selected by this update.
