use super::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            60 => g.subgraphs.push(Subgraph {
                id: Some(f[0].clone()),
                label: f[1].clone(),
                nodes: vec![],
                direction: None,
                icon: (!f[2].is_empty()).then(|| f[2].clone()),
            }),
            61 | 62 => {
                node(
                    g,
                    &f[0],
                    if r.tag == 62 {
                        String::new()
                    } else {
                        f[1].clone()
                    },
                    NodeShape::Rectangle,
                );
                g.nodes.get_mut(&f[0]).unwrap().icon = if r.tag == 62 {
                    Some("junction".into())
                } else {
                    (!f[2].is_empty()).then(|| f[2].clone())
                };
            }
            63 => {
                let mut e = edge(&f[0], &f[1], "");
                e.arrow_start = matches!(f[4].as_str(), "1" | "3");
                e.arrow_end = matches!(f[4].as_str(), "2" | "3");
                e.directed = e.arrow_start || e.arrow_end;
                g.arch_edge_ports
                    .insert(g.edges.len(), (Some(side(&f[2])), Some(side(&f[3]))));
                g.edges.push(e);
            }
            _ => {}
        }
    }
    for r in &d.records {
        if r.tag == 61 || r.tag == 62 {
            let parent = &r.fields[if r.tag == 61 { 3 } else { 1 }];
            if let Some(group) = g
                .subgraphs
                .iter_mut()
                .find(|s| s.id.as_ref() == Some(parent))
            {
                group.nodes.push(r.fields[0].clone());
            }
        }
    }
}
fn side(s: &str) -> ArchDir {
    match s {
        "L" => ArchDir::Left,
        "R" => ArchDir::Right,
        "T" => ArchDir::Top,
        _ => ArchDir::Bottom,
    }
}
