//! C4, resource architecture and block-grid schemas. No renderer types.
use super::Diagram;
use std::collections::BTreeMap;
pub fn validate(d: &Diagram) -> Result<(), String> {
    if !(6..=8).contains(&d.family) || d.records.is_empty() || d.records.len() > 512 {
        return Err("Architecture record limit/family".into());
    }
    let mut ids = BTreeMap::new();
    for r in &d.records {
        let (family, count) = match r.tag {
            50 => (6, 1),
            51 => (6, 6),
            52 => (6, 4),
            53 => (6, 5),
            60 | 61 => (7, 4),
            62 => (7, 2),
            63 => (7, 5),
            70 => (8, 1),
            71 => (8, 4),
            72 => (8, 2),
            73 => (8, 4),
            _ => return Err("Unknown architecture record".into()),
        };
        if family != d.family || r.fields.len() != count || r.fields.iter().any(|s| s.len() > 4096)
        {
            return Err("Architecture record schema".into());
        }
        if matches!(r.tag, 51 | 52 | 60 | 61 | 62 | 71 | 72) {
            let id = &r.fields[0];
            if id.is_empty()
                || !id.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_')
                || ids.insert(id.as_str(), r).is_some()
            {
                return Err("Duplicate/invalid architecture identity".into());
            }
        }
    }
    if ids.is_empty() || ids.len() > 128 {
        return Err("Architecture identity limit".into());
    }
    let reference = |id: &str, tags: &[u32]| -> Result<(), String> {
        if ids.get(id).is_some_and(|r| tags.contains(&r.tag)) {
            Ok(())
        } else {
            Err(format!("Unknown architecture reference: {id}"))
        }
    };
    let choice = |s: &str, values: &[&str]| -> Result<(), String> {
        if values.contains(&s) {
            Ok(())
        } else {
            Err(format!("Unsupported architecture value: {s}"))
        }
    };
    let mut columns = 1usize;
    let mut headers = 0;
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            50 => {
                headers += 1;
                choice(&f[0], &["C4Context", "C4Container", "C4Component"])?;
            }
            51 => {
                choice(
                    &f[2],
                    &[
                        "Person",
                        "Person_Ext",
                        "System",
                        "SystemDb",
                        "SystemQueue",
                        "System_Ext",
                        "SystemDb_Ext",
                        "SystemQueue_Ext",
                        "Container",
                        "ContainerDb",
                        "ContainerQueue",
                        "Container_Ext",
                        "ContainerDb_Ext",
                        "ContainerQueue_Ext",
                        "Component",
                        "ComponentDb",
                        "ComponentQueue",
                        "Component_Ext",
                        "ComponentDb_Ext",
                        "ComponentQueue_Ext",
                    ],
                )?;
                if !f[5].is_empty() {
                    reference(&f[5], &[52])?;
                }
            }
            52 => {
                choice(
                    &f[2],
                    &[
                        "Boundary",
                        "Enterprise_Boundary",
                        "System_Boundary",
                        "Container_Boundary",
                    ],
                )?;
                if !f[3].is_empty() {
                    reference(&f[3], &[52])?;
                }
                let mut node = r;
                let mut seen = std::collections::HashSet::new();
                while !node.fields[3].is_empty() {
                    if !seen.insert(&node.fields[0]) || seen.len() > 8 {
                        return Err("Cyclic/deep C4 hierarchy".into());
                    }
                    node = ids[node.fields[3].as_str()];
                }
            }
            53 => {
                reference(&f[0], &[51])?;
                reference(&f[1], &[51])?;
                choice(
                    &f[2],
                    &[
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
                    ],
                )?;
            }
            60 | 61 => {
                choice(
                    &f[2],
                    &["", "cloud", "database", "disk", "internet", "server"],
                )?;
                if !f[3].is_empty() {
                    if r.tag == 60 {
                        return Err("Nested architecture groups unsupported".into());
                    }
                    reference(&f[3], &[60])?;
                }
            }
            62 => {
                if !f[1].is_empty() {
                    reference(&f[1], &[60])?;
                }
            }
            63 => {
                reference(&f[0], &[61, 62])?;
                reference(&f[1], &[61, 62])?;
                choice(&f[2], &["L", "R", "T", "B"])?;
                choice(&f[3], &["L", "R", "T", "B"])?;
                choice(&f[4], &["0", "1", "2", "3"])?;
            }
            70 => {
                headers += 1;
                columns = f[0].parse().map_err(|_| "Invalid block columns")?;
                if !(1..=16).contains(&columns) {
                    return Err("Block columns limit".into());
                }
            }
            71 => choice(&f[3], &["rectangle"])?,
            73 => {
                reference(&f[0], &[71])?;
                reference(&f[1], &[71])?;
                choice(&f[3], &["0", "2"])?;
            }
            _ => {}
        }
    }
    if (d.family == 6 && headers != 1) || (d.family == 8 && headers > 1) {
        return Err("Architecture header count".into());
    }
    for r in &d.records {
        if r.tag == 71 || r.tag == 72 {
            let i = if r.tag == 71 { 2 } else { 1 };
            let span: usize = r.fields[i].parse().map_err(|_| "Invalid block span")?;
            if span == 0 || span > columns {
                return Err("Block span exceeds columns".into());
            }
        }
    }
    Ok(())
}
