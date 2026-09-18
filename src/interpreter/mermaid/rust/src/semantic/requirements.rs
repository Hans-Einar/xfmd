use super::{direction, id, text, tokens, unsupported};
use xfmd_diagram_contracts::semantic::*;
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut active: Option<String> = None;
    for line in lines {
        if let Some(owner) = &active {
            if *line == "}" {
                active = None;
            } else {
                let (key, value) = line
                    .split_once(':')
                    .ok_or("Requirement property requires colon")?;
                let key = key.trim().to_ascii_lowercase();
                if d.records
                    .iter()
                    .any(|r| r.tag == PROPERTY && r.fields[0] == *owner && r.fields[1] == key)
                {
                    return Err("Duplicate requirement property".into());
                }
                d.push(PROPERTY, &[owner, &key, &text(value)?]);
            }
            continue;
        }
        if direction(line, d)? {
            continue;
        }
        if let Some(header) = line.strip_suffix('{') {
            let t = tokens(header)?;
            if t.len() != 2 {
                return Err("Requirement declaration needs type and identity".into());
            }
            id(&t[1])?;
            d.push(REQUIREMENT, &[&t[1], &t[0]]);
            active = Some(t[1].clone());
            continue;
        }
        let t = tokens(line)?;
        if t.len() == 5 && t[1] == "-" && t[3] == "->" {
            d.push(REQUIREMENT_RELATION, &[id(&t[0])?, id(&t[4])?, &t[2]]);
            continue;
        }
        if t.len() == 5 && t[1] == "<-" && t[3] == "-" {
            d.push(REQUIREMENT_RELATION, &[id(&t[4])?, id(&t[0])?, &t[2]]);
            continue;
        }
        return Err(unsupported(line));
    }
    if active.is_some() {
        return Err("Unclosed requirement body".into());
    }
    Ok(())
}
