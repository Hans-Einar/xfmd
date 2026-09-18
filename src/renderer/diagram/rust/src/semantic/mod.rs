mod architecture;
mod block;
mod c4;
mod charts;
mod planning;
mod state;
mod structures;
mod types;
use mermaid_rs_renderer::ir::*;
use xfmd_diagram_contracts::semantic::*;
pub fn graph(d: &Diagram) -> Result<Graph, String> {
    d.validate()?;
    let mut g = Graph::new();
    g.kind = match d.family {
        2 => DiagramKind::State,
        3 => DiagramKind::Class,
        4 => DiagramKind::Requirement,
        5 => DiagramKind::Er,
        6 => DiagramKind::C4,
        7 => DiagramKind::Architecture,
        8 => DiagramKind::Block,
        9 => DiagramKind::Packet,
        10 => DiagramKind::Timeline,
        11 => DiagramKind::Gantt,
        12 => DiagramKind::Journey,
        13 => DiagramKind::Pie,
        14 => DiagramKind::Mindmap,
        15 => DiagramKind::GitGraph,
        16 => DiagramKind::Sankey,
        17 => DiagramKind::Quadrant,
        18 => DiagramKind::ZenUML,
        19 => DiagramKind::Kanban,
        20 => DiagramKind::Radar,
        21 => DiagramKind::Treemap,
        22 => DiagramKind::XYChart,
        _ => return Err("Unknown semantic family".into()),
    };
    if let Some(r) = d.records.iter().find(|r| r.tag == DIRECTION) {
        g.direction = match r.fields[0].as_str() {
            "LR" => Direction::LeftRight,
            "RL" => Direction::RightLeft,
            "BT" => Direction::BottomTop,
            _ => Direction::TopDown,
        };
    }
    match d.family {
        2 => state::populate(d, &mut g),
        3..=5 => types::populate(d, &mut g),
        6 => c4::populate(d, &mut g),
        7 => architecture::populate(d, &mut g),
        8 => block::populate(d, &mut g),
        9..=12 => planning::populate(d, &mut g),
        13 | 16 | 17 | 20 | 22 => charts::populate(d, &mut g),
        14 | 15 | 18 | 19 | 21 => structures::populate(d, &mut g),
        _ => unreachable!(),
    }
    Ok(g)
}
pub(super) fn node(g: &mut Graph, id: &str, label: String, shape: NodeShape) {
    let index = g.nodes.len();
    g.node_order.insert(id.into(), index);
    g.nodes.insert(
        id.into(),
        Node {
            id: id.into(),
            label,
            shape,
            value: None,
            icon: None,
        },
    );
}
pub(super) fn edge(from: &str, to: &str, label: &str) -> Edge {
    Edge {
        from: from.into(),
        to: to.into(),
        label: (!label.is_empty()).then(|| label.into()),
        start_label: None,
        end_label: None,
        directed: true,
        arrow_start: false,
        arrow_end: true,
        arrow_start_kind: None,
        arrow_end_kind: None,
        start_decoration: None,
        end_decoration: None,
        style: EdgeStyle::Solid,
    }
}
