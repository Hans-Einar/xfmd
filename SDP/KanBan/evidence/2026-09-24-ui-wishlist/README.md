# Owner UI wishlist: source context, 2026-09-24

The owner supplied these screenshots in the 2026-09-24 Europe/Oslo conversation
requesting backlog capture of seven XFMD UI/functionality wishes. These are copies
of the supplied images, not screenshots taken or acceptance tests run by this agent.
The source is that dated conversation; no chat URL is available.

- [Image 1: sidebar before changes](sidebar-before.png) shows Files/Index tabs,
  a `warloc` folder label, Refresh, a filename search field, `.md`/`.txt` toggles
  and the tree. It motivates KB-XFMD-008/009.
- [Image 2: toolbar before changes](toolbar-before.png) shows the document label,
  view/theme controls, Wrap/A4/Fit width and exposed BG/Text sliders. It motivates
  KB-XFMD-008/010/011. Its title identifies `xfmd 0.1 sprint/003/phase/048-boxui-verification:198`.
  That displayed build is not asserted to match this checkout or a current binary.

Source inspection for these cards used `/home/warloc/git/xfmd-sdl-navigation`,
branch `sprint/005/phase/051-tooling-boundaries`, HEAD
`47a245a49ad6ab839971812f4edf01b4ecfc6a9e`, with existing uncommitted documentation,
tooling and KanBan work. Findings describe files as read on 2026-09-24, not a
clean-build certification. During registration another agent switched the branch
to `sprint/006/phase/052-sdl-acceptance` at the same HEAD and selected KB-XFMD-005.
That selection and its ledger event are preserved. Another agent is working in this repository; this
capture changes only the conversation board and its evidence attachments.

The seven owner items are preserved across six coherent cards:

| Owner item | Card and retained scope |
| --- | --- |
| 1 | KB-XFMD-006: Open dialog accepts folders and documents |
| 2 | KB-XFMD-007: plain text, HTML/browser, OS-default Ctrl+click |
| 3 | KB-XFMD-008: full-width path/edit/copy/filter row |
| 4 | KB-XFMD-009: remove folder heading and relocate contextual Refresh |
| 5 | KB-XFMD-009: recent tabs, Up and Open placement; KB-XFMD-006: dialog root update |
| 6 | KB-XFMD-010: wheel/key zoom, percentages, fit modes and View submenu |
| 7 | KB-XFMD-011: toolbar grouping, theme position and color popup |

The owner also expressed interest in describing the UI in SDUI, while explicitly
prioritizing the XFMD changes. KB-XFMD-011 retains that optional later review;
KB-XFMD-003 remains separate and is not a dependency of this wishlist.

Use the [board index](../../README.md) to find the current card paths.
