# Activity0003 native consumer evidence

Development candidate: qualified Activity2 HEAD4c77944 plus the isolated readable
browsing working diff on sdp/readable-browsing. No clean-commit evidence claim.
Coordinator: PLAN-SDP-0028 / ACTIVITY-SDP-0003 B3–B5. Source contract and bounded
plan: sprints/Sprint-010--SDP-Navigation/Activity0003-Consumer-Handoff.md.

Nine targeted tests pass (four native GUI), retained in targeted-tests.log:
SdpReadingTest, SdpDisplayTest, SdpDiscoveryTest, SdpActivityTest,
SdpPanelGuiTest, SdpProtocolTest, SdpDynamicGuiTest, SdpRuntimeGuiTest and
SdpActivityGuiTest. Native tests use separate Xvfb displays and temporary homes.
They verify generic exact order/labels, primary activation versus loading,
presentation/capability mismatch and stale/foreign model rejection while preserving
legacy/reference behavior and existing Activity/runtime workflows.

Commands:

```sh
cmake --build build --parallel 4 --target xfmd SdpReadingGuiTest SdpReadingTest SdpDisplayTest SdpDiscoveryTest SdpActivityTest SdpPanelGuiTest SdpProtocolTest SdpDynamicGuiTest SdpRuntimeGuiTest SdpActivityGuiTest
ctest --test-dir build -R '^(SdpReadingTest|SdpDisplayTest|SdpDiscoveryTest|SdpActivityTest|SdpPanelGuiTest|SdpProtocolTest|SdpDynamicGuiTest|SdpRuntimeGuiTest|SdpActivityGuiTest)$' --output-on-failure
python3 tools/validate_blueprints.py
python3 tools/check_blueprint_symbols.py
python3 packaging/rpm/test-producer-pin.py
```

Blueprint metadata and 311 implemented callees pass their structural checks;
these are not model semantics or call-graph proofs. Two producer-pin tests pass
(actual Git archive identity/digest and hostile members/links). Local-CI version
and publisher policy tests also pass. The full configured 90-test regression passes. Independent integrated review and
RPM/public DNF evidence remain pending.


## Exact producer native qualification

Final supplied producer source 7370e29a46b392b146f26e694f1da55153a09647 is built
from the digest-checked Git archive by build-producer.py. producer-build.json
records the source archive, embedded revision, normalized binary digest and Go
toolchain. development-binaries.sha256 identifies that helper and the dirty native
test binary. This is exact producer proof; the consumer is still 4c77944 plus the
recorded working diff until its milestone commit and clean package build.

Fixture and read-only HEOS Micro/Base runs retained under fixture/, heos/ and
heos-base/ verify: deferred System primary activation before children; expansion
preserving the document; VP01–VP11 exact sibling order; opaque collection and
object actions; exact source selection; Source files overview and file projection;
retained Back without regeneration; and native/PDF output. All eleven landing
actions are activated in each actual native run. Every advertised Mermaid block
produces a nonempty DiagramScene/SVG; every advertised image has a visual resource.
Backend diagnostics are recorded as renderer instrumentation, not treated as
errors merely because they include routing metrics or leader omission counts.

The fixture selects VP04 Interfaces/Console; Micro selects VP08 Contracts with
all 13 chapters; Base selects VP02 Units/Kernel. The HEOS source files are only
read. Commands use tools/run_with_xvfb.py, XFMD_SDP_TOOL selecting the identified
helper and XFMD_UI_EVIDENCE selecting separate output directories. HEOS overrides
XFMD_READING_REAL_PROJECT and the Base run additionally selects source label
System.design, viewpoint 2, collection Units and focus Kernel. The default configured
reading test remains a disposable fixture. Final independent integrated review and
clean exact-consumer package/publication evidence remain pending.

## Structural panel refinement and final bounded checks

The captured development producer now partitions containment and Channel/Mode
figures into readable semantic panels. Current retained HEOS Base evidence uses
System.design: 29 root visual panels in a 28-page PDF and Kernel detail in an 11-page
PDF. Native screenshots and A4 raster inspection of Kernel PDF pages 2/4 show
readable containment and FileApi labels, correctly nested Units, separate Kernel
boundary participation and an external ApplicationTasks occurrence. Micro root
has 12 panels; the VP08 Contracts walkthrough remains complete. At most four root
visual screenshots are captured; all SVG panels and the full PDF are retained.
These observations resolve the earlier dense-label concern in the inspected figures;
independent review owns final integrated acceptance.

Final real workflow tests additionally activate the supplied Source files overview
and first file's source projection through native Enter, preserving opaque source
selectors and retained Back history. The fixture checks dependency freshness
without modifying HEOS. Source-bundle Markdown and SVG paths remain together in
structure-assets/; focused Kernel SVGs sit beside its reading-page.md.
