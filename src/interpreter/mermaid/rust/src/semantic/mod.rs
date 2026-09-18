//! Strict profiles produce domain records; no source crosses the layout boundary.
mod architecture;
mod block;
mod c4;
mod requirements;
mod state;
mod types;
use xfmd_diagram_contracts::{
    Model,
    semantic::{DIRECTION, Diagram},
};
pub fn parse(source: &str, header: &str) -> Result<Model, String> {
    if source.len() > 65536 || source.contains("%%{") {
        return Err("Semantic diagram source limit/directive".into());
    }
    let family = match header {
        "stateDiagram-v2" => 2,
        "classDiagram" => 3,
        "requirementDiagram" => 4,
        "erDiagram" => 5,
        "C4Context" | "C4Container" | "C4Component" => 6,
        "architecture-beta" => 7,
        "block-beta" => 8,
        _ => return Err(format!("Unsupported diagram family: {header}")),
    };
    let mut d = Diagram {
        family,
        records: vec![],
    };
    let lines: Vec<_> = source
        .lines()
        .map(str::trim)
        .filter(|s| !s.is_empty() && !s.starts_with("%%"))
        .skip(1)
        .collect();
    match family {
        2 => state::parse(&lines, &mut d)?,
        3 | 5 => types::parse(&lines, &mut d)?,
        4 => requirements::parse(&lines, &mut d)?,
        6 => c4::parse(&lines, &mut d, header)?,
        7 => architecture::parse(&lines, &mut d)?,
        8 => block::parse(&lines, &mut d)?,
        _ => unreachable!(),
    }
    d.validate()?;
    Ok(Model {
        semantic: Some(d),
        sequence: None,
        direction: 0,
        nodes: vec![],
        edges: vec![],
        groups: vec![],
    })
}
pub(super) fn id(s: &str) -> Result<&str, String> {
    if !s.is_empty()
        && !s.starts_with("__")
        && s.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_')
    {
        Ok(s)
    } else {
        Err(format!("Unsupported identifier: {s}"))
    }
}
pub(super) fn text(s: &str) -> Result<String, String> {
    let s = s.trim();
    let s = if s.starts_with('"') {
        s.strip_prefix('"')
            .and_then(|v| v.strip_suffix('"'))
            .ok_or("Unbalanced text quotes")?
    } else {
        s
    };
    if s.contains(['<', '>', '`', '&', ';', '\"']) || s.len() > 4096 {
        return Err("Only plain diagram text is supported".into());
    }
    Ok(s.into())
}
pub(super) fn direction(line: &str, d: &mut Diagram) -> Result<bool, String> {
    if let Some(dir) = line.strip_prefix("direction ") {
        if d.records.iter().any(|r| r.tag == DIRECTION) {
            return Err("Duplicate direction".into());
        }
        d.push(DIRECTION, &[dir]);
        Ok(true)
    } else {
        Ok(false)
    }
}
// Whitespace tokens with quotes retained; consumes every byte and rejects broken quotes.
pub(super) fn tokens(s: &str) -> Result<Vec<String>, String> {
    let mut out = vec![];
    let mut current = String::new();
    let mut quoted = false;
    for c in s.chars() {
        if c == '"' {
            quoted = !quoted;
            current.push(c);
        } else if c.is_whitespace() && !quoted {
            if !current.is_empty() {
                out.push(std::mem::take(&mut current));
            }
        } else {
            current.push(c);
        }
    }
    if quoted {
        return Err("Unclosed quoted token".into());
    }
    if !current.is_empty() {
        out.push(current);
    }
    Ok(out)
}
pub(super) fn unsupported(s: &str) -> String {
    format!("Unsupported Mermaid statement: {s}")
}

#[cfg(test)]
mod tests {
    use super::*;
    use xfmd_diagram_contracts::wire::{Reader, Writer};
    #[test]
    fn domain_fixtures_roundtrip() {
        for source in [
            include_str!("../../../../../../docs/design/mermaid/measurement-state.mmd"),
            include_str!("../../../../../../docs/design/mermaid/state-regions.mmd"),
            include_str!("../../../../../../docs/design/mermaid/state-choice.mmd"),
            include_str!("../../../../../../docs/design/mermaid/sdl-class.mmd"),
            include_str!("../../../../../../docs/design/mermaid/apt-requirements.mmd"),
            include_str!("../../../../../../docs/design/mermaid/provenance-er.mmd"),
        ] {
            let model = parse(source, source.lines().next().unwrap()).unwrap();
            let mut w = Writer::default();
            model.write(&mut w);
            let mut r = Reader::new(&w.0);
            assert_eq!(Model::read(&mut r).unwrap(), model);
            r.finish().unwrap();
        }
    }
    #[test]
    fn unsupported_constructs_are_not_silently_dropped() {
        for source in [
            "stateDiagram-v2\nstate X {\nX-->Y",
            "stateDiagram-v2\nA-->B\nstyle A fill:red",
            "classDiagram\nclass A\nclick A callback()",
            "requirementDiagram\nelement E {\nunknown: value\n}",
            "erDiagram\nA ||--?? B : x",
            "classDiagram\nA --|> B trailing",
            "requirementDiagram\nA - verifies -> B",
        ] {
            assert!(
                parse(source, source.lines().next().unwrap()).is_err(),
                "{source}"
            );
        }
    }
    #[test]
    fn preserves_domain_meaning() {
        let source = include_str!("../../../../../../docs/design/mermaid/sdl-class.mmd");
        let model = parse(source, "classDiagram").unwrap();
        let d = model.semantic.unwrap();
        let owner = d
            .records
            .iter()
            .find(|r| r.tag == 24 && r.fields[2] == "composition")
            .unwrap();
        assert_eq!(
            &owner.fields[..5],
            &["Unit", "Functionality", "composition", "1", "0..*"]
        );
        let source = include_str!("../../../../../../docs/design/mermaid/state-regions.mmd");
        let d = parse(source, "stateDiagram-v2").unwrap().semantic.unwrap();
        assert_eq!(d.records.iter().filter(|r| r.tag == 12).count(), 2);
    }
}

#[cfg(test)]
mod architecture_tests;
