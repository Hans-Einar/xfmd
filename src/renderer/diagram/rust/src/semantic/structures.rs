use super::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    g.gitgraph.main_branch = "main".into();
    if d.family == 15 {
        g.direction = Direction::LeftRight;
    }
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            130 => {
                let shape = match f[3].as_str() {
                    "circle" => NodeShape::Circle,
                    "rounded" => NodeShape::RoundRect,
                    _ => NodeShape::Rectangle,
                };
                node(g, &f[0], f[1].clone(), shape);
                if !f[4].is_empty() {
                    g.nodes.get_mut(&f[0]).unwrap().value = Some(f[4].parse().unwrap());
                }
                if !f[2].is_empty() {
                    let mut e = edge(&f[2], &f[0], "");
                    e.directed = false;
                    e.arrow_end = false;
                    g.edges.push(e);
                }
                if d.family == 14 {
                    let level = if f[2].is_empty() {
                        g.mindmap.root_id = Some(f[0].clone());
                        0
                    } else {
                        let parent = g.mindmap.nodes.iter_mut().find(|n| n.id == f[2]).unwrap();
                        parent.children.push(f[0].clone());
                        parent.level + 1
                    };
                    g.mindmap.nodes.push(MindmapNode {
                        id: f[0].clone(),
                        label: f[1].clone(),
                        level,
                        section: None,
                        node_type: match f[3].as_str() {
                            "circle" => MindmapNodeType::Circle,
                            "rounded" => MindmapNodeType::RoundedRect,
                            "rectangle" => MindmapNodeType::Rect,
                            _ => MindmapNodeType::Default,
                        },
                        icon: None,
                        class: None,
                        children: vec![],
                    });
                }
            }
            140 => g.gitgraph.branches.push(GitGraphBranch {
                name: f[0].clone(),
                order: None,
                insertion_index: g.gitgraph.branches.len(),
            }),
            141 => g.gitgraph.commits.push(GitGraphCommit {
                id: f[0].clone(),
                message: None,
                seq: g.gitgraph.commits.len(),
                commit_type: if f[4] == "merge" {
                    GitGraphCommitType::Merge
                } else {
                    GitGraphCommitType::Normal
                },
                custom_type: None,
                tags: vec![],
                parents: f[2..4].iter().filter(|p| !p.is_empty()).cloned().collect(),
                branch: f[1].clone(),
                custom_id: !f[0].starts_with("__"),
            }),
            170 => {
                node(g, &f[0], f[1].clone(), NodeShape::Rectangle);
                g.sequence_participants.push(f[0].clone());
            }
            171 => {
                let mut e = edge(&f[0], &f[1], &f[2]);
                e.arrow_end_kind = Some(EdgeArrowhead::OpenV);
                g.sequence_events
                    .push(SequenceEvent::Message(g.edges.len()));
                g.edges.push(e);
            }
            180 => g.subgraphs.push(Subgraph {
                id: Some(f[0].clone()),
                label: f[1].clone(),
                nodes: vec![],
                direction: None,
                icon: None,
            }),
            181 => {
                node(g, &f[0], f[1].clone(), NodeShape::Rectangle);
                g.subgraphs
                    .iter_mut()
                    .find(|s| s.id.as_ref() == Some(&f[2]))
                    .unwrap()
                    .nodes
                    .push(f[0].clone());
            }
            _ => {}
        }
    }
}
