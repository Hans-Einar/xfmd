# Follow local file links with heading fragments

| Field | Value |
| --- | --- |
| id | KB-XFMD-024 |
| project | XFMD |
| type | Change |
| CardState | gate-review |
| created | 2026-10-04T00:00:00Z |
| source | Owner Session 0002 T003 |
| next_review | Owner review of MM-001 local Release candidate |
| tags | navigation, links, headings |

## Owner outcome and analysis

Follow `/somePath/someFile.md#SomeLocationInTheFile`. Main Application currently
passes the complete target to LinkResolver::resourcePath, which rejects #. The
navigator has a separate restricted matcher. This is a missing main workflow,
not a file-not-found error. Existing index grouping deliberately strips fragments
and remains a separate file/source-location interaction.

## Selected work and acceptance

Implement UR-048/AT-077 and FUNC-008 as specified before code in
[PLAN-XFMD-0007](../../05--Implementation/Standalone-Mermaid/Plan.md).
Use exact heading titles and generated slugs, live-buffer same-file jumps and
transactional cross-file navigation. Test percent escapes, duplicate slugs,
missing targets, dirty Cancel and asynchronous token ownership. Do not add HTML
explicit IDs or change external browser dispatch. Keep #023 Mermaid work separate
but deliver both in the owner-requested local Release candidate.

## Session and outcome

[Session 0002](../../Sessions/session-%230002--Standalone-Mermaid-files.md), T003.
Implemented and included in the MM-001 local Release candidate.
[Evidence](../../05--Implementation/Standalone-Mermaid/evidence/MM-001/README.md):
85/85 regression checks, actual absolute file#heading click, relative/same-buffer
jumps, source-anchor history, dirty Cancel and stale-result protection.
Independent/owner review pending; no publication or installation. Model-source
hold is preserved.

## T004 installation — 2026-10-05

Owner authorized installation of the prepared candidate. [MM-002](../../05--Implementation/Standalone-Mermaid/evidence/MM-002/README.md)
records checksum-verified package installation, all-file hash verification, backup
and installed native startup. Prior no-install statements describe T003's boundary;
T004 supersedes that boundary only. Review/acceptance remains pending; no public
release or product/model source change occurred.
