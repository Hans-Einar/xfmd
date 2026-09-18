use super::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    let mut sections = std::collections::BTreeMap::new();
    g.direction = Direction::LeftRight;
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            80 => g.packet_fields.push(PacketField {
                start: f[0].parse().unwrap(),
                end: f[1].parse().unwrap(),
                label: f[2].clone(),
            }),
            90 => match d.family {
                10 => g.timeline.title = Some(f[0].clone()),
                11 => g.gantt_title = Some(f[0].clone()),
                12 => g.journey_title = Some(f[0].clone()),
                _ => {}
            },
            91 => {
                sections.insert(f[0].clone(), f[1].clone());
                match d.family {
                    10 => g.timeline.sections.push(f[1].clone()),
                    11 => g.gantt_sections.push(f[1].clone()),
                    12 => g.subgraphs.push(Subgraph {
                        id: Some(f[0].clone()),
                        label: f[1].clone(),
                        nodes: vec![],
                        direction: None,
                        icon: None,
                    }),
                    _ => {}
                }
            }
            92 => g.timeline.events.push(TimelineEvent {
                time: f[1].clone(),
                events: f[3..].to_vec(),
                section: sections.get(&f[2]).cloned(),
            }),
            100 => g.gantt_tasks.push(GanttTask {
                id: f[0].clone(),
                label: f[1].clone(),
                start: (!f[2].is_empty()).then(|| f[2].clone()),
                duration: Some(format!("{}d", f[3])),
                after: (!f[4].is_empty()).then(|| f[4].clone()),
                section: sections.get(&f[5]).cloned(),
                status: match f[6].as_str() {
                    "done" => Some(GanttStatus::Done),
                    "active" => Some(GanttStatus::Active),
                    "crit" => Some(GanttStatus::Crit),
                    _ => None,
                },
            }),
            110 => {
                node(
                    g,
                    &f[0],
                    format!("{}\n{}", f[1], f[4].replace(',', ", ")),
                    NodeShape::Rectangle,
                );
                g.nodes.get_mut(&f[0]).unwrap().value = Some(f[2].parse().unwrap());
                if let Some(group) = g
                    .subgraphs
                    .iter_mut()
                    .find(|s| s.id.as_ref() == Some(&f[3]))
                {
                    group.nodes.push(f[0].clone());
                }
            }
            _ => {}
        }
    }
}
