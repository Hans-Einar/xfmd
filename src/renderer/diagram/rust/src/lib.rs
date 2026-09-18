mod model;
mod semantic;
mod sequence;
pub mod text_metrics;
use std::collections::HashMap;
use xfmd_diagram_contracts::{
    Model,
    wire::{Reader, Writer},
};
pub fn layout(input: &[u8]) -> Result<Vec<u8>, String> {
    layout_measured(input, None)
}
pub fn layout_measured(
    input: &[u8],
    metrics: Option<text_metrics::Metrics>,
) -> Result<Vec<u8>, String> {
    let mut r = Reader::new(input);
    if r.count(5)? != 5 {
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
    let graph = if let Some(d) = &model.semantic {
        semantic::graph(d)?
    } else if let Some(s) = &model.sequence {
        sequence::graph(s)
    } else {
        model::graph(&model)
    };
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
    let mut config = mermaid_rs_renderer::LayoutConfig::default();
    if model.semantic.is_some() {
        config.node_spacing = 70.;
        config.rank_spacing = 70.;
        config.max_label_width_chars = 20;
        config.flowchart.auto_spacing.enabled = false;
        if graph.subgraphs.is_empty() && model.semantic.as_ref().is_some_and(|d| d.family != 2) {
            config.flowchart.engine = mermaid_rs_renderer::config::FlowchartLayoutEngine::Dagre;
        } else {
            config.node_spacing = 50.;
            config.rank_spacing = 50.;
        }
        config.requirement.fill = theme.primary_color.clone();
        config.requirement.box_stroke = theme.primary_border_color.clone();
        config.requirement.stroke = theme.primary_border_color.clone();
        config.requirement.label_color = theme.primary_text_color.clone();
        config.requirement.divider_color = theme.primary_border_color.clone();
        config.requirement.edge_stroke = theme.line_color.clone();
        config.requirement.edge_label_color = theme.primary_text_color.clone();
        config.requirement.edge_label_background = theme.edge_label_background.clone();
    }
    let duration = std::time::Duration::from_millis(budget as u64);
    let deadline = std::time::Instant::now() + duration;
    let (layout, mut diagnostics) = if let Some(d) = &model.semantic {
        let metrics = metrics.ok_or("Semantic diagrams require external text metrics")?;
        let metrics = text_metrics::Context {
            metrics,
            no_wrap: if [3, 5].contains(&d.family) {
                graph.nodes.values().map(|n| n.label.clone()).collect()
            } else {
                Default::default()
            },
        };
        let layout = measurements::with_measurer(
            &metrics as *const _ as usize,
            text_metrics::measure,
            duration,
            || {
                mermaid_rs_renderer::layout::compute_semantic_layout(
                    &graph,
                    &theme,
                    &config,
                    &RoutingControl {
                        deadline,
                        cancelled: &|| false,
                    },
                )
            },
        );
        let (layout, details) = layout.map_err(|e| format!("Semantic layout: {e}"))?;
        (
            layout,
            format!(
                "Semantic profile {}; {}; cooperative deadline",
                d.family,
                details.join("; ")
            ),
        )
    } else if model.sequence.is_some() {
        let layout = measurements::layout(
            &graph,
            &theme,
            &mermaid_rs_renderer::LayoutConfig::default(),
            labels,
            duration,
        );
        (
            layout,
            "Sequence 2; dedicated sequence layout; cooperative deadline".into(),
        )
    } else {
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
        (routed.layout, diagnostics)
    };
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
    let inspection_groups = if model.semantic.is_some() {
        &[][..]
    } else {
        &layout.subgraphs[..]
    };
    w.u32(inspection_groups.len() as u32);
    for g in inspection_groups {
        w.text(g.id.as_deref().unwrap_or(""));
        w.number(g.x as f64);
        w.number(g.y as f64);
        w.number(g.width as f64);
        w.number(g.height as f64);
        w.number(g.label_block.height as f64);
    }
    let inspection_edges = if model.sequence.is_some() || model.semantic.is_some() {
        &[][..]
    } else {
        &layout.edges[..]
    };
    w.u32(inspection_edges.len() as u32);
    for e in inspection_edges {
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
    let svg = if jumps && model.sequence.is_none() && model.semantic.is_none() {
        mermaid_rs_renderer::render::render_svg_with_crossings(
            &layout,
            &theme,
            &config,
            mermaid_rs_renderer::render::CrossingJumps::default(),
        )
    } else {
        mermaid_rs_renderer::render_svg(&layout, &theme, &config)
    };
    let (svg, omitted) = if model.semantic.is_some() {
        (svg, 0)
    } else {
        mermaid_rs_renderer::render::add_label_leaders(svg, &layout, &theme, &config)
    };
    diagnostics.push_str(&format!("; label leaders omitted: {omitted}"));
    if std::time::Instant::now() >= deadline {
        return Err("Diagram layout time budget exceeded".into());
    }
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
