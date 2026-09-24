# SDUI widget ownership and XFMD boundary

| Field | Value |
| --- | --- |
| id | KB-XFMD-003 |
| project | XFMD |
| type | Question |
| created | 2026-09-23T23:44:59Z |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD audit, BoxUI/SDUI direction and KanBan notes |
| next_review | Before selecting the next work round; SDUI question at the next SDUI UI discussion |

## Need and source

The owner recalled on 2026-09-24 that Fyne would own the window while the UI was
built from our own widgets. Preserve this distinction before further work turns
an assumption into an architecture decision. SDUI owns this decision; this local
note records the discussion and its impact on XFMD. No foreign KanBan card has
been created and this is not an invented Ref to one.

## Confirmed and still open

Confirmed in SDP-vNow `321e193258966bea049ea02178192404f8daad99`: Fyne is the
standalone SDUI host. SDUI owns language, model, widget identities, layout and
runtime; XFMD serves Markdown/generated documentation independently.

However, the inspected implementation uses Fyne controls: view.go calls
widget.NewButton; input.go extends widget.Entry with SDUI draft/focus behavior;
sdui-fyne creates the Fyne application/window. Shared SDUI layout places those
controls and static SVG content. The target architecture explicitly permits reuse
of Fyne controls for text input, focus and keyboard handling.

file:///home/warloc/git/SDP-vNow/SDUI/go/host/fynehost/view.go

file:///home/warloc/git/SDP-vNow/SDUI/go/host/fynehost/input.go

file:///home/warloc/git/SDP-vNow/SDUI/go/cmd/sdui-fyne/main.go

file:///home/warloc/git/SDP-vNow/SDUI/docs/target-architecture.md

Open: does "our own widgets" mean SDUI-defined controls with Fyne-backed input,
or entirely custom drawing and event behavior with Fyne only supplying window and
canvas services? The former describes the inspected implementation. The latter
would be an additional SDUI decision; this investigation found no evidence that
it has already been delivered. Do not infer that a standalone host requires every
control to be reimplemented.

Neither interpretation requires XFMD to host interactive BoxUI or change toolkit.

## Next review and completion criteria

At the next SDUI UI discussion, agree the intended control/input boundary and
record it in SDUI's own board or architecture process. Link that result here;
complete this question when the answer and XFMD impact are explicit. Do not turn
this question into an XFMD widget implementation task.

## Related notes

[BoxUI disposition](../completed/%23002--Study--BoxUI-branch-disposition.md) records the superseded
FOX direction and candidate general-purpose fixes without deciding this question.

## Discussion review — 2026-09-24

Keep the unresolved custom-widget boundary separate: SDUI owns it, and answering it is not required to preserve XFMD audit findings or establish this board.
