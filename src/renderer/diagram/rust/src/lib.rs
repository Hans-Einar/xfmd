mod model;
use std::collections::HashMap;
use xfmd_diagram_contracts::{
    Model,
    wire::{Reader, Writer},
};
pub fn layout(input: &[u8]) -> Result<Vec<u8>, String> {
    let mut r = Reader::new(input);
    let model = Model::read(&mut r)?;
    let mut labels = HashMap::new();
    for _ in 0..r.count(1024)? {
        let text = r.text()?;
        let width = r.number()?;
        let height = r.number()?;
        if width < 0.0 || height <= 0.0 {
            return Err("Invalid label metrics".into());
        }
        let lines = text.split('\n').map(str::to_owned).collect();
        labels.insert(
            text,
            mermaid_rs_renderer::layout::TextBlock {
                lines,
                width: width as f32,
                height: height as f32,
            },
        );
    }
    r.finish()?;
    let graph = model::graph(&model);
    let mut theme = mermaid_rs_renderer::Theme::modern();
    theme.font_size = 16.0;
    let layout = mermaid_rs_renderer::layout::measurements::layout(
        &graph,
        &theme,
        &mermaid_rs_renderer::LayoutConfig::default(),
        labels,
    );
    let mut w = Writer::default();
    w.number(layout.width as f64);
    w.number(layout.height as f64);
    w.u32(model.nodes.len() as u32);
    for node in &model.nodes {
        let n = layout.nodes.get(&node.id).ok_or("Layout lost node")?;
        w.number(n.x as f64);
        w.number(n.y as f64);
        w.number(n.width as f64);
        w.number(n.height as f64);
    }
    w.u32(layout.subgraphs.len() as u32);
    for g in &layout.subgraphs {
        w.text(g.id.as_deref().unwrap_or(""));
        w.number(g.x as f64);
        w.number(g.y as f64);
        w.number(g.width as f64);
        w.number(g.height as f64);
        w.number(g.label_block.height as f64);
    }
    w.u32(layout.edges.len() as u32);
    for e in &layout.edges {
        w.u32(e.points.len() as u32);
        for p in &e.points {
            w.number(p.0 as f64);
            w.number(p.1 as f64)
        }
        let anchor = e.label_anchor.unwrap_or_else(|| {
            e.points
                .get(e.points.len() / 2)
                .copied()
                .unwrap_or((0.0, 0.0))
        });
        w.number(anchor.0 as f64);
        w.number(anchor.1 as f64);
    }
    if w.0.len() > 8 * 1024 * 1024 {
        return Err("Diagram scene limit exceeded".into());
    }
    Ok(w.0)
}
