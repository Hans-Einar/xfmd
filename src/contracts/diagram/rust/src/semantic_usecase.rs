//! Native use-case records; source parsing and dependency IR remain outside contracts.
use super::{DIRECTION, Diagram};
use std::collections::BTreeMap;
pub const ELEMENT: u32 = 210;
pub const BOUNDARY: u32 = 211;
pub const RELATION: u32 = 212;
pub fn validate(d: &Diagram) -> Result<(), String> {
    if d.family != 23 || d.records.is_empty() || d.records.len() > 512 {
        return Err("Use-case family/record limit".into());
    }
    let mut ids = BTreeMap::new();
    let mut direction = false;
    for r in &d.records {
        let f = &r.fields;
        let count = match r.tag {
            DIRECTION => 1,
            ELEMENT => 5,
            BOUNDARY => 2,
            RELATION => 5,
            _ => return Err("Unknown use-case record".into()),
        };
        if f.len() != count
            || f.iter().any(|s| {
                s.len() > 4096
                    || s.trim() == "---"
                    || s.chars().any(char::is_control)
                    || s.contains(['<', '>', '&', '`', ';', '"'])
            })
        {
            return Err("Invalid use-case fields/plain text".into());
        }
        if matches!(r.tag, ELEMENT | BOUNDARY) {
            if f[0].is_empty()
                || f[0].starts_with("__")
                || !f[0].bytes().all(|c| c.is_ascii_alphanumeric() || c == b'_')
                || ids.insert(f[0].as_str(), r).is_some()
            {
                return Err("Duplicate/invalid use-case identity".into());
            }
        }
        if r.tag == DIRECTION {
            if direction || !["LR", "RL", "TB", "TD", "BT"].contains(&f[0].as_str()) {
                return Err("Invalid use-case direction".into());
            }
            direction = true;
        }
        if r.tag == ELEMENT
            && (!["actor", "usecase"].contains(&f[2].as_str())
                || !["ellipse", "rectangle"].contains(&f[3].as_str())
                || (f[2] == "actor" && f[3] != "ellipse"))
        {
            return Err("Invalid use-case element kind/shape".into());
        }
    }
    if ids.len() > 128 || !d.records.iter().any(|r| r.tag == ELEMENT) {
        return Err("Use-case identity/empty diagram limit".into());
    }
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            ELEMENT if !f[4].is_empty() => {
                if ids.get(f[4].as_str()).is_none_or(|p| p.tag != BOUNDARY) {
                    return Err("Unknown use-case boundary".into());
                }
            }
            BOUNDARY => {
                if !d
                    .records
                    .iter()
                    .any(|c| c.tag == ELEMENT && c.fields[4] == f[0])
                {
                    return Err("Empty use-case boundary".into());
                }
            }
            RELATION => {
                let a = ids
                    .get(f[0].as_str())
                    .filter(|r| r.tag == ELEMENT)
                    .ok_or("Unknown use-case relation source")?;
                let b = ids
                    .get(f[1].as_str())
                    .filter(|r| r.tag == ELEMENT)
                    .ok_or("Unknown use-case relation target")?;
                if !["none", "forward", "reverse"].contains(&f[3].as_str()) {
                    return Err("Invalid use-case relation fields".into());
                }
                match f[2].as_str() {
                    "association" => {}
                    "include" | "extend"
                        if a.fields[2] == "usecase"
                            && b.fields[2] == "usecase"
                            && f[3] == "forward"
                            && f[4].is_empty() => {}
                    "generalization"
                        if a.fields[2] == b.fields[2] && f[3] == "forward" && f[4].is_empty() => {}
                    _ => return Err("Invalid use-case relationship/endpoints".into()),
                }
            }
            _ => {}
        }
    }
    Ok(())
}
