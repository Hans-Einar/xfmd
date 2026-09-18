use crate::parse;
use xfmd_diagram_contracts::{Model, wire::Reader};
#[test]
fn architecture_profiles_roundtrip() {
    for source in [
        include_str!("../../../../../../docs/design/mermaid/c4-context.mmd"),
        include_str!("../../../../../../docs/design/mermaid/c4-container.mmd"),
        include_str!("../../../../../../docs/design/mermaid/c4-component.mmd"),
        include_str!("../../../../../../docs/design/mermaid/architecture-resources.mmd"),
        include_str!("../../../../../../docs/design/mermaid/block-layers.mmd"),
    ] {
        let bytes = parse(source.as_bytes()).unwrap();
        let m = Model::read(&mut Reader::new(&bytes)).unwrap();
        assert!(m.semantic.is_some());
        assert!(m.nodes.is_empty());
    }
}
#[test]
fn reject_architecture_semantic_loss() {
    for source in [
        "C4Context\nUnknown(A,\"a\")",
        "C4Context\nPerson(A,\"a\")\nRel(A,B,\"x\")",
        "architecture-beta\nservice A(custom)[A]",
        "architecture-beta\ngroup G[G] in Other",
        "block-beta\ncolumns 2\nA[A]:3",
        "block-beta\nblock:group\nA\nend",
        "architecture-beta\nservice A[A]\nA:R --> L:unknown",
    ] {
        assert!(parse(source.as_bytes()).is_err(), "{source}");
    }
}
