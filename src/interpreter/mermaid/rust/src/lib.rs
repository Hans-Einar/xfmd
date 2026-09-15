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
