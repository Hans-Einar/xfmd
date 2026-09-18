use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut branches = std::collections::BTreeMap::new();
    branches.insert("main".to_string(), String::new());
    let mut current = "main".to_string();
    let mut serial = 0;
    d.push(140, &["main", ""]);
    for line in lines {
        if let Some(rest) = line.strip_prefix("branch ") {
            let name = id(rest)?;
            if branches.contains_key(name) {
                return Err("Duplicate branch".into());
            }
            let parent = branches[&current].clone();
            branches.insert(name.into(), parent.clone());
            d.push(140, &[name, &parent]);
            current = name.into();
        } else if let Some(rest) = line
            .strip_prefix("checkout ")
            .or_else(|| line.strip_prefix("switch "))
        {
            if !branches.contains_key(rest) {
                return Err("Unknown branch".into());
            }
            current = rest.into();
        } else if let Some(rest) = line.strip_prefix("commit id:") {
            if !rest.starts_with('"') {
                return Err("Commit ID requires quotes".into());
            }
            let name = text(rest)?;
            id(&name)?;
            d.push(
                141,
                &[&name, &current, &branches[&current], "", "normal", ""],
            );
            branches.insert(current.clone(), name);
        } else if let Some(rest) = line.strip_prefix("merge ") {
            let name = id(rest)?;
            let other = branches.get(name).ok_or("Unknown merge branch")?;
            if name == current || other.is_empty() || branches[&current].is_empty() {
                return Err("Invalid merge".into());
            }
            let commit = format!("__merge{serial}");
            serial += 1;
            d.push(
                141,
                &[&commit, &current, &branches[&current], other, "merge", ""],
            );
            branches.insert(current.clone(), commit);
        } else {
            return Err(unsupported(line));
        }
    }
    Ok(())
}
