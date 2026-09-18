use crate::parse;
use xfmd_diagram_contracts::{Model, wire::Reader};
#[test]
fn planning_records_roundtrip() {
    for source in [
        include_str!("../../../../../../docs/design/mermaid/packet-encoding.mmd"),
        include_str!("../../../../../../docs/design/mermaid/timeline-decisions.mmd"),
        include_str!("../../../../../../docs/design/mermaid/gantt-pilot.mmd"),
        include_str!("../../../../../../docs/design/mermaid/journey-review.mmd"),
    ] {
        let bytes = parse(source.as_bytes()).unwrap();
        let m = Model::read(&mut Reader::new(&bytes)).unwrap();
        assert!(m.semantic.is_some());
        assert!(m.nodes.is_empty());
    }
}
#[test]
fn rejects_invalid_planning() {
    for source in [
        "packet\n0-7: \"A\"\n4-9: \"B\"",
        "packet\n+0: \"bad\"",
        "gantt\nA : a, 2026-02-30, 1d",
        "gantt\nA : a, after missing, 1d",
        "gantt\nexcludes weekends\nA : a, 2026-09-18, 1d",
        "journey\nA : 7 : User",
        "timeline\n: orphan",
    ] {
        assert!(parse(source.as_bytes()).is_err(), "{source}");
    }
}
