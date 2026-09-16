mod model;
use std::collections::HashMap;
use xfmd_diagram_contracts::{
    Model,
    wire::{Reader, Writer},
};
pub fn layout(input: &[u8]) -> Result<Vec<u8>, String> {
    let mut r = Reader::new(input);
    if r.count(4)? != 4 {
        return Err("Unsupported diagram layout payload".into());
    }
    let model = Model::read(&mut r)?;
    let budget = r.count(10000)?;
    if budget == 0 {
        return Err("Invalid diagram deadline".into());
    }
    let mut labels = HashMap::new();
    for _ in 0..r.count(1024)? {
        let text = r.text()?;
        let width = r.number()?;
        let height = r.number()?;
        if width < 0.0 || height <= 0.0 {
            return Err("Invalid label metrics".into());
        }
        let count = r.count(4096)?;
        if count == 0 {
            return Err("Missing measured label lines".into());
        }
        let mut lines = Vec::with_capacity(count as usize);
        for _ in 0..count {
            lines.push(r.text()?);
        }
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
    theme.font_family = "DejaVu Sans".into();
    use mermaid_rs_renderer::layout::{
        measurements,
        routed::{self, Engine},
    };
    use mermaid_rs_renderer::routing_backend::RoutingControl;
    let engine = match std::env::var("XFMD_MERMAID_ROUTER").as_deref() {
        Err(std::env::VarError::NotPresent) | Ok("libavoid") => Engine::Libavoid,
        Ok("legacy") => Engine::Legacy,
        _ => return Err("XFMD_MERMAID_ROUTER must be libavoid or legacy".into()),
    };
    let jumps = match std::env::var("XFMD_MERMAID_CROSSING_JUMPS").as_deref() {
        Err(std::env::VarError::NotPresent) | Ok("0") => false,
        Ok("1") => true,
        _ => return Err("XFMD_MERMAID_CROSSING_JUMPS must be 0 or 1".into()),
    };
    let duration = std::time::Duration::from_millis(budget as u64);
    let deadline = std::time::Instant::now() + duration;
    let routed = measurements::with_measurements(labels, duration, || {
        routed::compute(
            &graph,
            &theme,
            &mermaid_rs_renderer::LayoutConfig::default(),
            engine,
            &RoutingControl {
                deadline,
                cancelled: &|| false,
            },
        )
    })
    .map_err(|e| format!("{engine:?}: {e}"))?;
    let diagnostics = format!("{engine:?}; {}", routed.diagnostics.join("; "));
    let layout = routed.layout;
    let mut w = Writer::default();
    w.u32(3);
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
    let config = mermaid_rs_renderer::LayoutConfig::default();
    let svg = if jumps {
        mermaid_rs_renderer::render::render_svg_with_crossings(
            &layout,
            &theme,
            &config,
            mermaid_rs_renderer::render::CrossingJumps::default(),
        )
    } else {
        mermaid_rs_renderer::render_svg(&layout, &theme, &config)
    };
    w.text(&svg);
    w.text(&diagnostics);
    if w.0.len() > 8 * 1024 * 1024 {
        return Err("Diagram scene limit exceeded".into());
    }
    Ok(w.0)
}

#[cfg(test)]
mod tests {
    #[test]
    fn deadline_does_not_escape_layout_call() {
        use mermaid_rs_renderer::{LayoutConfig, Theme, ir::Graph, layout::measurements};
        // An expired nested call may return or unwind, depending on its graph.
        // In both cases an unrelated checkpoint must see the original TLS state.
        let _ = std::panic::catch_unwind(|| {
            measurements::layout(
                &Graph::new(),
                &Theme::modern(),
                &LayoutConfig::default(),
                std::collections::HashMap::new(),
                std::time::Duration::ZERO,
            )
        });
        // Cover a complete clock-sampling interval, including the expiry check.
        for _ in 0..64 {
            measurements::checkpoint();
        }
    }
}
