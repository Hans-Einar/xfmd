# Concise product version — 2026-11-10

KB-XFMD-031 → UR-042/SR-025 → FUNC-026 → AT-066/AT-067.
Owner asks to remove the visible branch/commit/dirty suffix. The display now uses
VERSION's major.minor; complete build provenance remains in JSON. The CLI, window
title and status label already share buildVersion(), so their call graph is unchanged.
Requirements, architecture, blueprint and versioning documentation now describe this
separation. Historical delivery records retain their original identities.

## Candidate and verification

Main 953d09ec9bee5ef573893cfa73afed13233dfb67 plus the uncommitted working-tree
change. [Installation receipt](installation.json) records changed executable-source
hashes, exact installed binary, metadata and old executable backup. Same-context
verification only; no independent review, public release or RPM publication.

Commands executed on AlmaLinux, using the configured Rust 1.92.0 toolchain:

```sh
./tools/bootstrap_dependencies.sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DCMAKE_INSTALL_PREFIX=/home/warloc/.local
cmake --build build --parallel 4 --target xfmd CompactWorkspaceTest
ctest --test-dir build --output-on-failure -R '^(BuildIdentityTest|BuildVersionTest|CompactWorkspaceTest)$'
cmake --install build
```

All three focused CTests pass; [retained output](tests.log). BuildIdentityTest
covers seven disposable real-Git scenarios, metadata preservation, an unchanged
header after commit-only changes and a refreshed header after changing VERSION.
CompactWorkspaceTest exercises the real FOX title/status at four window widths
in isolated Xvfb. BuildVersionTest compares actual CLI output to generated metadata.
The installed executable reports exactly `xfmd 0.9`; its SHA256 and installed
metadata match the tested build. The previous executable is retained in the backup
recorded in the receipt. Running windows require a restart to use the new binary.

Blueprint structure, plumbing-symbol and KanBan replay checks pass, as does
`git diff --check`. This is focused display-change verification, not a new full
regression or release acceptance run. Historical GitHub test failures have not been
repaired or reclassified by this work.
