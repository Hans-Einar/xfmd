# RT-002 — XFMD 0.5 local candidate

Owner Session 0002 T009 selects version 0.5 and local installation. Package version
is 0.5.0; branch/count/+dirty remains visible. No public publication is authorized.

Release build and CPack TGZ completed. The extracted executable matches the build
SHA256, reports 0.5, and ldd reports no missing system libraries. package.json
identifies exact archive/binary bytes; xfmd-build.json records the dirty baseline.
candidate-sources.tar.gz captures the current product/test inputs. The archive
contains preceding changes, not just this turn's version adjustment.

Five focused checks PASS on this candidate: BuildVersionTest, SdpIntegrationTest,
StandaloneMermaidTest, HeadingLinkTest and SdpRuntimeTest. Logs are retained here.
Native acceptance is still pending: RT-001 records the previous socket/Xvfb
restriction; no GUI pass is claimed for this rebuild. No independent review.

Included: #023 raw Mermaid, #024 local heading fragments, and #025's private
runtime allocation, lazy parent activation and bundle cleanup. The producer's
24-diagram and other per-result limits remain open. No authored model updates.
See ReleaseChecklist.md for applicable and N/A gates.

## Installation status

NOT INSTALLED. The current host's writable roots are the repository and /tmp;
/home/warloc/.local is outside that permission boundary. Owner authorization is
present, but does not change the tool sandbox. No out-of-bound writes attempted.
The previously installed 0.4 remains in place.

install.py is prepared for execution with Python in an unrestricted user terminal.
It verifies the package, backs up replaced files, uses atomic replacements, checks
all installed hashes and isolated native startup, and rolls back on failure.
Successful execution creates a receipt under ~/.local/state/xfmd/installations;
no such receipt or successful installation is claimed by this record.
