use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
fn participant(d: &mut Diagram, name: &str, label: &str) {
    if !d
        .records
        .iter()
        .any(|r| r.tag == 170 && r.fields[0] == name)
    {
        d.push(170, &[name, label]);
    }
}
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    for line in lines {
        if let Some((a, rest)) = line.split_once("->") {
            let (b, label) = rest
                .split_once(':')
                .ok_or("ZenUML async message requires label")?;
            let a = id(a.trim())?;
            let b = id(b.trim())?;
            participant(d, a, a);
            participant(d, b, b);
            d.push(171, &[a, b, &text(label)?]);
        } else if let Some((a, label)) = line.split_once(" as ") {
            let a = id(a)?;
            if d.records.iter().any(|r| r.tag == 170 && r.fields[0] == a) {
                return Err("Declare aliases before use".into());
            }
            participant(d, a, &text(label)?);
        } else if id(line).is_ok() {
            participant(d, line, line);
        } else {
            return Err(unsupported(line));
        }
    }
    Ok(())
}
