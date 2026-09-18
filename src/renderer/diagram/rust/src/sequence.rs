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
    for e in &s.events {
        let first = &s.participants[e.first as usize].id;
        let second = &s.participants[e.second as usize].id;
        let event = match e.kind {
            Kind::Message | Kind::Reply | Kind::Async | Kind::AsyncReply => {
                let index = graph.edges.len();
                graph.edges.push(Edge {
                    from: first.clone(),
                    to: second.clone(),
                    label: Some(e.text.clone()),
                    start_label: None,
                    end_label: None,
                    directed: true,
                    arrow_start: false,
                    arrow_end: true,
                    arrow_start_kind: None,
                    arrow_end_kind: if matches!(e.kind, Kind::Async | Kind::AsyncReply) {
                        Some(EdgeArrowhead::OpenV)
                    } else {
                        None
                    },
                    start_decoration: None,
                    end_decoration: None,
                    style: if matches!(e.kind, Kind::Reply | Kind::AsyncReply) {
                        EdgeStyle::Dotted
                    } else {
                        EdgeStyle::Solid
                    },
                });
                SequenceEvent::Message(index)
            }
            Kind::NoteLeft | Kind::NoteRight | Kind::NoteOver => {
                let index = graph.sequence_notes.len();
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
                    index: graph.edges.len(),
                });
                SequenceEvent::Note(index)
            }
            Kind::Activate => SequenceEvent::Activate(first.clone()),
            Kind::Deactivate => SequenceEvent::Deactivate(first.clone()),
            Kind::Alt | Kind::Opt | Kind::Loop | Kind::Par => SequenceEvent::Start(
                match e.kind {
                    Kind::Alt => SequenceFrameKind::Alt,
                    Kind::Opt => SequenceFrameKind::Opt,
                    Kind::Loop => SequenceFrameKind::Loop,
                    _ => SequenceFrameKind::Par,
                },
                e.text.clone(),
            ),
            Kind::Else | Kind::And => SequenceEvent::Branch(e.text.clone()),
            Kind::End => SequenceEvent::End,
        };
        graph.sequence_events.push(event);
    }
    graph
}
