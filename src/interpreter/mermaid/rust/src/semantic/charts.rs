use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
pub fn named(s: &str) -> Result<(String, String), String> {
    if let Some((name, label)) = s.trim().split_once('[') {
        let label = label.strip_suffix(']').ok_or("Unclosed label")?;
        Ok((id(name.trim())?.into(), text(label)?))
    } else {
        Ok((id(s.trim())?.into(), s.trim().into()))
    }
}
fn values(s: &str, open: char, close: char) -> Result<Vec<String>, String> {
    let inner = s
        .trim()
        .strip_prefix(open)
        .and_then(|v| v.strip_suffix(close))
        .ok_or("Unclosed value list")?;
    inner.split(',').map(text).collect()
}
pub fn parse(lines: &[&str], d: &mut Diagram, header: &str) -> Result<(), String> {
    if header == "pie showData" {
        d.push(121, &["true"]);
    }
    for line in lines {
        if let Some(t) = line.strip_prefix("title ") {
            d.push(90, &[&text(t)?]);
            continue;
        }
        match d.family {
            13 => {
                let (label, value) = line.rsplit_once(':').ok_or_else(|| unsupported(line))?;
                if !label.trim().starts_with('"') {
                    return Err("Pie labels require quotes".into());
                }
                d.push(120, &[&text(label)?, value.trim()]);
            }
            16 => {
                let p: Vec<_> = line.split(',').map(str::trim).collect();
                if p.len() != 3 || p.iter().any(|s| s.contains('"')) {
                    return Err("Sankey requires three unquoted CSV fields".into());
                }
                d.push(150, &[&text(p[0])?, &text(p[1])?, p[2]]);
            }
            17 => {
                if let Some(rest) = line
                    .strip_prefix("x-axis ")
                    .or_else(|| line.strip_prefix("y-axis "))
                {
                    let (a, b) = rest
                        .split_once("-->")
                        .ok_or("Quadrant needs both axis endpoints")?;
                    d.push(161, &[&line[..1], &text(a)?, &text(b)?]);
                } else if let Some(rest) = line.strip_prefix("quadrant-") {
                    let (n, label) = rest.split_once(' ').ok_or("Quadrant label")?;
                    d.push(162, &[n, &text(label)?]);
                } else {
                    let (label, v) = line.split_once(':').ok_or_else(|| unsupported(line))?;
                    let v = values(v, '[', ']')?;
                    if v.len() != 2 {
                        return Err("Quadrant point needs x and y".into());
                    }
                    d.push(160, &[&text(label)?, &v[0], &v[1]]);
                }
            }
            20 => {
                if let Some(rest) = line.strip_prefix("axis ") {
                    for axis in rest.split(',') {
                        let (id, label) = named(axis)?;
                        d.push(190, &[&id, &label]);
                    }
                } else if let Some(rest) = line.strip_prefix("curve ") {
                    let (name, v) = rest.split_once('{').ok_or("Radar curve values")?;
                    let (id, label) = named(name)?;
                    let mut fields = vec![id, label];
                    fields.extend(values(&format!("{{{v}"), '{', '}')?);
                    d.records.push(Record { tag: 191, fields });
                } else {
                    let (key, value) = line.split_once(' ').ok_or_else(|| unsupported(line))?;
                    d.push(192, &[key, value.trim()]);
                }
            }
            22 => {
                if let Some(rest) = line.strip_prefix("x-axis ") {
                    d.records.push(Record {
                        tag: 200,
                        fields: values(rest, '[', ']')?,
                    });
                } else if let Some(rest) = line.strip_prefix("y-axis ") {
                    if let Some((left, max)) = rest.rsplit_once("-->") {
                        let (label, min) =
                            left.trim().rsplit_once(' ').unwrap_or(("", left.trim()));
                        d.push(201, &[&text(label)?, min, max.trim()]);
                    } else {
                        d.push(201, &[&text(rest)?, "", ""]);
                    }
                } else if let Some(rest) = line
                    .strip_prefix("bar ")
                    .or_else(|| line.strip_prefix("line "))
                {
                    let kind = if line.starts_with("bar ") {
                        "bar"
                    } else {
                        "line"
                    };
                    let mut fields = vec![kind.into()];
                    fields.extend(values(rest, '[', ']')?);
                    d.records.push(Record { tag: 202, fields });
                } else {
                    return Err(unsupported(line));
                }
            }
            _ => return Err(unsupported(line)),
        }
    }
    Ok(())
}
