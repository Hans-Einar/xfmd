mod model;
mod profile;
use xfmd_diagram_contracts::wire::Writer;
pub fn parse(source: &[u8]) -> Result<Vec<u8>, String> {
    let source = std::str::from_utf8(source).map_err(|_| "Invalid UTF-8")?;
    let profile = profile::inspect(source)?;
    let parsed = mermaid_rs_renderer::parse_mermaid_strict(source).map_err(|e| e.to_string())?;
    let model = model::map_graph(parsed.graph, &profile)?;
    let mut writer = Writer::default();
    model.write(&mut writer);
    Ok(writer.0)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn examples() {
        for s in [
            include_str!("../../../../../docs/design/mermaid/service-map.mmd"),
            include_str!("../../../../../docs/design/mermaid/layer-delivery.mmd"),
        ] {
            assert!(parse(s.as_bytes()).is_ok());
        }
    }
    #[test]
    fn reject_loss() {
        for s in [
            "flowchart LR\nA-->B\nclick A call()",
            "flowchart LR\nA[ok] gibberish",
            "flowchart LR\nA[[shape]]",
            "flowchart LR\nsubgraph X\nA-->B",
            "flowchart LR\n%%{init: {}}%%\nA-->B",
        ] {
            assert!(parse(s.as_bytes()).is_err(), "{s}");
        }
    }
    #[test]
    fn labels_not_commands() {
        assert!(parse(b"flowchart LR\nA[click] -->|style| B[end]").is_ok());
    }
}
#[cfg(test)]
mod profile_matrix {
    use super::parse;
    use xfmd_diagram_contracts::{Model, wire::Reader};
    fn model(s: &str) -> Model {
        let bytes = parse(s.as_bytes()).unwrap();
        Model::read(&mut Reader::new(&bytes)).unwrap()
    }
    #[test]
    fn shapes_arrows_and_directions() {
        for direction in ["LR", "RL", "TD", "TB", "BT"] {
            let m = model(&format!(
                "flowchart {direction}\nA[Ærlig] <--> B(Round)\nB -.-> C{{Choice}}\nC ==> D((Circle))\nD --> D"
            ));
            assert_eq!(
                m.nodes.iter().map(|n| n.shape).collect::<Vec<_>>(),
                vec![0, 1, 2, 3]
            );
            assert_eq!(m.edges[0].arrows, 3);
            assert_eq!(m.edges[1].style, 1);
            assert_eq!(m.edges[2].style, 2);
            assert_eq!(m.edges[3].from, m.edges[3].to);
        }
    }
    #[test]
    fn groups_and_chain() {
        let m = model(
            "flowchart LR\nsubgraph G [Outer]\ndirection TD\nsubgraph H [Inner]\nA-->B-->C\nend\nend\nC-->A\nA-->B",
        );
        assert_eq!(m.groups.len(), 2);
        assert_eq!(m.edges.len(), 4);
        assert_eq!(m.groups[1].parent, 0);
    }
    #[test]
    fn unsupported_is_local_error() {
        for body in [
            "A-->B\nstyle A fill:red",
            "A:::red",
            "A[[Subroutine]]",
            "A[(Database)]",
            "A[<b>HTML</b>]",
            "A[`Markdown`]",
            "A-->B\nclassDef c fill:red",
            "A-->B\nclick A href https://example.com",
            "A & B --> C",
            "A@{img: x}",
        ] {
            assert!(
                parse(format!("flowchart LR\n{body}").as_bytes()).is_err(),
                "{body}"
            );
        }
    }
    #[test]
    fn arbitrary_input_does_not_panic() {
        let mut seed = 1u64;
        for length in 0..512 {
            let mut input = b"flowchart LR\n".to_vec();
            for _ in 0..length {
                seed = seed.wrapping_mul(6364136223846793005).wrapping_add(1);
                input.push((seed >> 32) as u8);
            }
            let _ = parse(&input);
        }
        assert!(parse(&vec![b'A'; 65537]).is_err());
    }
}
