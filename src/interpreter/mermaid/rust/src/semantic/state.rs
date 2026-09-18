use super::{direction, id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
fn ensure(
    d: &mut Diagram,
    name: &str,
    label: &str,
    parent: &str,
    role: &str,
    region: &str,
) -> Result<(), String> {
    if let Some(r) = d
        .records
        .iter_mut()
        .find(|r| r.tag == STATE && r.fields[0] == name)
    {
        if !label.is_empty() {
            r.fields[1] = label.into();
            r.fields[2] = parent.into();
            r.fields[4] = region.into();
        }
        if role != "normal" {
            r.fields[3] = role.into();
        }
        // A reference to an existing state does not move it into another scope.
    } else {
        d.push(
            STATE,
            &[
                name,
                if label.is_empty() { name } else { label },
                parent,
                role,
                region,
            ],
        );
    }
    Ok(())
}
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut scope: Vec<(String, String)> = vec![];
    let mut serial = 0;
    for line in lines {
        if direction(line, d)? {
            if !scope.is_empty() {
                return Err("Local state direction is not supported".into());
            }
            continue;
        }
        if *line == "}" {
            scope.pop().ok_or("Unexpected state end")?;
            continue;
        }
        if *line == "--" {
            let current = scope
                .last_mut()
                .ok_or("Concurrent region requires composite state")?;
            serial += 1;
            current.1 = format!("__xfmdRegion{serial}");
            d.push(REGION, &[&current.1, &current.0]);
            continue;
        }
        let (parent, region) = scope.last().cloned().unwrap_or_default();
        if let Some(decl) = line.strip_prefix("state ") {
            let composite = decl.ends_with('{');
            let decl = decl.trim_end_matches('{').trim();
            let (decl, role) = if let Some((s, k)) = decl.split_once(" <<") {
                let role = k.strip_suffix(">>").ok_or("Invalid state stereotype")?;
                if !["choice", "fork", "join"].contains(&role) {
                    return Err(unsupported(line));
                }
                (s, role)
            } else {
                (decl, if composite { "composite" } else { "normal" })
            };
            let (name, label) = if let Some((label, name)) = decl.split_once(" as ") {
                (id(name)?, text(label)?)
            } else {
                (id(decl)?, decl.to_string())
            };
            ensure(d, name, &label, &parent, role, &region)?;
            if composite {
                if scope.len() >= 8 {
                    return Err("State nesting exceeds eight levels".into());
                }
                serial += 1;
                let region = format!("__xfmdRegion{serial}");
                d.push(REGION, &[&region, name]);
                scope.push((name.into(), region));
            }
            continue;
        }
        if let Some((from, rest)) = line.split_once("-->") {
            let (to, label) = rest.split_once(':').unwrap_or((rest, ""));
            let label = text(label)?;
            let mut ends = vec![];
            for (token, role) in [(from.trim(), "initial"), (to.trim(), "final")] {
                let name = if token == "[*]" {
                    format!("__xfmd{}_{}_{}", parent, region, role)
                } else {
                    id(token)?.into()
                };
                ensure(
                    d,
                    &name,
                    if token == "[*]" { " " } else { "" },
                    &parent,
                    if token == "[*]" { role } else { "normal" },
                    &region,
                )?;
                ends.push(name);
            }
            d.push(TRANSITION, &[&ends[0], &ends[1], &label]);
            continue;
        }
        if let Some((name, label)) = line.split_once(':') {
            ensure(
                d,
                id(name.trim())?,
                &text(label)?,
                &parent,
                "normal",
                &region,
            )?;
            continue;
        }
        if id(line).is_ok() {
            ensure(d, line, line, &parent, "normal", &region)?;
            continue;
        }
        return Err(unsupported(line));
    }
    if !scope.is_empty() {
        return Err("Unclosed composite state".into());
    }
    Ok(())
}
