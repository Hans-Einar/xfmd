//! Map typed interaction values to the dependency's dedicated sequence IR.
use mermaid_rs_renderer::ir::*;
use xfmd_diagram_contracts::sequence::{Kind, Sequence};
pub fn graph(s: &Sequence) -> Graph {
    let mut graph = Graph::new();
    graph.kind = DiagramKind::Sequence;
    for (i, p) in s.participants.iter().enumerate() {
        graph.node_order.insert(p.id.clone(), i);
        graph.nodes.insert(
            p.id.clone(),
            Node {
                id: p.id.clone(),
                value: None,
                icon: None,
                label: p.label.clone(),
                shape: if p.actor {
                    NodeShape::ActorBox
                } else {
                    NodeShape::Rectangle
                },
            },
        );
        graph.sequence_participants.push(p.id.clone());
    }
    let mut frame: Option<SequenceFrame> = None;
    for e in &s.events {
        let first = &s.participants[e.first as usize].id;
        let second = &s.participants[e.second as usize].id;
        let index = graph.edges.len();
        match e.kind {
            Kind::Message | Kind::Reply => graph.edges.push(Edge {
                from: first.clone(),
                to: second.clone(),
                label: Some(e.text.clone()),
                start_label: None,
                end_label: None,
                directed: true,
                arrow_start: false,
                arrow_end: true,
                arrow_start_kind: None,
                arrow_end_kind: None,
                start_decoration: None,
                end_decoration: None,
                style: if e.kind == Kind::Reply {
                    EdgeStyle::Dotted
                } else {
                    EdgeStyle::Solid
                },
            }),
            Kind::NoteLeft | Kind::NoteRight | Kind::NoteOver => {
                graph.sequence_notes.push(SequenceNote {
                    position: match e.kind {
                        Kind::NoteLeft => SequenceNotePosition::LeftOf,
                        Kind::NoteRight => SequenceNotePosition::RightOf,
                        _ => SequenceNotePosition::Over,
                    },
                    participants: if first == second {
                        vec![first.clone()]
                    } else {
                        vec![first.clone(), second.clone()]
                    },
                    label: e.text.clone(),
                    index,
                })
            }
            Kind::Activate | Kind::Deactivate => {
                graph.sequence_activations.push(SequenceActivation {
                    participant: first.clone(),
                    index: index.saturating_sub(1),
                    kind: if e.kind == Kind::Activate {
                        SequenceActivationKind::Activate
                    } else {
                        SequenceActivationKind::Deactivate
                    },
                })
            }
            Kind::Alt | Kind::Opt | Kind::Loop | Kind::Par => {
                frame = Some(SequenceFrame {
                    kind: match e.kind {
                        Kind::Alt => SequenceFrameKind::Alt,
                        Kind::Opt => SequenceFrameKind::Opt,
                        Kind::Loop => SequenceFrameKind::Loop,
                        _ => SequenceFrameKind::Par,
                    },
                    sections: vec![SequenceFrameSection {
                        label: Some(e.text.clone()),
                        start_idx: index,
                        end_idx: index,
                    }],
                    start_idx: index,
                    end_idx: index,
                })
            }
            Kind::Else | Kind::And => {
                let f = frame.as_mut().expect("validated frame");
                f.sections.last_mut().unwrap().end_idx = index;
                f.sections.push(SequenceFrameSection {
                    label: Some(e.text.clone()),
                    start_idx: index,
                    end_idx: index,
                });
            }
            Kind::End => {
                let mut f = frame.take().expect("validated frame");
                f.end_idx = index;
                f.sections.last_mut().unwrap().end_idx = index;
                graph.sequence_frames.push(f);
            }
        }
    }
    graph
}
