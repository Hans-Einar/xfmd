use super::Diagram;
pub fn validate(d: &Diagram) -> Result<(), String> {
    if !(9..=12).contains(&d.family) || d.records.is_empty() || d.records.len() > 256 {
        return Err("Planning record limit".into());
    }
    let mut ids = std::collections::HashSet::new();
    let mut sections = std::collections::HashSet::new();
    let mut next = 0;
    let mut title = false;
    let mut count = 0;
    for r in &d.records {
        let f = &r.fields;
        let valid = match r.tag {
            80 => d.family == 9 && f.len() == 3,
            90 => d.family >= 10 && f.len() == 1,
            91 => d.family >= 10 && f.len() == 2,
            92 => d.family == 10 && (4..=16).contains(&f.len()),
            100 => d.family == 11 && f.len() == 7,
            110 => d.family == 12 && f.len() == 5,
            _ => false,
        };
        if !valid || f.iter().any(|s| s.len() > 4096) {
            return Err("Planning schema".into());
        }
        match r.tag {
            80 => {
                let start: u32 = f[0].parse().map_err(|_| "Packet bit")?;
                let end: u32 = f[1].parse().map_err(|_| "Packet bit")?;
                if start < next || end < start || end >= 4096 {
                    return Err("Packet overlap/range".into());
                }
                next = end + 1;
                count += 1;
            }
            90 => {
                if title {
                    return Err("Duplicate title".into());
                }
                title = true;
            }
            91 => {
                if f[0].is_empty() || !sections.insert(f[0].clone()) {
                    return Err("Duplicate section".into());
                }
            }
            92 | 100 | 110 => {
                if f[0].is_empty() || ids.contains(&f[0]) {
                    return Err("Duplicate task/event".into());
                }
                let section = &f[if r.tag == 100 {
                    5
                } else if r.tag == 110 {
                    3
                } else {
                    2
                }];
                if !section.is_empty() && !sections.contains(section) {
                    return Err("Unknown section".into());
                }
                if r.tag == 100 {
                    if f[4].is_empty() {
                        date(&f[2])?;
                    } else if !f[2].is_empty() || !ids.contains(&f[4]) {
                        return Err("Gantt dependency must precede task".into());
                    }
                    let duration: u32 = f[3].parse().map_err(|_| "Gantt duration")?;
                    if duration == 0
                        || duration > 3650
                        || !["", "done", "active", "crit"].contains(&f[6].as_str())
                    {
                        return Err("Gantt duration/status".into());
                    }
                }
                if r.tag == 110 {
                    let score: u32 = f[2].parse().map_err(|_| "Journey score")?;
                    if !(1..=5).contains(&score)
                        || f[4].split(',').count() > 16
                        || f[4].split(',').any(|s| s.trim().is_empty())
                    {
                        return Err("Journey score/actors".into());
                    }
                }
                ids.insert(f[0].clone());
                count += 1;
            }
            _ => {}
        }
    }
    if count == 0 || count > 128 {
        return Err("Planning item count".into());
    }
    Ok(())
}
fn date(s: &str) -> Result<(), String> {
    let parts: Vec<_> = s.split('-').collect();
    if parts.len() != 3 || parts[0].len() != 4 || parts[1].len() != 2 || parts[2].len() != 2 {
        return Err("Use YYYY-MM-DD".into());
    }
    let y: u32 = parts[0].parse().map_err(|_| "Year")?;
    let m: usize = parts[1].parse().map_err(|_| "Month")?;
    let d: u32 = parts[2].parse().map_err(|_| "Day")?;
    let days = [
        31,
        if y % 4 == 0 && (y % 100 != 0 || y % 400 == 0) {
            29
        } else {
            28
        },
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31,
    ];
    if !(1970..=2100).contains(&y) || !(1..=12).contains(&m) || d == 0 || d > days[m - 1] {
        return Err("Invalid/bounded calendar date".into());
    }
    Ok(())
}
