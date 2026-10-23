//! Map validated use-case records to native glyphs and UML relations.
use super::{edge, node};
use mermaid_rs_renderer::ir::*;
use xfmd_diagram_contracts::semantic::{Diagram, usecase::*};
pub fn populate(d: &Diagram, g: &mut Graph) {
    for r in d.records.iter().filter(|r| r.tag == ELEMENT) {
        let f = &r.fields;
        node(
            g,
            &f[0],
            f[1].clone(),
            if f[2] == "actor" {
                NodeShape::UseCaseActor
            } else if f[3] == "rectangle" {
                NodeShape::Rectangle
            } else {
                NodeShape::Ellipse
            },
        );
    }
    for r in d.records.iter().filter(|r| r.tag == BOUNDARY) {
        g.subgraphs.push(Subgraph {
            id: Some(r.fields[0].clone()),
            label: r.fields[1].clone(),
            nodes: d
                .records
                .iter()
                .filter(|c| c.tag == ELEMENT && c.fields[4] == r.fields[0])
                .map(|c| c.fields[0].clone())
                .collect(),
            direction: None,
            icon: None,
        });
    }
    for r in d.records.iter().filter(|r| r.tag == RELATION) {
        let f = &r.fields;
        let label = match f[2].as_str() {
            "include" => "«include»",
            "extend" => "«extend»",
            _ => &f[4],
        };
        let mut e = edge(&f[0], &f[1], label);
        e.arrow_start = f[3] == "reverse";
        e.arrow_end = f[3] == "forward";
        e.directed = e.arrow_start || e.arrow_end;
        e.arrow_start_kind = Some(EdgeArrowhead::OpenV);
        e.arrow_end_kind = Some(if f[2] == "generalization" {
            EdgeArrowhead::OpenTriangle
        } else {
            EdgeArrowhead::OpenV
        });
        if ["include", "extend"].contains(&f[2].as_str()) {
            e.style = EdgeStyle::Dotted;
        }
        g.edges.push(e);
    }
}
