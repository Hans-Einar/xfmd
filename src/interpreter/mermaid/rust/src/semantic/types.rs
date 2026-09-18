use super::{direction, id, text, tokens, unsupported};
use xfmd_diagram_contracts::semantic::*;
fn ensure(d: &mut Diagram, name: &str) -> Result<(), String> {
    id(name)?;
    let tag = if d.family == 3 { CLASS } else { ENTITY };
    if !d
        .records
        .iter()
        .any(|r| r.tag == tag && r.fields[0] == name)
    {
        if tag == CLASS {
            d.push(tag, &[name, name]);
        } else {
            d.push(tag, &[name]);
        }
    }
    Ok(())
}
fn member(d: &mut Diagram, owner: &str, s: &str) -> Result<(), String> {
    if d.family == 3 {
        if let Some(annotation) = s.strip_prefix("<<").and_then(|s| s.strip_suffix(">>")) {
            d.push(ANNOTATION, &[owner, &text(annotation)?]);
        } else {
            let value = text(s)?;
            if value.is_empty() {
                return Err("Empty class member".into());
            }
            let operation = value.contains('(') && value.contains(')');
            if value.contains('(') != value.contains(')') {
                return Err("Unbalanced operation".into());
            }
            d.push(
                if operation { OPERATION } else { ATTRIBUTE },
                &[owner, &value],
            );
        }
    } else {
        let t = tokens(s)?;
        if !(2..=4).contains(&t.len()) {
            return Err("ER attribute needs type, name, optional keys and comment".into());
        }
        id(&t[0])?;
        id(&t[1])?;
        let mut keys = String::new();
        let mut comment = String::new();
        for token in &t[2..] {
            if token.starts_with('"') {
                if !comment.is_empty() {
                    return Err("Duplicate ER comment".into());
                }
                comment = text(token)?;
            } else {
                if !keys.is_empty() || !token.split(',').all(|s| ["PK", "FK", "UK"].contains(&s)) {
                    return Err("Unsupported ER key".into());
                }
                keys = token.clone();
            }
        }
        d.push(ENTITY_ATTRIBUTE, &[owner, &t[0], &t[1], &keys, &comment]);
    }
    Ok(())
}
pub fn parse(lines: &[&str], d: &mut Diagram) -> Result<(), String> {
    let mut active: Option<String> = None;
    for line in lines {
        if let Some(owner) = &active {
            if *line == "}" {
                active = None;
            } else {
                member(d, owner, line)?;
            }
            continue;
        }
        if direction(line, d)? {
            continue;
        }
        if line.ends_with('{') {
            let name = line.trim_end_matches('{').trim();
            let name = if d.family == 3 {
                name.strip_prefix("class ")
                    .ok_or("Class block requires class keyword")?
            } else {
                name
            };
            ensure(d, name)?;
            active = Some(name.into());
            continue;
        }
        if d.family == 3 {
            if let Some(name) = line.strip_prefix("class ") {
                ensure(d, name)?;
                continue;
            }
            let (ends, label) = line.split_once(" : ").unwrap_or((line, ""));
            let mut relation = None;
            for (symbol, kind, reverse) in [
                ("<|--", "inheritance", true),
                ("--|>", "inheritance", false),
                ("..|>", "realization", false),
                ("<|..", "realization", true),
                ("*--", "composition", false),
                ("--*", "composition", true),
                ("o--", "aggregation", false),
                ("--o", "aggregation", true),
                ("..>", "dependency", false),
                ("<..", "dependency", true),
                ("-->", "association", false),
                ("<--", "association", true),
                ("--", "link", false),
            ] {
                if let Some((left, right)) = ends.split_once(symbol) {
                    relation = Some((left, right, kind, reverse));
                    break;
                }
            }
            if let Some((left, right, kind, reverse)) = relation {
                let l = tokens(left)?;
                let r = tokens(right)?;
                if !(1..=2).contains(&l.len()) || !(1..=2).contains(&r.len()) {
                    return Err("Invalid class relation".into());
                }
                if (l.len() == 2 && !l[1].starts_with('"'))
                    || (r.len() == 2 && !r[0].starts_with('"'))
                {
                    return Err("Class multiplicity must be quoted".into());
                }
                let (from, to) = (&l[0], &r[r.len() - 1]);
                ensure(d, from)?;
                ensure(d, to)?;
                let lm = if l.len() == 2 {
                    text(&l[1])?
                } else {
                    String::new()
                };
                let rm = if r.len() == 2 {
                    text(&r[0])?
                } else {
                    String::new()
                };
                let label = text(label)?;
                if reverse {
                    d.push(ASSOCIATION, &[to, from, kind, &rm, &lm, &label]);
                } else {
                    d.push(ASSOCIATION, &[from, to, kind, &lm, &rm, &label]);
                }
                continue;
            }
            if let Some((owner, value)) = line.split_once(':') {
                ensure(d, owner.trim())?;
                member(d, owner.trim(), value.trim())?;
                continue;
            }
        } else {
            if let Some((ends, label)) = line.split_once(':') {
                let t = tokens(ends)?;
                if t.len() != 3 {
                    return Err("ER relation requires spaced endpoints".into());
                }
                let (left, right, ident) = if let Some((a, b)) = t[1].split_once("--") {
                    (a, b, "true")
                } else if let Some((a, b)) = t[1].split_once("..") {
                    (a, b, "false")
                } else {
                    return Err("Invalid ER relation".into());
                };
                let cardinality = |s: &str| match s {
                    "||" => Ok("one"),
                    "o|" | "|o" => Ok("zero-one"),
                    "}|" | "|{" => Ok("many"),
                    "}o" | "o{" => Ok("zero-many"),
                    _ => Err("Unsupported ER cardinality"),
                };
                ensure(d, &t[0])?;
                ensure(d, &t[2])?;
                d.push(
                    ENTITY_RELATION,
                    &[
                        &t[0],
                        &t[2],
                        cardinality(left)?,
                        cardinality(right)?,
                        ident,
                        &text(label)?,
                    ],
                );
                continue;
            }
            if id(line).is_ok() {
                ensure(d, line)?;
                continue;
            }
        }
        return Err(unsupported(line));
    }
    if active.is_some() {
        return Err("Unclosed type body".into());
    }
    Ok(())
}
