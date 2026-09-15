use mermaid_rs_renderer::ir::{Edge, EdgeStyle, Node, Subgraph};
use mermaid_rs_renderer::{Direction, Graph, NodeShape};
use xfmd_diagram_contracts::Model;
pub fn direction(d: u32) -> Direction {
    match d {
        1 => Direction::LeftRight,
        2 => Direction::BottomTop,
        3 => Direction::RightLeft,
        _ => Direction::TopDown,
    }
}
pub fn graph(m: &Model) -> Graph {
    let mut g = Graph::new();
    g.direction = direction(m.direction);
    for (i, n) in m.nodes.iter().enumerate() {
        g.node_order.insert(n.id.clone(), i);
        g.nodes.insert(
            n.id.clone(),
            Node {
                id: n.id.clone(),
                label: n.label.clone(),
                shape: match n.shape {
                    1 => NodeShape::RoundRect,
                    2 => NodeShape::Diamond,
                    3 => NodeShape::Circle,
                    _ => NodeShape::Rectangle,
                },
                value: None,
                icon: None,
            },
        );
    }
    for e in &m.edges {
        g.edges.push(Edge {
            from: m.nodes[e.from as usize].id.clone(),
            to: m.nodes[e.to as usize].id.clone(),
            label: if e.label.is_empty() {
                None
            } else {
                Some(e.label.clone())
            },
            start_label: None,
            end_label: None,
            directed: e.arrows != 0,
            arrow_start: e.arrows & 1 != 0,
            arrow_end: e.arrows & 2 != 0,
            arrow_start_kind: None,
            arrow_end_kind: None,
            start_decoration: None,
            end_decoration: None,
            style: match e.style {
                1 => EdgeStyle::Dotted,
                2 => EdgeStyle::Thick,
                _ => EdgeStyle::Solid,
            },
        });
    }
    for s in &m.groups {
        g.subgraphs.push(Subgraph {
            id: Some(s.id.clone()),
            label: s.label.clone(),
            nodes: s
                .nodes
                .iter()
                .map(|n| m.nodes[*n as usize].id.clone())
                .collect(),
            direction: if s.direction < 4 {
                Some(direction(s.direction))
            } else {
                None
            },
            icon: None,
        });
    }
    g
}
