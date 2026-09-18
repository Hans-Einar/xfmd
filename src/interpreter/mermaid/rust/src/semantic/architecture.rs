use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    for line in lines {
        if let Some(rest) = line
            .strip_prefix("group ")
            .or_else(|| line.strip_prefix("service "))
        {
            let (head, tail) = rest
                .split_once('[')
                .ok_or("Architecture node requires label")?;
            let (label, tail) = tail.split_once(']').ok_or("Unclosed architecture label")?;
            let parent = if tail.trim().is_empty() {
                ""
            } else {
                tail.trim()
                    .strip_prefix("in ")
                    .ok_or("Unsupported architecture suffix")?
            };
            if !parent.is_empty() {
                id(parent)?;
            }
            let (name, icon) = if let Some((name, icon)) = head.split_once('(') {
                (
                    name.trim(),
                    icon.trim().strip_suffix(')').ok_or("Unclosed icon")?,
                )
            } else {
                (head.trim(), "")
            };
            if line.starts_with("group ") && !parent.is_empty() {
                return Err("Nested architecture groups are not yet supported".into());
            }
            d.push(
                if line.starts_with("group ") { 60 } else { 61 },
                &[id(name)?, &text(label)?, icon, parent],
            );
            continue;
        }
        if let Some(rest) = line.strip_prefix("junction ") {
            let (name, parent) = rest.split_once(" in ").unwrap_or((rest, ""));
            d.push(62, &[id(name)?, parent]);
            continue;
        }
        let compact = line.trim();
        let mut relation = None;
        for (symbol, arrows) in [("<-->", "3"), ("-->", "2"), ("<--", "1"), ("--", "0")] {
            if let Some((a, b)) = compact.split_once(symbol) {
                relation = Some((a, b, arrows));
                break;
            }
        }
        if let Some((a, b, arrows)) = relation {
            let (from, fp) = a
                .split_once(':')
                .ok_or("Missing architecture source side")?;
            let (tp, to) = b
                .split_once(':')
                .ok_or("Missing architecture target side")?;
            d.push(
                63,
                &[
                    id(from.trim())?,
                    id(to.trim())?,
                    fp.trim(),
                    tp.trim(),
                    arrows,
                ],
            );
            continue;
        }
        return Err(unsupported(line));
    }
    Ok(())
}
