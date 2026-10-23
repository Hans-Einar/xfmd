# N3f-001 — Owner-reported VP07 detail routing failure

Session0005 T007, 2026-10-09. Analysis/reproduction only; not a fix.

Owner reports VP07 DocumentReferences detail failing with Libavoid NoSpace:
label attachment intrusion on edge 11. Reproduced through the same candidate
SDPTool target, normal native renderer and PDF probe: zero native diagram scenes;
PDF contains the exact error. This is a 12-node/15-edge detail graph, separate
from the newly added VP07 aggregate and its fit-to-width reading gap.

The strict renderer check in layout/routed/labels.rs rejects another edge crossing
the attachment zone between an edge and its label; the error is not an SDL mode
validation failure or invalid Mermaid declaration. One bounded expanded-spacing
retry already exists and is insufficient for this fixture. Root cause within label
placement/obstacle routing is not yet established. Preserve strict geometry,
source semantics, fixed candidate pin and existing original routing regression.
Next implementation boundary: reproduce with exact text metrics in the Mermaid
fork, correct attachment routing/placement, then test original and new fixtures
through native preview/PDF before dependency adoption. No guard removal or source
model rewrite is selected as a workaround.

NormalDesktop is the model's sole declared mode (XfmdDesktop.design line 180),
used by allocations, channel participation and scenarios. It denotes ordinary
standalone desktop execution; the name has no built-in runtime switch semantics.
Selecting it in VP07 filters modeled allocation facts, not the running app's mode.

Evidence: [target](target.json), [generated detail](generated/entry.md),
[native log](native.log), [PDF text](document-references.txt),
[PDF](document-references.pdf), [identity](identity.json).

Command: tools/run_with_xvfb.py /tmp/xfmd-n3e-native-probe generated/entry.md document-references
(using absolute evidence paths). Probe exit 5 records zero diagram scenes.
