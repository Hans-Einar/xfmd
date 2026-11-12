# Activity0003 generic reading consumer and RPM handoff

| Field | Value |
| --- | --- |
| PlanType | ImplementationPlan |
| BranchPolicy | sdp/readable-browsing stacked on qualified Activity2 4c77944 |
| CommitPolicy | milestone |
| Authority | ACTIVITY-SDP-0003 / PLAN-SDP-0028 B3–B5; owner selected XFMD 0.10 |

Canonical contract: /home/warloc/git/SDP/SDP/Activities/Activity-0003--Readable-SDL-browsing/04-Design/ImplementationContract.md.
This bounded consumer assignment preserves the dirty primary worktree and makes
no main merge or publication before coordinating qualification.

## Design and source reconciliation

UR-046/SR-029 and FUNC-032 retain ownership. Canonical SDL mapping remains
SdpProjectNavigation owned by SdpProjects, consuming SdpToolProtocol in
SDP/SDL/XFMD/XfmdDesktop.design. This is a generic presentation adapter change;
no new service, SDL parser, category dictionary or source-model entity is needed.
Wire details and native guarantees belong in the blueprint and executable tests,
not invented SDL properties. Existing structural model coverage remains bounded.

Trees marked presentation=producer-reading/1 preserve producer labels, child
arrays and primary target, with expansionTarget independent. The discovery
capability readingNavigation=1 must agree with its tree presentation. Deferred
model responses retain that presentation and exact project/model/revision.
References do not alter visible labels or replace primary activation on this path.
Legacy producer display and activation remain supported. Select URIs are opaque;
no collection/register/focus selectors are reconstructed by XFMD.

## Milestones and evidence

1. Reconcile consumer design and protocol readiness before product changes.
2. Implement decoding, ordered display and independent activation/expansion;
   verify valid/invalid capability envelopes, stale expansions, labels/order,
   legacy behavior and Activity2 regressions. Real native producer tests follow
   once the coordinator supplies the integrated producer.
3. Set product 0.10 and package exact committed producer source with immutable
   archive digest/provenance, source/licenses and installed helper checks.
   Final producer pin remains pending; never guess it from an intermediate tree.
4. Return exact candidate, test artifacts and release checklist to coordinating
   independent review; publish only after coordinating qualification.

Current status: milestones 1–2 implemented and development-qualified. All 90
configured CTests pass; six final affected checks pass after exact-label tooltip
refinement. Actual native fixture/Micro/Base workflows cover primary activation,
loading, collections, Kernel detail, exact source selection, retained Back and
native/PDF export. Broader visual acceptance remains with coordinating review.
Milestone 3 packaging machinery is prepared; final producer pin and exact package
are pending. Milestone 4 independent review/publication is not complete.

SdpProtocol.cpp is 326 lines after this additive validation. Keeping its shared
envelope, graph and source-context admission together avoids an unrelated decoder
refactor; no parsing/projection semantics have entered this application boundary.


Final producer supplied for qualification is 7370e29a46b392b146f26e694f1da55153a09647.
All eleven viewpoint landing actions render in the actual native fixture and
HEOS Micro/Base runs; advertised Mermaid/image content has corresponding native
visual resources. Source files overview and individual source projections retain
the advertised URI without native selector construction. The immutable source
archive pin and private helper build are prepared for the next packaging milestone.
