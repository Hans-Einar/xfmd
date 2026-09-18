use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut spaces = 0;
    for line in lines {
        if let Some(value) = line.strip_prefix("columns ") {
            d.push(70, &[value]);
            continue;
        }
        if *line == "space" || line.starts_with("space:") {
            spaces += 1;
            let span = line.strip_prefix("space:").unwrap_or("1");
            d.push(72, &[&format!("__xfmdSpace{spaces}"), span]);
            continue;
        }
        if let Some((from, to)) = line.split_once("-->") {
            d.push(73, &[id(from.trim())?, id(to.trim())?, "", "2"]);
            continue;
        }
        if let Some((from, to)) = line.split_once("---") {
            d.push(73, &[id(from.trim())?, id(to.trim())?, "", "0"]);
            continue;
        }
        if let Some((name, tail)) = line.split_once('[') {
            let (label, span) = tail.rsplit_once(']').ok_or("Unclosed block label")?;
            let span = if span.is_empty() {
                "1"
            } else {
                span.strip_prefix(':').ok_or("Unsupported block suffix")?
            };
            d.push(71, &[id(name)?, &text(label)?, span, "rectangle"]);
            continue;
        }
        if id(line).is_ok() {
            d.push(71, &[line, line, "1", "rectangle"]);
            continue;
        }
        return Err(unsupported(line));
    }
    Ok(())
}
