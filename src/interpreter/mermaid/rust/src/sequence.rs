//! Full-consumption Sequence 2 profile. Never send unrecognized lines to the
//! permissive dependency parser: it silently drops several Mermaid constructs.
use xfmd_diagram_contracts::{
    Model,
    sequence::{Event, Kind, Participant, Sequence},
};
fn identifier(s: &str) -> bool {
    !s.is_empty() && s.bytes().all(|c| c.is_ascii_alphanumeric() || c == b'_')
}
pub fn parse(source: &str) -> Result<Model, String> {
    if source.len() > 65536 {
        return Err("Diagram exceeds 64 KiB".into());
    }
    let mut sequence = Sequence::default();
    let mut lines = source
        .lines()
        .map(str::trim)
        .filter(|s| !s.is_empty() && !s.starts_with("%%"));
    if source.contains("%%{") || lines.next() != Some("sequenceDiagram") {
        return Err("Unsupported sequence header/directive".into());
    }
    for line in lines {
        if line.contains(['<', '>', '{', '}', ';', '`', '&']) && !line.contains("->>") {
            return Err("Sequence 2: HTML, directives and encoded labels are not supported".into());
        }
        if let Some(rest) = line
            .strip_prefix("participant ")
            .or_else(|| line.strip_prefix("actor "))
        {
            if !sequence.events.is_empty() {
                return Err("Declare sequence participants before events".into());
            }
            let (id, label) = rest.split_once(" as ").unwrap_or((rest, rest));
            if !identifier(id)
                || label.is_empty()
                || label.contains(['<', '>', '{', '}', ';', '`', '&'])
            {
                return Err("Unsupported participant declaration".into());
            }
            sequence.participants.push(Participant {
                id: id.into(),
                label: label.into(),
                actor: line.starts_with("actor "),
            });
            continue;
        }
        let index = |id: &str| {
            sequence
                .participants
                .iter()
                .position(|p| p.id == id.trim())
                .map(|n| n as u32)
                .ok_or_else(|| format!("Sequence 2: declare participant {} first", id.trim()))
        };
        let mut event = Event {
            kind: Kind::End,
            first: 0,
            second: 0,
            text: String::new(),
        };
        if let Some((ends, label)) = line
            .split_once(':')
            .filter(|(s, _)| s.contains("->>") || s.contains("-)"))
        {
            let (arrow, kind) = if ends.contains("--)") {
                ("--)", Kind::AsyncReply)
            } else if ends.contains("-)") {
                ("-)", Kind::Async)
            } else if ends.contains("-->>") {
                ("-->>", Kind::Reply)
            } else {
                ("->>", Kind::Message)
            };
            let (from, to) = ends.split_once(arrow).ok_or("Invalid message")?;
            event = Event {
                kind,
                first: index(from)?,
                second: index(to)?,
                text: label.trim().into(),
            };
        } else if let Some(note) = line
            .strip_prefix("Note ")
            .or_else(|| line.strip_prefix("note "))
        {
            let (position, label) = note.split_once(':').ok_or("Missing note text")?;
            let (kind, ids) = if let Some(ids) = position.strip_prefix("left of ") {
                (Kind::NoteLeft, ids)
            } else if let Some(ids) = position.strip_prefix("right of ") {
                (Kind::NoteRight, ids)
            } else if let Some(ids) = position.strip_prefix("over ") {
                (Kind::NoteOver, ids)
            } else {
                return Err("Unsupported note position".into());
            };
            let (first, second) = ids.split_once(',').unwrap_or((ids, ids));
            if kind != Kind::NoteOver && first != second {
                return Err("Side notes need one participant".into());
            }
            event = Event {
                kind,
                first: index(first)?,
                second: index(second)?,
                text: label.trim().into(),
            };
        } else {
            let (keyword, rest) = line.split_once(' ').unwrap_or((line, ""));
            event.kind = match keyword {
                "activate" => Kind::Activate,
                "deactivate" => Kind::Deactivate,
                "alt" => Kind::Alt,
                "opt" => Kind::Opt,
                "loop" => Kind::Loop,
                "par" => Kind::Par,
                "else" => Kind::Else,
                "and" => Kind::And,
                "end" if rest.is_empty() => Kind::End,
                _ => {
                    return Err(format!(
                        "Sequence 2: unsupported statement or arrow: {line}"
                    ));
                }
            };
            if matches!(event.kind, Kind::Activate | Kind::Deactivate) {
                event.first = index(rest)?;
            } else {
                event.text = rest.trim().into();
            }
        }
        if event.text.contains(['<', '>', '{', '}', ';', '`', '&']) {
            return Err("Sequence 2: only plain text labels are supported".into());
        }
        sequence.events.push(event);
    }
    sequence.validate()?;
    // Reuse strict dependency parsing, and verify its message order/identity.
    // The ordered XFMD events retain frame boundaries and note/activation order.
    let graph = mermaid_rs_renderer::parse_mermaid_strict(source)
        .map_err(|e| format!("Mermaid: {e}"))?
        .graph;
    let messages: Vec<_> = sequence
        .events
        .iter()
        .filter(|e| {
            matches!(
                e.kind,
                Kind::Message | Kind::Reply | Kind::Async | Kind::AsyncReply
            )
        })
        .collect();
    if graph.edges.len() != messages.len()
        || graph.sequence_participants.len() != sequence.participants.len()
    {
        return Err("Sequence parser lost participants or messages".into());
    }
    for p in &sequence.participants {
        let node = graph.nodes.get(&p.id).ok_or("Lost sequence participant")?;
        if node.label != p.label {
            return Err("Sequence parser changed participant label".into());
        }
    }
    for (edge, event) in graph.edges.iter().zip(messages) {
        if edge.from != sequence.participants[event.first as usize].id
            || edge.to != sequence.participants[event.second as usize].id
            || edge.label.as_deref().unwrap_or("") != event.text
        {
            return Err("Sequence parser changed message order/text".into());
        }
    }
    Ok(Model {
        sequence: Some(sequence),
        semantic: None,
        direction: 0,
        nodes: vec![],
        edges: vec![],
        groups: vec![],
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn apt_order_and_roundtrip() {
        let model = parse(include_str!(
            "../../../../../docs/design/mermaid/apt-import.mmd"
        ))
        .unwrap();
        let sequence = model.sequence.as_ref().unwrap();
        assert!(sequence.participants[0].actor);
        assert_eq!(sequence.participants[0].label, "Operatør");
        assert_eq!(sequence.events[4].kind, Kind::Reply);
        let mut w = xfmd_diagram_contracts::wire::Writer::default();
        model.write(&mut w);
        let mut r = xfmd_diagram_contracts::wire::Reader::new(&w.0);
        assert_eq!(Model::read(&mut r).unwrap(), model);
        r.finish().unwrap();
    }
    #[test]
    fn nested_events_and_async_are_preserved() {
        let m = parse(include_str!(
            "../../../../../docs/design/mermaid/sequence-nested.mmd"
        ))
        .unwrap();
        let s = m.sequence.unwrap();
        assert!(s.events.iter().any(|e| e.kind == Kind::Async));
        assert!(s.events.iter().any(|e| e.kind == Kind::AsyncReply));
        assert_eq!(s.events.iter().filter(|e| e.kind == Kind::End).count(), 5);
    }
    #[test]
    fn reject_lossy_or_ambiguous_constructs() {
        let header = "sequenceDiagram\nparticipant A\nparticipant B\n";
        for body in [
            "A->B: no arrow",
            "A->>+B: inline",
            "A->>C: implicit",
            "autonumber\nA->>B: x",
            "else orphan\nA->>B: x",
            "loop x\nend",
            "activate A\nA->>B: x",
            "A->>B: x\ndeactivate B",
            "A->>B: <b>html</b>",
            "A->>B: x\nunknown statement",
            "alt x\nactivate B\nA->>B: x\nelse y\nB-->>A: y\ndeactivate B\nend",
            "par x\nA->>B: x\nelse y\nB-->>A: y\nend",
        ] {
            assert!(parse(&format!("{header}{body}")).is_err(), "{body}");
        }
    }
    #[test]
    fn ordered_flat_fragments_and_notes() {
        for (kind, body) in [
            (Kind::Opt, "opt maybe\nA->>B: value\nend"),
            (Kind::Loop, "loop retry\nA->>B: value\nend"),
            (
                Kind::Par,
                "par one\nA->>B: value\nand two\nB-->>A: result\nend",
            ),
        ] {
            let m=parse(&format!("sequenceDiagram\nparticipant A\nparticipant B\nNote left of A: Start\nA->>B: Begin\n{body}\nNote right of B: End")).unwrap();
            assert_eq!(m.sequence.unwrap().events[2].kind, kind);
        }
    }
}
