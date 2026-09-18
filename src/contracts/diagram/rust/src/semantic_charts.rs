//! Bounded records for charts, trees, commit history and asynchronous messages.
use super::Diagram;
use std::collections::{BTreeMap, HashSet};
pub fn number(s: &str) -> Result<f32, String> {
    let n: f32 = s.parse().map_err(|_| "Expected number")?;
    if !n.is_finite() || n.abs() > 1_000_000. {
        return Err("Non-finite/out-of-range number".into());
    }
    Ok(n)
}
pub fn validate(d: &Diagram) -> Result<(), String> {
    if !(13..=22).contains(&d.family) || d.records.is_empty() || d.records.len() > 256 {
        return Err("Chart family/record limit".into());
    }
    let mut ids: BTreeMap<&str, u32> = BTreeMap::new();
    let mut once = HashSet::new();
    let mut count = 0;
    let mut axes = 0;
    let mut categories = 0;
    let mut roots = 0;
    let mut parents = BTreeMap::new();
    for r in &d.records {
        let f = &r.fields;
        let valid = match r.tag {
            90 => [13, 17, 20, 22].contains(&d.family) && f.len() == 1,
            120 => d.family == 13 && f.len() == 2,
            121 => d.family == 13 && f.len() == 1,
            130 => [14, 21].contains(&d.family) && f.len() == 5,
            140 => d.family == 15 && f.len() == 2,
            141 => d.family == 15 && f.len() == 6,
            150 => d.family == 16 && f.len() == 3,
            160 => d.family == 17 && f.len() == 3,
            161 => d.family == 17 && f.len() == 3,
            162 => d.family == 17 && f.len() == 2,
            170 => d.family == 18 && f.len() == 2,
            171 => d.family == 18 && f.len() == 3,
            180 => d.family == 19 && f.len() == 2,
            181 => d.family == 19 && f.len() == 3,
            190 => d.family == 20 && f.len() == 2,
            191 => d.family == 20 && (5..=14).contains(&f.len()),
            192 => d.family == 20 && f.len() == 2,
            200 => d.family == 22 && (1..=12).contains(&f.len()),
            201 => d.family == 22 && f.len() == 3,
            202 => d.family == 22 && (2..=13).contains(&f.len()),
            _ => false,
        };
        if !valid || f.iter().any(|s| s.len() > 4096) {
            return Err("Chart record schema".into());
        }
        let reference = |name: &str, tag| -> Result<(), String> {
            if ids.get(name) == Some(&tag) {
                Ok(())
            } else {
                Err(format!("Unknown chart reference: {name}"))
            }
        };
        match r.tag {
            90 | 121 | 200 | 201 => {
                if !once.insert(format!("{}", r.tag)) {
                    return Err("Duplicate chart option".into());
                }
            }
            _ => {}
        }
        match r.tag {
            120 => {
                if f[0].is_empty() || number(&f[1])? <= 0. {
                    return Err("Pie needs positive values".into());
                }
                count += 1;
            }
            121 => {
                if f[0] != "true" {
                    return Err("Invalid showData".into());
                }
            }
            130 => {
                if f[2].is_empty() {
                    roots += 1;
                } else {
                    reference(&f[2], 130)?;
                }
                if !["default", "rectangle", "rounded", "circle"].contains(&f[3].as_str()) {
                    return Err("Tree shape".into());
                }
                if d.family == 21 && !f[4].is_empty() && number(&f[4])? <= 0. {
                    return Err("Treemap needs positive values".into());
                }
                if d.family == 14 && !f[4].is_empty() {
                    return Err("Mindmap has no weights".into());
                }
                parents.insert(f[0].as_str(), f[2].as_str());
                count += 1;
            }
            140 => {
                if !f[1].is_empty() {
                    reference(&f[1], 141)?;
                }
            }
            141 => {
                reference(&f[1], 140)?;
                for p in [&f[2], &f[3]] {
                    if !p.is_empty() {
                        reference(p, 141)?;
                    }
                }
                if !["normal", "merge"].contains(&f[4].as_str())
                    || !f[5].is_empty()
                    || (f[4] == "merge" && (f[2].is_empty() || f[3].is_empty()))
                {
                    return Err("Commit kind/parents".into());
                }
                count += 1;
            }
            150 => {
                if f[0].is_empty() || f[1].is_empty() || f[0] == f[1] || number(&f[2])? <= 0. {
                    return Err("Sankey endpoints/weight".into());
                }
                count += 1;
            }
            160 => {
                if !(0. ..=1.).contains(&number(&f[1])?) || !(0. ..=1.).contains(&number(&f[2])?) {
                    return Err("Quadrant coordinates 0..1".into());
                }
                count += 1;
            }
            161 | 162 => {
                let valid = if r.tag == 161 {
                    ["x", "y"].contains(&f[0].as_str())
                } else {
                    ["1", "2", "3", "4"].contains(&f[0].as_str())
                };
                if !valid || !once.insert(format!("{}{}", r.tag, f[0])) {
                    return Err("Quadrant axis/label".into());
                }
            }
            171 => {
                reference(&f[0], 170)?;
                reference(&f[1], 170)?;
                count += 1;
            }
            181 => {
                reference(&f[2], 180)?;
                count += 1;
            }
            190 => axes += 1,
            191 => {
                for v in &f[2..] {
                    number(v)?;
                }
                count += 1;
            }
            192 => {
                if !["min", "max", "graticule"].contains(&f[0].as_str())
                    || !once.insert(format!("radar{}", f[0]))
                {
                    return Err("Radar option".into());
                }
                if f[0] == "graticule" {
                    if !["circle", "polygon"].contains(&f[1].as_str()) {
                        return Err("Radar graticule".into());
                    }
                } else {
                    number(&f[1])?;
                }
            }
            200 => {
                categories = f.len();
                if f.iter().any(|s| s.is_empty()) {
                    return Err("Empty category".into());
                }
            }
            201 => {
                if f[1].is_empty() != f[2].is_empty() {
                    return Err("Incomplete Y range".into());
                }
                if !f[1].is_empty() && number(&f[1])? >= number(&f[2])? {
                    return Err("Y range order".into());
                }
            }
            202 => {
                if !["bar", "line"].contains(&f[0].as_str()) {
                    return Err("XY series kind".into());
                }
                for v in &f[1..] {
                    number(v)?;
                }
                count += 1;
            }
            _ => {}
        }
        if matches!(r.tag, 130 | 140 | 141 | 170 | 180 | 181 | 190 | 191) {
            if f[0].is_empty() || ids.insert(&f[0], r.tag).is_some() {
                return Err("Duplicate chart identity".into());
            }
        }
    }
    if count == 0 || count > 128 || ids.len() > 128 || (d.family == 13 && count > 12) {
        return Err("Chart item count".into());
    }
    if d.family == 14 && roots != 1 {
        return Err("Mindmap needs one root".into());
    }
    for r in &d.records {
        let f = &r.fields;
        if r.tag == 130 {
            let mut at = f[0].as_str();
            for depth in 0..=8 {
                let parent = parents[at];
                if parent.is_empty() {
                    break;
                }
                if depth == 8 {
                    return Err("Tree depth limit".into());
                }
                at = parent;
            }
            if d.family == 21 {
                let has_children = parents.values().any(|p| *p == f[0]);
                if has_children == !f[4].is_empty() {
                    return Err("Treemap weights belong only to leaves".into());
                }
            }
        }
        if r.tag == 191 && f.len() != axes + 2 {
            return Err("Radar values must match axes".into());
        }
        if r.tag == 202 && f.len() != categories + 1 {
            return Err("XY values must match categories".into());
        }
    }
    if d.family == 20 {
        if !(3..=12).contains(&axes) {
            return Err("Radar axis limit".into());
        }
        let option = |key: &str| {
            d.records
                .iter()
                .find(|r| r.tag == 192 && r.fields[0] == key)
                .map(|r| number(&r.fields[1]).unwrap())
        };
        let min = option("min").unwrap_or(0.);
        let max = option("max").unwrap_or(1_000_000.);
        if min >= max {
            return Err("Radar scale order".into());
        }
        for r in &d.records {
            if r.tag == 191
                && r.fields[2..].iter().any(|v| {
                    let n = number(v).unwrap();
                    n < min || n > max
                })
            {
                return Err("Radar value outside scale".into());
            }
        }
    }
    if d.family == 16 {
        let mut remaining: Vec<_> = d.records.iter().filter(|r| r.tag == 150).collect();
        while !remaining.is_empty() {
            let sources: HashSet<_> = remaining
                .iter()
                .map(|r| r.fields[0].as_str())
                .filter(|s| !remaining.iter().any(|r| r.fields[1] == *s))
                .collect();
            if sources.is_empty() {
                return Err("Cyclic Sankey unsupported".into());
            }
            remaining.retain(|r| !sources.contains(r.fields[0].as_str()));
        }
    }
    Ok(())
}
