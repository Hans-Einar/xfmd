use mermaid_rs_renderer::{Direction, Graph, NodeShape};
use xfmd_diagram_contracts::{Edge, Group, Model, Node};
pub fn direction(d: Direction) -> u32 {
    match d {
        Direction::TopDown => 0,
        Direction::LeftRight => 1,
        Direction::BottomTop => 2,
        Direction::RightLeft => 3,
    }
}
pub fn map_graph(graph: Graph, profile: &super::profile::Profile) -> Result<Model, String> {
    let mut order: Vec<_> = graph.nodes.values().collect();
    order.sort_by_key(|n| {
        (
            graph.node_order.get(&n.id).copied().unwrap_or(usize::MAX),
            n.id.clone(),
        )
    });
    if graph
        .nodes
        .keys()
        .cloned()
        .collect::<std::collections::BTreeSet<_>>()
        != profile.nodes
        || graph.edges.len() != profile.edges.len()
        || graph.subgraphs.len() != profile.parents.len()
    {
        return Err("Mermaid parser did not preserve the complete diagram".into());
    }
    let mut nodes = Vec::new();
    for n in order {
        nodes.push(Node {
            id: n.id.clone(),
            label: n.label.clone(),
            shape: match n.shape {
                NodeShape::Rectangle => 0,
                NodeShape::RoundRect => 1,
                NodeShape::Diamond => 2,
                // The pinned upstream maps ((label)) to DoubleCircle. The profile
                // rejects triple parentheses, so both variants mean our circle here.
                NodeShape::Circle | NodeShape::DoubleCircle => 3,
                _ => return Err("Unsupported node shape".into()),
            },
        })
    }
    let index = |id: &str| {
        nodes
            .iter()
            .position(|n| n.id == id)
            .map(|i| i as u32)
            .ok_or("Missing node".to_string())
    };
    let mut edges = Vec::new();
    for (e, expected) in graph.edges.iter().zip(&profile.edges) {
        if (&e.from, &e.to) != (&expected.0, &expected.1) {
            return Err("Mermaid parser changed edge endpoints".into());
        }
        edges.push(Edge {
            from: index(&e.from)?,
            to: index(&e.to)?,
            label: e.label.clone().unwrap_or_default(),
            arrows: u32::from(e.arrow_start) | (u32::from(e.arrow_end) << 1),
            style: match e.style {
                mermaid_rs_renderer::ir::EdgeStyle::Solid => 0,
                mermaid_rs_renderer::ir::EdgeStyle::Dotted => 1,
                mermaid_rs_renderer::ir::EdgeStyle::Thick => 2,
            },
        });
    }
    let mut groups = Vec::new();
    for (i, g) in graph.subgraphs.iter().enumerate() {
        groups.push(Group {
            id: g.id.clone().unwrap_or_default(),
            label: g.label.clone(),
            direction: g.direction.map(direction).unwrap_or(4),
            parent: profile.parents[i],
            nodes: g.nodes.iter().map(|s| index(s)).collect::<Result<_, _>>()?,
        })
    }
    Ok(Model {
        direction: direction(graph.direction),
        nodes,
        edges,
        groups,
    })
}
