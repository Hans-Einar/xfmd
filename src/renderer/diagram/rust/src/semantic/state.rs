use super::{edge, node};
use mermaid_rs_renderer::ir::*;
use xfmd_diagram_contracts::semantic::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    let native_id = |id: &str| match d
        .records
        .iter()
        .find(|r| r.tag == STATE && r.fields[0] == id)
        .map(|r| r.fields[3].as_str())
    {
        Some("initial") => format!("__start_{id}__"),
        Some("final") => format!("__end_{id}__"),
        _ => id.to_string(),
    };
    for r in d.records.iter().filter(|r| r.tag == STATE) {
        let f = &r.fields;
        let shape = match f[3].as_str() {
            "initial" => NodeShape::Circle,
            "final" => NodeShape::DoubleCircle,
            "choice" => NodeShape::Diamond,
            "fork" | "join" => NodeShape::ForkJoin,
            _ => NodeShape::RoundRect,
        };
        node(
            g,
            &native_id(&f[0]),
            if matches!(
                f[3].as_str(),
                "initial" | "final" | "choice" | "fork" | "join"
            ) {
                String::new()
            } else {
                f[1].clone()
            },
            shape,
        );
    }
    for r in d
        .records
        .iter()
        .filter(|r| r.tag == STATE && r.fields[3] == "composite")
    {
        let mut members = vec![];
        for child in d.records.iter().filter(|r| r.tag == STATE) {
            let mut parent = child.fields[2].as_str();
            for _ in 0..128 {
                if parent == r.fields[0] {
                    members.push(native_id(&child.fields[0]));
                    break;
                }
                if parent.is_empty() {
                    break;
                }
                parent = d
                    .records
                    .iter()
                    .find(|p| p.tag == STATE && p.fields[0] == parent)
                    .map(|p| p.fields[2].as_str())
                    .unwrap_or("");
            }
        }
        g.subgraphs.push(Subgraph {
            id: Some(r.fields[0].clone()),
            label: r.fields[1].clone(),
            nodes: members,
            direction: None,
            icon: None,
        });
    }
    // The dependency recognizes reserved region IDs to draw concurrent dividers.
    for (i, r) in d.records.iter().filter(|r| r.tag == REGION).enumerate() {
        if d.records
            .iter()
            .filter(|p| p.tag == REGION && p.fields[1] == r.fields[1])
            .count()
            < 2
        {
            continue;
        }
        let id = format!("__region_{i}__");
        let nodes = d
            .records
            .iter()
            .filter(|p| p.tag == STATE && p.fields[4] == r.fields[0])
            .map(|p| native_id(&p.fields[0]))
            .collect();
        g.subgraphs.push(Subgraph {
            id: Some(id.clone()),
            label: String::new(),
            nodes,
            direction: None,
            icon: None,
        });
        g.subgraph_styles.insert(
            id,
            NodeStyle {
                fill: Some("none".into()),
                stroke: Some("none".into()),
                stroke_width: Some(0.),
                ..Default::default()
            },
        );
    }
    for r in d.records.iter().filter(|r| r.tag == TRANSITION) {
        g.edges.push(edge(
            &native_id(&r.fields[0]),
            &native_id(&r.fields[1]),
            &r.fields[2],
        ));
    }
}
