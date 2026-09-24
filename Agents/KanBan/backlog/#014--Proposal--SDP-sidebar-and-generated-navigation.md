# Discover SDP projects and expose an SDP sidebar tab

| Field | Value |
| --- | --- |
| id | KB-XFMD-014 |
| project | XFMD |
| type | Proposal |
| created | 2026-09-24T20:52:36+00:00 |
| source | Owner conversation, 2026-09-24: project-aware SDP tab and version/capability marker |
| next_review | Joint discovery-contract review with the SDP-vNow owner/agent and KB-SDP-016, before selecting implementation |

## Owner direction

Add a third sidebar tab, SDP, alongside Files and Index. Show it when the current
project folder contains a valid SDP area. The owner has additional layout ideas;
this card captures the job now rather than settling that layout or starting code.

The proposed marker lives inside the project's SDP folder and declares version
and capabilities: SDL and SDUI language versions used, Agents/KanBan availability,
SDL navigation support, and configuration XFMD otherwise receives as CLI arguments.
The conversation uses both `SDP_Version.yaml` and `SDL_version.yaml`; exact spelling,
case and schema are unresolved. SDP owns the contract, XFMD consumes it.

Producer-side companion: **KB-SDP-016** (discovery/version/capability contract).

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/backlog/%23016--Proposal--SDP-discovery-and-viewer-capabilities.md

## Bounded investigation — existing behavior

Inspected XFMD at 9b93edb and SDP-vNow at afd9edb on 2026-09-24. This is source and
document review, not a new runtime test.

- [FUNC-031](../../../src/blueprint/functionality/Functionality-031--Generated-Document-Navigation.md)
  and [DocumentViews](../../../src/application/navigation/DocumentViews.cpp) already
  host independent main/navigation Markdown sessions. SDL owns parsing, projection
  and generated resources; XFMD owns display, window targeting and input.
- On an `sdl-view://<registered-project>/...` selection, direct mode runs the
  registered SDL tool with `view <source> --uri <uri> --output <temporary-directory>`
  and optional renderer. Delivery metadata selects the target pane and entry.md.
  A later selection cancels an older child; results stay tied to the originating
  window. No full static export is needed for each selected detail.
- The optional broker path uses addressed delivery and per-pane leases; this is
  not required for direct generation. Preserve current dirty-buffer admission,
  cancellation/stale-request rules and resource ownership when adding discovery.
- The existing navigator is a separate navigationArea, not a Files/Index tab.
  Moving/reusing it inside SDP requires an explicit layout/lifecycle decision;
  merely adding a tab label does not integrate the current navigator.
- KB-SDP-002 proposes `sdptool view ip`: open a main plan and navigator, generate
  selected details from current sources with prebuilt tools, without requiring a
  full export, startup compilation or daemon. Navigator snapshots need explicit
  refresh when model structure changes. This sdptool command remains proposed.

Current documented CLI, supplied by the owner and confirmed in main.cpp:

```text
xfmd [file|directory] [--navigator file.md --sdl-tool program --sdl-source model --project ID --renderer program --window-id ID]
xfmd --window ID --pane main|navigation [--client ID --request N] file.md
xfmd --window ID --info
```

main.cpp also accepts broker/lease options not shown in that short help. Discovery
must preserve these optional modes rather than assume the help is a complete wire
contract. Source: [main.cpp](../../../src/application/main.cpp).

## Metadata boundary to agree with SDP

| Existing input | Candidate discovery/configuration source, not an agreed schema |
| --- | --- |
| --project | Stable project identity, distinct from repository name and product release version |
| --sdl-source | Model entry path, resolved from a specified marker/SDP base |
| --navigator | Existing navigator path or a supported initial projection/viewpoint; define generation and refresh |
| --sdl-tool / --renderer | Required capability/version plus host-registered executable resolution; distinguish declarations from permission to execute |
| --window-id / --window | Runtime window identity/target, normally not a fixed project-manifest value |
| --pane / --client / --request | Runtime delivery/sequence metadata; preserve existing protocol |
| optional broker / leases | Host/session configuration and runtime resources; not persistent project facts |

## Sidebar sketch and open questions

Agent suggestion for discussion: a compact project/version header, an explicit
refresh action and the generated navigator as the main SDP-tab content. Candidate
links are overview, implementation plan, requirements/design views and KanBan only
where supported. These are proposed labels, not claims that corresponding SDL
viewpoints already exist. Keep the selected generated document in XFMD's main pane.
The owner will supply further layout direction before a blueprint is selected.

Agree what valid means: supported marker schema/profile, declared capabilities and
resolvable sources. Distinguish missing SDP from malformed/unsupported SDP or missing
tools. Specify whether the work root itself may be an SDP area, its SDP child,
and how nested projects or parent lookup work. Do not silently select a parent.
Define tab behavior and disposal of pending jobs/leases when work root changes;
keep ordinary Files/Index and dirty documents usable if SDP setup fails.

SDP already has a release/project manifest and an R3 proposal for SDP/project.json.
Resolve this overlap in KB-SDP-016 before adding another marker. Preserve CLI
usage and document override precedence. Declaring an SDUI language version does
not make XFMD an SDUI/BoxUI widget renderer; KB-XFMD-003 remains separate/deferred.
KB-XFMD-012 concerns KanBan format compatibility, not this discovery/UI capability.

## Next action and acceptance when selected

Review KB-SDP-016 together with SDP's KB-SDP-001/002/014 and the owner's layout
ideas. Agree a single discovery contract and initial supported viewpoints, then
write XFMD requirements/blueprint/plumbing before code. Candidate acceptance:
valid/missing/invalid/unsupported projects, relative paths from unrelated cwd,
optional KanBan/SDUI, source edits and navigator refresh, one on-demand view click,
correct window/pane, missing tool/generation errors, and dirty-buffer retention.

Outcome: backlog capture only. No tab, manifest, resolver or runtime behavior is
implemented or approved by this note; no other agent has been contacted.
