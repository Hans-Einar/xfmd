use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut section = String::new();
    let mut serial = 0;
    let mut next = 0u32;
    for line in lines {
        if let Some(value) = line.strip_prefix("title ") {
            d.push(90, &[&text(value)?]);
            continue;
        }
        if let Some(value) = line.strip_prefix("section ") {
            section = format!("section{serial}");
            serial += 1;
            d.push(91, &[&section, &text(value)?]);
            continue;
        }
        if d.family == 11 && *line == "dateFormat YYYY-MM-DD" {
            continue;
        }
        let (a, b) = line.split_once(':').ok_or_else(|| unsupported(line))?;
        let a = a.trim();
        let b = b.trim();
        let name = format!("item{serial}");
        serial += 1;
        match d.family {
            9 => {
                let (start, end) = if let Some(n) = a.strip_prefix('+') {
                    let n: u32 = n.parse().map_err(|_| "Packet width")?;
                    if n == 0 || n > 4096 {
                        return Err("Packet width limit".into());
                    }
                    (next, next + n - 1)
                } else if let Some((start, end)) = a.split_once('-') {
                    (
                        start.trim().parse().map_err(|_| "Packet start")?,
                        end.trim().parse().map_err(|_| "Packet end")?,
                    )
                } else {
                    let bit = a.parse().map_err(|_| "Packet bit")?;
                    (bit, bit)
                };
                if end >= 4096 {
                    return Err("Packet bit limit".into());
                }
                next = end + 1;
                if !b.starts_with('"') || !b.ends_with('"') {
                    return Err("Packet label requires quotes".into());
                }
                d.push(80, &[&start.to_string(), &end.to_string(), &text(b)?]);
            }
            10 => {
                let events: Vec<_> = b.split(':').map(text).collect::<Result<_, _>>()?;
                if a.is_empty() {
                    let last = d
                        .records
                        .last_mut()
                        .filter(|r| r.tag == 92)
                        .ok_or("Timeline continuation needs preceding period")?;
                    last.fields.extend(events);
                } else {
                    let mut fields = vec![name, text(a)?, section.clone()];
                    fields.extend(events);
                    d.records.push(Record { tag: 92, fields });
                }
            }
            11 => {
                let mut p: Vec<_> = b.split(',').map(str::trim).collect();
                let status = if p
                    .first()
                    .is_some_and(|s| ["done", "active", "crit"].contains(s))
                {
                    p.remove(0)
                } else {
                    ""
                };
                if p.len() != 3 {
                    return Err("Gantt needs explicit ID, start/after and duration".into());
                }
                let (start, after) = if let Some(after) = p[1].strip_prefix("after ") {
                    ("", id(after)?)
                } else {
                    (p[1], "")
                };
                let duration = p[2]
                    .strip_suffix('d')
                    .ok_or("Gantt duration must be whole days")?;
                d.push(
                    100,
                    &[
                        id(p[0])?,
                        &text(a)?,
                        start,
                        duration,
                        after,
                        &section,
                        status,
                    ],
                );
            }
            12 => {
                let (score, actors) = b
                    .split_once(':')
                    .ok_or("Journey requires score and actors")?;
                let actors: Vec<_> = actors.split(',').map(text).collect::<Result<_, _>>()?;
                d.push(
                    110,
                    &[&name, &text(a)?, score.trim(), &section, &actors.join(",")],
                );
            }
            _ => unreachable!(),
        }
    }
    Ok(())
}
