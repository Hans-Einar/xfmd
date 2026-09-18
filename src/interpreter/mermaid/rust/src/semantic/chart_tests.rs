use crate::parse;
use xfmd_diagram_contracts::{Model, wire::Reader};
#[test]
fn chart_profiles_roundtrip() {
    for source in [
        include_str!("../../../../../../docs/design/mermaid/pie-evidence.mmd"),
        include_str!("../../../../../../docs/design/mermaid/mindmap-review.mmd"),
        include_str!("../../../../../../docs/design/mermaid/gitgraph-proposal.mmd"),
        include_str!("../../../../../../docs/design/mermaid/sankey-provenance.mmd"),
        include_str!("../../../../../../docs/design/mermaid/quadrant-priorities.mmd"),
        include_str!("../../../../../../docs/design/mermaid/zenuml-observation.mmd"),
        include_str!("../../../../../../docs/design/mermaid/kanban-review.mmd"),
        include_str!("../../../../../../docs/design/mermaid/radar-quality.mmd"),
        include_str!("../../../../../../docs/design/mermaid/treemap-effort.mmd"),
        include_str!("../../../../../../docs/design/mermaid/xychart-evidence.mmd"),
    ] {
        let bytes = parse(source.as_bytes()).unwrap_or_else(|e| panic!("{source}: {e}"));
        let model = Model::read(&mut Reader::new(&bytes)).unwrap();
        assert!(model.semantic.is_some());
        assert!(model.nodes.is_empty());
    }
}
#[test]
fn reject_loss_or_invalid_values() {
    for source in [
        "pie\n\"Bad\":NaN",
        "mindmap\n Root\n     Skipped",
        "gitGraph\ncommit",
        "sankey-beta\nA,B,1\nB,A,1",
        "quadrantChart\nX:[2,0]",
        "zenuml\nA.method()",
        "zenuml\nA-->B: reply",
        "kanban\nA[A]\n    B[B]",
        "radar-beta\naxis A,B,C\ncurve X{1,2}",
        "treemap-beta\n\"Root\": 1\n  \"Child\": 2",
        "xychart-beta\nx-axis [A,B]\nbar [1]",
        "xychart-beta\nx-axis [A]\nbar [Infinity]",
    ] {
        assert!(parse(source.as_bytes()).is_err(), "{source}");
    }
}

#[test]
fn git_branch_switches_context() {
    let bytes =
        parse(b"gitGraph\ncommit id:\"base\"\nbranch proposal\ncommit id:\"change\"").unwrap();
    let m = Model::read(&mut Reader::new(&bytes)).unwrap();
    let d = m.semantic.unwrap();
    let commit = d
        .records
        .iter()
        .find(|r| r.tag == 141 && r.fields[0] == "change")
        .unwrap();
    assert_eq!(commit.fields[1], "proposal");
    assert_eq!(commit.fields[2], "base");
}
