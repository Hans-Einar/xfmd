# KB-SDUI-003 widget scope handoff — applied

**Current status:** applied during Session0004 T006 after write access was restored.
The target card still matched the prepared baseline. The patch and actual review
event EVT-KB-SDUI-000018 are now in SDP-vNow; card state remains backlog.
See handoff.json for application identity. The preparation account below is historical.

Owner XFMD Session0004 T005 requests the complete useful widget inventory and
explicitly permits XM-M2 in ordinary discovery. The updated card and exact patch
are ready, but SDP-vNow is outside this session's writable roots. No upstream
write, worklog claim, ledger event, lifecycle change or implementation occurred.

Files: KB-SDUI-003.updated.md.txt (verbatim proposed upstream card), KB-SDUI-003.patch (change),
handoff.json (original/updated hashes and target). This is a transfer artifact,
not a second authoritative card; the existing upstream path remains authoritative.

Apply from an authorized SDP-vNow workspace after comparing the live card with
the recorded baseline. Run `git apply --check` and then apply this patch, or merge
only its intended edits if concurrent changes exist. Do not overwrite new work.
Append an actual x-kanban:reviewed event for KB-SDUI-003 to the shared management
ledger, allocating the event ID and previousEventId from live history at application
time; leave backlog state/path unchanged. Add the actual event to the card worklog
and validate the board. Reference external Session0004 T005; do not attribute this
request to SDP's unrelated governance Session. No event ID is reserved here.

Patch application was validated in a temporary directory containing the exact
source card; resulting bytes match the prepared full card. Live upstream baseline
was checked unchanged after preparation. Local Session and KB027 are updated now
with the owner's XM-M2 decision. No product build is needed for this documentation.

Session0005 verification note: renamed the verbatim upstream card snapshot from
.updated.md to .updated.md.txt without changing its bytes. Relative links belong
to its original upstream location; this transfer payload is not a maintained
XFMD Markdown document. The patch, applied upstream card and content hashes are
unchanged. This corrects the local document validator's false-location failures.
