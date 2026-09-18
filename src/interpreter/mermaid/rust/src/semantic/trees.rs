use super::{id, text};
use xfmd_diagram_contracts::semantic::*;
pub fn parse(source: &str, d: &mut Diagram) -> Result<(), String> {
    let mut base = None;
    let mut stack: Vec<String> = vec![];
    let mut serial = 0;
    let mut header = false;
    for raw in source.lines() {
        let line = raw.trim();
        if line.is_empty() || line.starts_with("%%") {
            continue;
        }
        if !header {
            header = true;
            continue;
        }
        if line.contains(":::") || line.starts_with("::icon") || line.starts_with("classDef ") {
            return Err("Mindmap styles/icons are not supported".into());
        }
        if raw.contains('\t') {
            return Err("Use two spaces per hierarchy level".into());
        }
        let indent = raw.len() - raw.trim_start().len();
        let base = *base.get_or_insert(indent);
        if indent < base || (indent - base) % 2 != 0 {
            return Err("Invalid hierarchy indentation".into());
        }
        let level = (indent - base) / 2;
        if level > 8 || level > stack.len() {
            return Err("Skipped/deep hierarchy level".into());
        }
        stack.truncate(level);
        let parent = stack.last().cloned().unwrap_or_default();
        let generated = format!("tree{serial}");
        serial += 1;
        let (name, label, shape, value) = match d.family {
            14 => {
                let mut parsed = None;
                for (open, close, shape) in [
                    ("((", "))", "circle"),
                    ("[", "]", "rectangle"),
                    ("(", ")", "rounded"),
                ] {
                    if let Some((name, label)) = line.split_once(open) {
                        let label = label.strip_suffix(close).ok_or("Unclosed mindmap node")?;
                        parsed = Some((
                            if name.trim().is_empty() {
                                generated.clone()
                            } else {
                                id(name.trim())?.into()
                            },
                            text(label)?,
                            shape.to_string(),
                            String::new(),
                        ));
                        break;
                    }
                }
                parsed.unwrap_or((
                    generated.clone(),
                    text(line)?,
                    "default".into(),
                    String::new(),
                ))
            }
            21 => {
                let (label, value) = line.rsplit_once(':').unwrap_or((line, ""));
                if !label.trim().starts_with('"') {
                    return Err("Treemap labels require quotes".into());
                }
                (
                    generated.clone(),
                    text(label)?,
                    "rectangle".into(),
                    value.trim().into(),
                )
            }
            19 => {
                if level > 1 {
                    return Err("Kanban supports column and card levels".into());
                }
                let (name, label) = super::charts::named(line)?;
                if level == 0 {
                    d.push(180, &[&name, &label]);
                } else {
                    d.push(181, &[&name, &label, &parent]);
                }
                stack.push(name);
                continue;
            }
            _ => unreachable!(),
        };
        d.push(130, &[&name, &label, &parent, &shape, &value]);
        stack.push(name);
    }
    Ok(())
}
