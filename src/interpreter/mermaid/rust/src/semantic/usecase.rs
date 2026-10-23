//! Strict producer-friendly subset of Mermaid usecase-beta.
use super::{direction, id, text, tokens, unsupported};
use xfmd_diagram_contracts::semantic::{Diagram, usecase::*};

fn declaration(s: &str) -> Result<(String, String, &'static str), String> {
    let end = s.find(['(', '[']).unwrap_or(s.len());
    let name = id(s[..end].trim())?.to_string();
    if end == s.len() {
        return Ok((name.clone(), name, "ellipse"));
    }
    let (close, shape) = if s.as_bytes()[end] == b'(' {
        (')', "ellipse")
    } else {
        (']', "rectangle")
    };
    let label = s[end + 1..]
        .strip_suffix(close)
        .ok_or("Unclosed use-case declaration")?;
    if !label.starts_with('"') && label.contains(['(', ')', '[', ']', '{', '}']) {
        return Err(unsupported(s));
    }
    Ok((name, text(label)?, shape))
}

pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut boundary = String::new();
    for line in lines {
        if *line == "end" {
            if boundary.is_empty() {
                return Err("Unexpected use-case end".into());
            }
            boundary.clear();
            continue;
        }
        if let Some(rest) = line.strip_prefix("systemBoundary ") {
            if !boundary.is_empty() {
                return Err("Nested use-case boundaries are unsupported".into());
            }
            let (name, label, _) = declaration(rest)?;
            d.push(BOUNDARY, &[&name, &label]);
            boundary = name;
            continue;
        }
        if line.starts_with("direction ") {
            if !boundary.is_empty() {
                return Err("Direction inside use-case boundary".into());
            }
            direction(line, d)?;
            continue;
        }
        if let Some(rest) = line.strip_prefix("actor ") {
            let (name, label, shape) = declaration(rest)?;
            if shape != "ellipse" {
                return Err("Actor label requires parentheses".into());
            }
            d.push(ELEMENT, &[&name, &label, "actor", "ellipse", &boundary]);
            continue;
        }
        // Declarations are recognized before operators in quoted labels.
        let prefix = line
            .find(|c: char| !c.is_ascii_alphanumeric() && c != '_')
            .unwrap_or(line.len());
        if prefix == line.len() || line[prefix..].starts_with(['(', '[']) {
            let (name, label, shape) = declaration(line)?;
            d.push(ELEMENT, &[&name, &label, "usecase", shape, &boundary]);
            continue;
        }
        if !boundary.is_empty() {
            return Err("Only declarations are allowed inside use-case boundaries".into());
        }
        let t = tokens(line)?;
        let (from, to, kind, arrow, label) = match t.as_slice() {
            [a, op, b] if ["--", "-->", "<--", "--|>"].contains(&op.as_str()) => (
                a,
                b,
                if op == "--|>" {
                    "generalization"
                } else {
                    "association"
                },
                if op == "--" {
                    "none"
                } else if op == "<--" {
                    "reverse"
                } else {
                    "forward"
                },
                String::new(),
            ),
            [a, op, colon, kind, b]
                if op == "..>"
                    && colon == ":"
                    && ["include", "extend"].contains(&kind.as_str()) =>
            {
                (a, b, kind.as_str(), "forward", String::new())
            }
            [a, op, label, end, b] if op == "--" && ["--", "-->"].contains(&end.as_str()) => (
                a,
                b,
                "association",
                if end == "--" { "none" } else { "forward" },
                text(label)?,
            ),
            _ => return Err(unsupported(line)),
        };
        d.push(RELATION, &[id(from)?, id(to)?, kind, arrow, &label]);
    }
    if !boundary.is_empty() {
        return Err("Unclosed use-case boundary".into());
    }
    let references: Vec<_> = d
        .records
        .iter()
        .filter(|r| r.tag == RELATION)
        .flat_map(|r| r.fields[..2].iter().cloned())
        .collect();
    for name in references {
        if !d
            .records
            .iter()
            .any(|r| matches!(r.tag, ELEMENT | BOUNDARY) && r.fields[0] == name)
        {
            d.push(ELEMENT, &[&name, &name, "usecase", "ellipse", ""]);
        }
    }
    Ok(())
}
