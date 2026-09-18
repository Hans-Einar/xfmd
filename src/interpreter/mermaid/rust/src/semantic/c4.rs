use super::{id, text, unsupported};
use xfmd_diagram_contracts::semantic::*;
// The C4 call grammar has positional plain/quoted arguments, no executable code.
fn arguments(s: &str) -> Result<Vec<String>, String> {
    let mut result = vec![];
    let mut value = String::new();
    let mut quote = false;
    for c in s.chars() {
        if c == '"' {
            quote = !quote;
            value.push(c);
        } else if c == ',' && !quote {
            result.push(std::mem::take(&mut value).trim().into());
        } else {
            value.push(c);
        }
    }
    if quote {
        return Err("Unclosed C4 quote".into());
    }
    result.push(value.trim().into());
    Ok(result)
}
pub fn parse(lines: &[&str], d: &mut Diagram, header: &str) -> Result<(), String> {
    d.push(50, &[header]);
    let mut parents: Vec<String> = vec![];
    for line in lines {
        if *line == "}" {
            parents.pop().ok_or("Unexpected C4 boundary end")?;
            continue;
        }
        let boundary = line.ends_with('{');
        let line = line.trim_end_matches('{').trim();
        let (function, args) = line.split_once('(').ok_or_else(|| unsupported(line))?;
        let args = args.strip_suffix(')').ok_or("Unclosed C4 call")?;
        let args = arguments(args)?;
        let parent = parents.last().cloned().unwrap_or_default();
        if [
            "Boundary",
            "Enterprise_Boundary",
            "System_Boundary",
            "Container_Boundary",
        ]
        .contains(&function)
        {
            if !boundary || args.len() != 2 {
                return Err("C4 boundary requires identity, label and explicit body".into());
            }
            let name = id(&args[0])?;
            d.push(52, &[name, &text(&args[1])?, function, &parent]);
            parents.push(name.into());
            if parents.len() > 8 {
                return Err("C4 boundary nesting exceeds eight levels".into());
            }
            continue;
        }
        if boundary {
            return Err(unsupported(line));
        }
        if [
            "Rel",
            "BiRel",
            "Rel_U",
            "Rel_Up",
            "Rel_D",
            "Rel_Down",
            "Rel_L",
            "Rel_Left",
            "Rel_R",
            "Rel_Right",
            "Rel_Back",
        ]
        .contains(&function)
        {
            if !(3..=4).contains(&args.len()) {
                return Err(
                    "C4 relation needs source, target, label and optional technology".into(),
                );
            }
            d.push(
                53,
                &[
                    id(&args[0])?,
                    id(&args[1])?,
                    function,
                    &text(&args[2])?,
                    &if args.len() == 4 {
                        text(&args[3])?
                    } else {
                        String::new()
                    },
                ],
            );
            continue;
        }
        let technology = function.starts_with("Container") || function.starts_with("Component");
        let required = if technology { 3 } else { 2 };
        if args.len() < required || args.len() > required + 1 {
            return Err("Unsupported C4 element parameters".into());
        }
        let name = id(&args[0])?;
        let label = text(&args[1])?;
        let techn = if technology {
            text(&args[2])?
        } else {
            String::new()
        };
        let description = if args.len() > required {
            text(&args[required])?
        } else {
            String::new()
        };
        d.push(51, &[name, &label, function, &techn, &description, &parent]);
    }
    if !parents.is_empty() {
        return Err("Unclosed C4 boundary".into());
    }
    Ok(())
}
