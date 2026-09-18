use super::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    let mut b = BlockDiagram {
        columns: Some(1),
        nodes: vec![],
    };
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            70 => b.columns = Some(f[0].parse().unwrap()),
            71 | 72 => {
                let space = r.tag == 72;
                node(
                    g,
                    &f[0],
                    if space { String::new() } else { f[1].clone() },
                    NodeShape::Rectangle,
                );
                b.nodes.push(BlockNode {
                    id: f[0].clone(),
                    span: f[if space { 1 } else { 2 }].parse().unwrap(),
                    is_space: space,
                });
            }
            73 => {
                let mut e = edge(&f[0], &f[1], &f[2]);
                e.arrow_end = f[3] == "2";
                e.directed = e.arrow_end;
                g.edges.push(e);
            }
            _ => {}
        }
    }
    g.block = Some(b);
}
