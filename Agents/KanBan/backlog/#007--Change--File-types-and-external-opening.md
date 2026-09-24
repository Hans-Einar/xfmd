# Open plain text internally and route other targets consistently

| Field | Value |
| --- | --- |
| id | KB-XFMD-007 |
| project | XFMD |
| type | Change |
| created | 2026-09-24T18:01:41+02:00 |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD UI wishlist; Owner item 2 |
| next_review | Before selecting the next XFMD UI work round; resolve this card's named interaction questions during requirements/blueprint planning |

## Need and owner direction

Markdown links and the Files tree should support more file formats. Plain-text
files should open in the XFMD editor even when their suffix is not `.txt`.
Ctrl+click on a link or a file in the tree should open that target using the OS
default association. HTML files and web links should open in a browser by default.

| Target | Normal activation requested by owner | Ctrl+click requested by owner |
| --- | --- | --- |
| Markdown document | Keep normal XFMD document opening | OS default association |
| Other plain-text file | Open as text in XFMD editor | OS default association |
| Local HTML document | Browser, taking precedence over generic plain-text detection | OS default association |
| HTTP(S) web link | Browser | OS default URL handler |
| Other non-text file | Broader file support requested; recommend retaining existing OS handoff | OS default association |

The last row's normal-click rule is an agent recommendation, not an additional
owner decision. The owner did not request executing files as commands.

## Current evidence and affected scope

`InputPolicy` admits only `.md`/`.txt`, validates UTF-8 and rejects NUL. Files-tree
activation uses `Application::openTreePath`: those suffixes open internally;
other regular files already use `DesktopFileOpener`. HTTP(S) already has a browser
adapter and configurable browser program. Local Markdown link resolution still
uses the internal suffix restriction. Thus this is a policy extension and
consistent modifier route, not a new external-opening service from scratch.

Review UR-001/005/024/028, SR-005/006/011 and later amendments in
[requirements](../../../xfmd_requirements.md). Reuse
[FTR-003](../../../src/blueprint/feature/Feature-003--Document-Navigation.md),
[FUNC-002](../../../src/blueprint/functionality/Functionality-002--Local-File-Storage.md),
[FUNC-010](../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md)
and [FUNC-014](../../../src/blueprint/functionality/Functionality-014--Application-Preferences.md).
Inspect [InputPolicy.cpp](../../../src/application/io/InputPolicy.cpp),
[LinkResolver.cpp](../../../src/application/navigation/LinkResolver.cpp),
[ApplicationTree.cpp](../../../src/application/ApplicationTree.cpp),
[DesktopFileOpener.cpp](../../../src/application/adapters/DesktopFileOpener.cpp)
and [ExternalBrowser.cpp](../../../src/application/adapters/ExternalBrowser.cpp).

## Agent recommendations and open questions

- Use one target-routing policy across links/tree and the new entrypoints. Retain
  separate relative-path bases: document directory for Markdown links; work path
  for typed paths (KB-XFMD-008). Preserve modifier state through deferred dispatch.
- Define plain-text detection, including unknown/no suffix, empty files, encoding,
  binary input and size limits. Do not infer text solely from an extension list.
  Keep existing UTF-8/resource policies unless deliberately revised.
- Decide whether opening plain text forces editor mode or preserves the user's
  current view mode while presenting literal text. The owner explicitly wants
  editor access; plain text must not become Markdown merely due to content.
- For ordinary HTML/web activation, decide how the existing browser preference
  applies. Ctrl+click explicitly uses OS defaults, including when that preference
  specifies a different browser. HTML source editing would need a separate action;
  it is not the default requested here.
- Retain argv-based launching without shell evaluation and report launch failure.
  External opening should preserve the current buffer, dirty/undo and navigation
  history. Ordinary internal opening retains the shared dirty-check transaction.

## Next review and completion criteria

Review before selecting file-routing changes, alongside KB-XFMD-006/008. Link the
selected requirements/blueprints/sprint rather than treating this table as a new
normative specification.

Candidate acceptance: exercise the routing matrix from real link/tree clicks;
include uppercase suffixes, extensionless text, HTML, binary input, relative
links, spaces/Unicode, Ctrl+click without double activation, configured-browser
versus OS-default behavior, failed launch and canceled internal opens. No GUI or
routing tests were run while capturing this note.

## Outcome and related cards

Recorded in backlog only. Implementation has not been selected or started by
this capture. Findings describe the inspected working tree, not the installed
binary shown in the screenshots; see the [source context](../evidence/2026-09-24-ui-wishlist/README.md).

Related: [KB-XFMD-006](%23006--Change--Open-files-and-folders.md), [KB-XFMD-008](%23008--Change--Document-path-and-file-filter.md).
