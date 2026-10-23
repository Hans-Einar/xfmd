use super::*;
use xfmd_diagram_contracts::{
    Model,
    semantic::usecase::*,
    wire::{Reader, Writer},
};
fn diagram(s: &str) -> Diagram {
    parse(s, "usecase-beta").unwrap().semantic.unwrap()
}
#[test]
fn usecase_roundtrip_preserves_roles_membership_and_relation_semantics() {
    let d = diagram(include_str!(
        "../../../../../../tests/fixtures/mermaid/usecase-native.mmd"
    ));
    assert_eq!(d.family, 23);
    assert_eq!(d.records.iter().filter(|r| r.tag == ELEMENT).count(), 6);
    assert_eq!(d.records.iter().filter(|r| r.tag == RELATION).count(), 7);
    assert_eq!(d.records.iter().filter(|r| r.tag == BOUNDARY).count(), 1);
    assert!(d.records.iter().any(|r| r.tag == ELEMENT
        && r.fields
            == [
                "Isolated",
                "Independent use case",
                "usecase",
                "ellipse",
                "Viewer"
            ]));
    assert!(d.records.iter().any(|r| r.tag == RELATION
        && r.fields
            == [
                "Reader",
                "Export",
                "association",
                "forward",
                "include is plain text"
            ]));
    let m = Model {
        semantic: Some(d),
        sequence: None,
        direction: 0,
        nodes: vec![],
        edges: vec![],
        groups: vec![],
    };
    let mut w = Writer::default();
    m.write(&mut w);
    let mut r = Reader::new(&w.0);
    assert_eq!(Model::read(&mut r).unwrap(), m);
    r.finish().unwrap();
}
#[test]
fn forward_references_unicode_and_reverse_arrows() {
    let d = diagram("usecase-beta\nA <-- B\nactor A(\"Ærlig bruker\")\nB[\"Open file\"]\nB --> C");
    assert!(
        d.records
            .iter()
            .any(|r| r.tag == ELEMENT && r.fields == ["A", "Ærlig bruker", "actor", "ellipse", ""])
    );
    assert!(
        d.records
            .iter()
            .any(|r| r.tag == ELEMENT && r.fields == ["C", "C", "usecase", "ellipse", ""])
    );
    assert!(
        d.records
            .iter()
            .any(|r| r.tag == RELATION && r.fields[3] == "reverse")
    );
}
#[test]
fn malformed_or_unimplemented_usecase_syntax_is_not_dropped() {
    for body in [
        "actor A\nA ..> : include B",
        "actor A\nA --|> B",
        "A(x)\nA[y]",
        "systemBoundary X\nend",
        "systemBoundary X\nA\n",
        "end",
        "systemBoundary X\nsystemBoundary Y\nA\nend\nend",
        "systemBoundary X\nA --> B\nend",
        "actor A@{type: hollow}",
        "A(x) extra",
        "A --> B trailing",
        "A ..> B",
        "A --o B",
        "note for A \"note\"",
        "actor A\nstyle A fill:red",
        "direction XX\nA",
        "A\ndirection LR\ndirection TB",
        "A[<script>]",
        "A(\"---\")",
        "systemBoundary X\nA\nend\nX --> A",
        "\"Generated ID\"",
        "A(\"broken)",
    ] {
        assert!(
            parse(&format!("usecase-beta\n{body}"), "usecase-beta").is_err(),
            "{body}"
        );
    }
}
#[test]
fn usecase_contract_rejects_invalid_records_without_parser() {
    let d = diagram("usecase-beta\nactor A\nA --> B");
    for field in ["bad kind", "<script>"] {
        let mut bad = d.clone();
        bad.records
            .iter_mut()
            .find(|r| r.tag == ELEMENT)
            .unwrap()
            .fields[2] = field.into();
        assert!(bad.validate().is_err());
    }
    let mut bad = d.clone();
    bad.records
        .iter_mut()
        .find(|r| r.tag == RELATION)
        .unwrap()
        .fields[2] = "include".into();
    assert!(bad.validate().is_err());
    let mut bad = d;
    bad.records
        .iter_mut()
        .find(|r| r.tag == RELATION)
        .unwrap()
        .fields
        .pop();
    assert!(bad.validate().is_err());
}
