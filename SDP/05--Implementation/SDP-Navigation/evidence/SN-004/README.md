# SN-004 — XFMD 0.4 local development installation

Session T006, 2026-10-01. Owner explicitly selects version 0.4 and requests
installation of the latest XFMD. The version decision is in docs/versioning.md.
The local major/minor contract selects this minor feature delivery for #014/#021.

## Candidate and validation

Installed identity: `xfmd 0.4 sprint/009/phase/057-favorites:206+dirty`.
HEAD remains `41fa4948c5377edd792ba3f503e6b92b0fd4717a` plus the shared dirty working tree.
The only product change since SN-003 is VERSION, from 0.3 to 0.4. All product/test
source hashes recorded by SN-003 and every recorded SDL/SDUI source hash still
match. No model source or generated model view was edited in this turn.

- `cmake --build build-ui -j4`: passed; build.log.
- `ctest --test-dir build-ui -R '^(BuildIdentityTest|BuildVersionTest|SdpDiscoveryTest|SdpDynamicGuiTest|SdpNavigationGuiTest|BlueprintStructure|BlueprintSymbols)$' --output-on-failure`:
  7/7 passed on this 0.4 candidate, including real-producer native source opening.
- SN-003's broader 81-check results remain applicable to the unchanged product
  behavior; they are not represented as a newly executed full suite for 0.4.
- Generated build metadata, installed metadata, CLI version and CPack configuration
  identify 0.4 / package version 0.4.0. No package publication is claimed.
- Installed executable passed isolated Xvfb/temp-HOME startup; installed-smoke.log.

Binary SHA-256: `1cfb20cf177c91d35c7b66d41f890349a4ee9c11de3cf135b72950bd3be15eab`.
The installed executable matches build-ui/xfmd exactly. Candidate and baseline
hashes are in candidate.json; version.patch records the only product delta from
SN-003. No clean-commit or independent-review claim is made.

## Installation and preservation

CMake installation was staged, existing files backed up and payload files replaced
atomically, executable last. No existing user process was stopped. Receipt and
previous installed payload:

`/home/warloc/.local/state/xfmd/installations/20261001T194155Z`

The prior executable was XFMD 0.3 with hash
`485e236a927d4da95d1ca24c5885bfd7f65f50f91ebac336c6559903e32626bb`. Reproduction/install and smoke scripts are
retained beside the receipt; installation.json contains the installed file hashes.
Staged concurrent #020 remains preserved. No Git commit, tag, push, public release,
SDP-tool upgrade or project SDP upgrade was made.

The copied ReleaseChecklist.md records local-installation outcomes and explicit
N/A publication/SDP-publisher steps. Independent review and held model reconciliation
remain pending; this is not a passed public-release gate. Session S2 and #021's
updated real Sessions-producer check remain open. Version naming does not close them.
