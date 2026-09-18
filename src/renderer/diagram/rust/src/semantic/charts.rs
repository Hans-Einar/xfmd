use super::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            90 => match d.family {
                13 => g.pie_title = Some(f[0].clone()),
                17 => g.quadrant.title = Some(f[0].clone()),
                20 => g.radar.title = Some(f[0].clone()),
                22 => g.xychart.title = Some(f[0].clone()),
                _ => {}
            },
            120 => g.pie_slices.push(PieSlice {
                label: f[0].clone(),
                value: f[1].parse().unwrap(),
            }),
            121 => g.pie_show_data = true,
            150 => {
                let mut endpoints = vec![];
                for name in &f[..2] {
                    let existing = g
                        .nodes
                        .values()
                        .find(|n| &n.label == name)
                        .map(|n| n.id.clone());
                    let id = existing.unwrap_or_else(|| {
                        let id = format!("sankey{}", g.nodes.len());
                        node(g, &id, name.clone(), NodeShape::Rectangle);
                        id
                    });
                    endpoints.push(id);
                }
                g.edges.push(edge(&endpoints[0], &endpoints[1], &f[2]));
            }
            160 => g.quadrant.points.push(QuadrantPoint {
                label: f[0].clone(),
                x: f[1].parse().unwrap(),
                y: f[2].parse().unwrap(),
            }),
            161 => {
                if f[0] == "x" {
                    g.quadrant.x_axis_left = Some(f[1].clone());
                    g.quadrant.x_axis_right = Some(f[2].clone());
                } else {
                    g.quadrant.y_axis_bottom = Some(f[1].clone());
                    g.quadrant.y_axis_top = Some(f[2].clone());
                }
            }
            162 => {
                g.quadrant.quadrant_labels[f[0].parse::<usize>().unwrap() - 1] = Some(f[1].clone())
            }
            190 => g.radar.axes.push(f[1].clone()),
            191 => g.radar.curves.push(RadarCurve {
                name: f[1].clone(),
                entries: f[2..]
                    .iter()
                    .map(|v| RadarEntry::Positional(Some(v.parse().unwrap())))
                    .collect(),
            }),
            192 => match f[0].as_str() {
                "min" => g.radar.min = Some(f[1].parse().unwrap()),
                "max" => g.radar.max = Some(f[1].parse().unwrap()),
                _ => {
                    g.radar.graticule = if f[1] == "polygon" {
                        RadarGraticule::Polygon
                    } else {
                        RadarGraticule::Circle
                    }
                }
            },
            200 => g.xychart.x_axis_categories = f.clone(),
            201 => {
                g.xychart.y_axis_label = (!f[0].is_empty()).then(|| f[0].clone());
                g.xychart.y_axis_min = f[1].parse().ok();
                g.xychart.y_axis_max = f[2].parse().ok();
            }
            202 => g.xychart.series.push(XYSeries {
                kind: if f[0] == "bar" {
                    XYSeriesKind::Bar
                } else {
                    XYSeriesKind::Line
                },
                label: None,
                values: f[1..].iter().map(|v| v.parse().unwrap()).collect(),
            }),
            _ => {}
        }
    }
}
