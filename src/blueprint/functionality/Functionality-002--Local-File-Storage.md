---
id: FUNC-002
kind: Functionality
audience: System
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-003, UR-009, UR-028, SR-005, SR-006, SR-007, SR-011, SR-013
uses: none
---

# Functionality-002: Local storage and input policy

## 1. Purpose and scope

Reusable local file I/O and input validation, independent of widgets, history and Markdown layout. Validation concerns transport, encoding and file policy; incomplete Markdown is not a storage error.

## 2. Requirements and acceptance

UR-001, UR-003, UR-009, UR-028 and SR-005, SR-006, SR-007, SR-011, SR-013. Normative definitions and acceptance are in the [requirements](../../../xfmd_requirements.md).

## 3. Contracts and ownership

`LocalFileStore::read` and `writeAtomic` return owned data or throw `Error`.
`InputPolicy::validate` checks bounded UTF-8 and control bytes independently of suffix.
`plainText` is false only for a case-insensitive `.md` suffix; `supportedPath` identifies
known `.md`/`.txt` types for which invalid content must remain an internal error.
`FileOpenPolicy::classify` returns Markdown, Text, Browser or Desktop for Application.
It owns classification, not launchers or document state.

## 4. Behavior, state and failures

Read only regular files with an enforced 8 MiB limit. Write a temporary file in
the target directory, preserving owner, mode and xattrs/ACL. Reject hardlinks.
Repeat external-identity checks before rename; stat/hash is not atomic CAS.
Publish new files without overwrite through link/unlink. A directory-sync failure
after commit returns `durable=false`.

P053, 2026-09-24, supersedes suffix-only admission: `.html`/`.htm` route to the
browser; known `.md`/`.txt` use internal loading and report invalid input.
Other files use bounded content validation: valid UTF-8, including empty files
and an optional BOM, opens as literal text; NUL, binary control bytes, invalid
UTF-8 or oversize unknown content routes to OS association. TAB/LF/CR/FF are allowed;
other C0 bytes and DEL are rejected. I/O failures propagate instead of launching
an unreadable target. Atomic-save behavior and byte preservation remain unchanged.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `DocumentCoordinator::requestOpen` | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Path → bytes/identity | Bounded read and stat check | Implemented |
| 2 | `LocalFileStore::read / writeAtomic` | `InputPolicy::validate` | `src/application/io/InputPolicy.cpp` | Bytes → validation | Invalid input throws Error | Implemented |
| 3 | `DocumentCoordinator::save` | `LocalFileStore::writeAtomic` | `src/application/io/LocalFileStore.cpp` | Bytes/expected identity → saved target | Remove temporary file on failure | Implemented |
| 4 | `LocalFileStore::writeAtomic` | `copyAttributes` | `src/application/io/LocalFileStore.cpp` | Open descriptors → preserved metadata | Failure before commit | Implemented |
| 5 | `Application::openTarget / openDialogPath` | `FileOpenPolicy::classify` | `src/application/io/FileOpenPolicy.cpp` | Regular path → selected consumer | Bounded validation; I/O failures preserve document | Implemented |

## 6. Reuse and dependencies

No other functionality contract is consumed. Shared types follow the architecture.
DocumentCoordinator is the first consumer; future export may reuse storage if its
metadata/conflict policy fits. Features must not introduce competing save methods.

Sprint 004 consumer, reconstructed 2026-09-24: [FUNC-031](Functionality-031--Generated-Document-Navigation.md)
reuses LocalFileStore through the navigator DocumentCoordinator. The broker owns
generated bundle storage and lifetime.

## 7. Verification

AT-001, AT-003, AT-009, AT-015, AT-016, AT-017, AT-021, AT-023, AT-048. `DocumentTest` injects pre-rename errors and
checks xattrs, modes, hardlinks, new-file identity, external modification, invalid
UTF-8 and 8 MiB. Historical evidence: [P2](../../../docs/evidence/P2.md).
P7 evaluates combined requirement coverage; Implemented is not automatically Verified.

P053 adds FileOpenPolicyTest and native file-routing/dialog checks. Evidence: [P053 opening evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P053.md).

## 8. Status, risks and change impact

Implemented originally in P2; input admission and classification revised in P053.
Update contracts, call maps, consumers and tests together. Ports and ownership
remain explicit. Unknown textual files require a bounded probe and another read
when the document transaction opens them; the loader revalidates before commit.
Historical evidence retains its original scope.
