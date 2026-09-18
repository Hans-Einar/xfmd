use super::{edge, node};
use mermaid_rs_renderer::ir::*;
use xfmd_diagram_contracts::semantic::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    for r in d
        .records
        .iter()
        .filter(|r| matches!(r.tag, CLASS | REQUIREMENT | ENTITY))
    {
        let id = &r.fields[0];
        let children: Vec<_> = d.records.iter().filter(|c| c.fields[0] == *id).collect();
        let mut lines = vec![];
        match r.tag {
            CLASS => {
                for c in children.iter().filter(|c| c.tag == ANNOTATION) {
                    lines.push(format!("<<{}>>", c.fields[1]));
                }
                lines.push(r.fields[1].clone());
                for tag in [ATTRIBUTE, OPERATION] {
                    let values: Vec<_> = children.iter().filter(|c| c.tag == tag).collect();
                    if !values.is_empty() {
                        lines.push("---".into());
                        for c in values {
                            lines.push(c.fields[1].clone());
                        }
                    }
                }
            }
            REQUIREMENT => {
                lines.push(format!("<<{}>>\n{id}", r.fields[1]));
                for c in children.iter().filter(|c| c.tag == PROPERTY) {
                    lines.push(format!("{}: {}", c.fields[1], c.fields[2]));
                }
            }
            ENTITY => {
                lines.push(id.clone());
                let attrs: Vec<_> = children
                    .iter()
                    .filter(|c| c.tag == ENTITY_ATTRIBUTE)
                    .collect();
                if !attrs.is_empty() {
                    lines.push("---".into());
                    for c in attrs {
                        lines.push(
                            format!(
                                "{} {} {} {}",
                                c.fields[1],
                                c.fields[2],
                                c.fields[3],
                                if c.fields[4].is_empty() {
                                    String::new()
                                } else {
                                    format!("\"{}\"", c.fields[4])
                                }
                            )
                            .trim()
                            .into(),
                        );
                    }
                }
            }
            _ => {}
        }
        node(
            g,
            id,
            lines.join("\n"),
            if d.family == 5 {
                NodeShape::RoundRect
            } else {
                NodeShape::Rectangle
            },
        );
    }
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            ASSOCIATION => {
                let mut e = edge(&f[0], &f[1], &f[5]);
                e.start_label = (!f[3].is_empty()).then(|| f[3].clone());
                e.end_label = (!f[4].is_empty()).then(|| f[4].clone());
                e.arrow_end_kind = Some(EdgeArrowhead::ClassDependency);
                match f[2].as_str() {
                    "inheritance" | "realization" => {
                        e.arrow_end_kind = Some(EdgeArrowhead::OpenTriangle)
                    }
                    "composition" => {
                        e.arrow_end = false;
                        e.start_decoration = Some(EdgeDecoration::DiamondFilled);
                    }
                    "aggregation" => {
                        e.arrow_end = false;
                        e.start_decoration = Some(EdgeDecoration::Diamond);
                    }
                    "link" => e.arrow_end = false,
                    _ => {}
                }
                if ["dependency", "realization"].contains(&f[2].as_str()) {
                    e.style = EdgeStyle::Dotted;
                }
                g.edges.push(e);
            }
            REQUIREMENT_RELATION => {
                let mut e = edge(&f[0], &f[1], &f[2]);
                e.arrow_start = f[2] == "contains";
                e.arrow_end = !e.arrow_start;
                g.edges.push(e);
            }
            ENTITY_RELATION => {
                let mut e = edge(&f[0], &f[1], &f[5]);
                let card = |s: &str| {
                    Some(match s {
                        "one" => EdgeDecoration::CrowsFootOne,
                        "zero-one" => EdgeDecoration::CrowsFootZeroOne,
                        "many" => EdgeDecoration::CrowsFootMany,
                        _ => EdgeDecoration::CrowsFootZeroMany,
                    })
                };
                e.directed = false;
                e.arrow_end = false;
                e.start_decoration = card(&f[2]);
                e.end_decoration = card(&f[3]);
                if f[4] == "false" {
                    e.style = EdgeStyle::Dotted;
                }
                g.edges.push(e);
            }
            _ => {}
        }
    }
}
