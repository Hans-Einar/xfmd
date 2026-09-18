//! XFMD-owned domain record schema. Field positions are explicit and versioned.
use crate::wire::{Reader, Writer};
#[derive(Clone, Debug, PartialEq)]
pub struct Record {
    pub tag: u32,
    pub fields: Vec<String>,
}
#[derive(Clone, Debug, PartialEq)]
pub struct Diagram {
    pub family: u32,
    pub records: Vec<Record>,
}
pub const DIRECTION: u32 = 0;
pub const STATE: u32 = 10; // id, label, parent composite, role, region id
pub const TRANSITION: u32 = 11; // source, target, label
pub const REGION: u32 = 12; // id, owning composite
pub const CLASS: u32 = 20; // id, label
pub const ATTRIBUTE: u32 = 21; // owning class, declaration
pub const OPERATION: u32 = 22; // owning class, declaration
pub const ANNOTATION: u32 = 23; // owning class, annotation
pub const ASSOCIATION: u32 = 24; // source, target, kind, source multiplicity, target multiplicity, label
pub const REQUIREMENT: u32 = 30; // id, Mermaid requirement/element type
pub const PROPERTY: u32 = 31; // owner, key, value
pub const REQUIREMENT_RELATION: u32 = 32; // source, target, Mermaid relation type
pub const ENTITY: u32 = 40; // id
pub const ENTITY_ATTRIBUTE: u32 = 41; // owner, type, name, keys, comment
pub const ENTITY_RELATION: u32 = 42; // source, target, source cardinality, target cardinality, identifying, label
impl Diagram {
    pub fn push(&mut self, tag: u32, fields: &[&str]) {
        self.records.push(Record {
            tag,
            fields: fields.iter().map(|s| s.to_string()).collect(),
        });
    }
    pub fn validate(&self) -> Result<(), String> {
        use std::collections::HashSet;
        if !(2..=5).contains(&self.family) || self.records.is_empty() || self.records.len() > 512 {
            return Err("Invalid semantic diagram family/record count".into());
        }
        let mut ids = HashSet::new();
        for r in &self.records {
            let (family, size) = match r.tag {
                DIRECTION => (self.family, 1),
                STATE => (2, 5),
                TRANSITION => (2, 3),
                REGION => (2, 2),
                CLASS => (3, 2),
                ATTRIBUTE | OPERATION | ANNOTATION => (3, 2),
                ASSOCIATION => (3, 6),
                REQUIREMENT => (4, 2),
                PROPERTY | REQUIREMENT_RELATION => (4, 3),
                ENTITY => (5, 1),
                ENTITY_ATTRIBUTE => (5, 5),
                ENTITY_RELATION => (5, 6),
                _ => return Err("Unknown semantic record tag".into()),
            };
            if family != self.family
                || r.fields.len() != size
                || r.fields.iter().any(|s| s.len() > 4096)
            {
                return Err("Invalid semantic record schema".into());
            }
            if matches!(r.tag, STATE | CLASS | REQUIREMENT | ENTITY | REGION) {
                let id = &r.fields[0];
                if id.is_empty()
                    || !id.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_')
                    || !ids.insert(id.clone())
                {
                    return Err("Invalid or duplicate semantic identity".into());
                }
            }
        }
        if ids.len() > 128 {
            return Err("Semantic identity limit exceeded".into());
        }
        for r in &self.records {
            let f = &r.fields;
            let reference = |i: usize| {
                if ids.contains(&f[i]) {
                    Ok(())
                } else {
                    Err(format!("Unknown semantic reference: {}", f[i]))
                }
            };
            let choice = |i: usize, values: &[&str]| {
                if values.contains(&f[i].as_str()) {
                    Ok(())
                } else {
                    Err(format!("Invalid semantic enumeration: {}", f[i]))
                }
            };
            match r.tag {
                DIRECTION => choice(0, &["TB", "TD", "LR", "BT", "RL"])?,
                STATE => {
                    choice(
                        3,
                        &[
                            "normal",
                            "composite",
                            "initial",
                            "final",
                            "choice",
                            "fork",
                            "join",
                        ],
                    )?;
                    if !f[2].is_empty() {
                        reference(2)?;
                    }
                    if !f[4].is_empty() {
                        reference(4)?;
                    }
                }
                REGION => reference(1)?,
                TRANSITION | ASSOCIATION | REQUIREMENT_RELATION | ENTITY_RELATION => {
                    reference(0)?;
                    reference(1)?;
                }
                ATTRIBUTE | OPERATION | ANNOTATION | PROPERTY | ENTITY_ATTRIBUTE => reference(0)?,
                _ => {}
            }
            match r.tag {
                ASSOCIATION => choice(
                    2,
                    &[
                        "association",
                        "dependency",
                        "realization",
                        "inheritance",
                        "composition",
                        "aggregation",
                        "link",
                    ],
                )?,
                REQUIREMENT => choice(
                    1,
                    &[
                        "requirement",
                        "functionalRequirement",
                        "interfaceRequirement",
                        "performanceRequirement",
                        "physicalRequirement",
                        "designConstraint",
                        "element",
                    ],
                )?,
                PROPERTY => choice(1, &["id", "text", "risk", "verifymethod", "type", "docref"])?,
                REQUIREMENT_RELATION => choice(
                    2,
                    &[
                        "contains",
                        "copies",
                        "derives",
                        "satisfies",
                        "verifies",
                        "refines",
                        "traces",
                    ],
                )?,
                ENTITY_RELATION => {
                    choice(2, &["one", "zero-one", "many", "zero-many"])?;
                    choice(3, &["one", "zero-one", "many", "zero-many"])?;
                    choice(4, &["true", "false"])?;
                }
                _ => {}
            }
        }
        self.validate_references()?;
        Ok(())
    }
    fn validate_references(&self) -> Result<(), String> {
        let definition = |id: &str, tag: u32| {
            self.records
                .iter()
                .find(|r| r.tag == tag && r.fields[0] == id)
        };
        if !self
            .records
            .iter()
            .any(|r| matches!(r.tag, STATE | CLASS | REQUIREMENT | ENTITY))
        {
            return Err("Empty semantic diagram".into());
        }
        for r in &self.records {
            let f = &r.fields;
            let target = |id: &str, tag: u32| {
                definition(id, tag).ok_or_else(|| format!("Wrong reference kind: {id}"))
            };
            match r.tag {
                STATE => {
                    if !f[2].is_empty() {
                        let parent = target(&f[2], STATE)?;
                        if parent.fields[3] != "composite" {
                            return Err("State parent is not composite".into());
                        }
                    }
                    if !f[4].is_empty() {
                        let region = target(&f[4], REGION)?;
                        if region.fields[1] != f[2] {
                            return Err("State region owner mismatch".into());
                        }
                    }
                    let mut current = r;
                    let mut visited = std::collections::HashSet::new();
                    while !current.fields[2].is_empty() {
                        if !visited.insert(current.fields[0].clone()) || visited.len() > 8 {
                            return Err("Cyclic/deep state hierarchy".into());
                        }
                        current = target(&current.fields[2], STATE)?;
                    }
                }
                REGION => {
                    if target(&f[1], STATE)?.fields[3] != "composite" {
                        return Err("Region requires composite state".into());
                    }
                }
                TRANSITION => {
                    let from = target(&f[0], STATE)?;
                    let to = target(&f[1], STATE)?;
                    if from.fields[2] != to.fields[2] || from.fields[4] != to.fields[4] {
                        return Err(
                            "Transitions across internal state scopes are not supported".into()
                        );
                    }
                }
                ASSOCIATION => {
                    target(&f[0], CLASS)?;
                    target(&f[1], CLASS)?;
                }
                ATTRIBUTE | OPERATION | ANNOTATION => {
                    target(&f[0], CLASS)?;
                }
                ENTITY_ATTRIBUTE => {
                    target(&f[0], ENTITY)?;
                }
                ENTITY_RELATION => {
                    target(&f[0], ENTITY)?;
                    target(&f[1], ENTITY)?;
                }
                REQUIREMENT_RELATION => {
                    target(&f[0], REQUIREMENT)?;
                    target(&f[1], REQUIREMENT)?;
                }
                PROPERTY => {
                    let owner = target(&f[0], REQUIREMENT)?;
                    let allowed = if owner.fields[1] == "element" {
                        &["type", "docref"][..]
                    } else {
                        &["id", "text", "risk", "verifymethod"][..]
                    };
                    if !allowed.contains(&f[1].as_str()) {
                        return Err("Property does not belong to requirement/element type".into());
                    }
                    if f[1] == "risk" && !["low", "medium", "high"].contains(&f[2].as_str()) {
                        return Err("Unsupported requirement risk".into());
                    }
                    if f[1] == "verifymethod"
                        && !["analysis", "inspection", "test", "demonstration"]
                            .contains(&f[2].as_str())
                    {
                        return Err("Unsupported verification method".into());
                    }
                }
                _ => {}
            }
        }
        Ok(())
    }
    pub fn write(&self, w: &mut Writer) {
        w.u32(self.family);
        w.u32(self.records.len() as u32);
        for r in &self.records {
            w.u32(r.tag);
            w.u32(r.fields.len() as u32);
            for f in &r.fields {
                w.text(f);
            }
        }
    }
    pub fn read(r: &mut Reader) -> Result<Self, String> {
        let family = r.u32()?;
        let mut records = Vec::new();
        for _ in 0..r.count(512)? {
            let tag = r.u32()?;
            let mut fields = Vec::new();
            for _ in 0..r.count(16)? {
                fields.push(r.text()?);
            }
            records.push(Record { tag, fields });
        }
        let d = Self { family, records };
        d.validate()?;
        Ok(d)
    }
}
