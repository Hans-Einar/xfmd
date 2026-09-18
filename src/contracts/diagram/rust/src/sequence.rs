//! Ordered interaction values, independent of any parser or graphics library.
use crate::wire::{Reader, Writer};
#[derive(Clone, Debug, PartialEq)]
pub struct Participant {
    pub id: String,
    pub label: String,
    pub actor: bool,
}
#[derive(Clone, Copy, Debug, PartialEq)]
#[repr(u32)]
pub enum Kind {
    Message,
    Reply,
    NoteLeft,
    NoteRight,
    NoteOver,
    Activate,
    Deactivate,
    Alt,
    Opt,
    Loop,
    Par,
    Else,
    And,
    End,
    Async,
    AsyncReply,
}
impl Kind {
    pub fn read(r: &mut Reader) -> Result<Self, String> {
        use Kind::*;
        Ok([
            Message, Reply, NoteLeft, NoteRight, NoteOver, Activate, Deactivate, Alt, Opt, Loop,
            Par, Else, And, End, Async, AsyncReply,
        ][r.count(15)? as usize])
    }
}
#[derive(Clone, Debug, PartialEq)]
pub struct Event {
    pub kind: Kind,
    pub first: u32,
    pub second: u32,
    pub text: String,
}
#[derive(Clone, Debug, PartialEq, Default)]
pub struct Sequence {
    pub participants: Vec<Participant>,
    pub events: Vec<Event>,
}
impl Sequence {
    pub fn validate(&self) -> Result<(), String> {
        use Kind::*;
        if self.participants.is_empty() || self.participants.len() > 16 || self.events.len() > 128 {
            return Err("Sequence limit: 1–16 participants, at most 128 events".into());
        }
        let mut ids = std::collections::BTreeSet::new();
        for p in &self.participants {
            if p.id.is_empty() || !ids.insert(&p.id) {
                return Err("Duplicate/empty participant".into());
            }
        }
        let mut active = vec![0u32; self.participants.len()];
        let mut frames: Vec<(Kind, usize, Vec<u32>)> = vec![];
        let mut messages = 0;
        for (index, e) in self.events.iter().enumerate() {
            if e.first as usize >= active.len() || e.second as usize >= active.len() {
                return Err("Invalid sequence reference".into());
            }
            match e.kind {
                Message | Reply | Async | AsyncReply => messages += 1,
                Activate => active[e.first as usize] += 1,
                Deactivate => {
                    let a = &mut active[e.first as usize];
                    *a = a.checked_sub(1).ok_or("Unbalanced activation")?;
                }
                Alt | Opt | Loop | Par => {
                    if frames.len() >= 8 {
                        return Err("Sequence nesting limit: 8".into());
                    }
                    frames.push((e.kind, index, active.clone()));
                }
                Else | And => {
                    let (kind, start, baseline) =
                        frames.last_mut().ok_or("Branch outside fragment")?;
                    if !matches!((*kind, e.kind), (Alt, Else) | (Par, And))
                        || index == *start + 1
                        || active != *baseline
                    {
                        return Err("Invalid branch or activation crossing alternatives".into());
                    }
                    *start = index;
                }
                End => {
                    let (_, start, baseline) = frames.pop().ok_or("Unexpected fragment end")?;
                    if index == start + 1 || active != baseline {
                        return Err("Empty fragment or unbalanced branch activation".into());
                    }
                }
                _ => {}
            }
        }
        if !frames.is_empty() || active.iter().any(|a| *a != 0) || messages == 0 {
            return Err("Unclosed fragment/activation or missing message".into());
        }
        Ok(())
    }
    pub fn write(&self, w: &mut Writer) {
        w.u32(self.participants.len() as u32);
        for p in &self.participants {
            w.text(&p.id);
            w.text(&p.label);
            w.u32(p.actor as u32);
        }
        w.u32(self.events.len() as u32);
        for e in &self.events {
            w.u32(e.kind as u32);
            w.u32(e.first);
            w.u32(e.second);
            w.text(&e.text);
        }
    }
    pub fn read(r: &mut Reader) -> Result<Self, String> {
        let mut s = Self::default();
        for _ in 0..r.count(16)? {
            s.participants.push(Participant {
                id: r.text()?,
                label: r.text()?,
                actor: r.count(1)? != 0,
            });
        }
        for _ in 0..r.count(128)? {
            s.events.push(Event {
                kind: Kind::read(r)?,
                first: r.u32()?,
                second: r.u32()?,
                text: r.text()?,
            });
        }
        s.validate()?;
        Ok(s)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn reject_corrupt_sequence_records() {
        let value = Sequence {
            participants: vec![Participant {
                id: "A".into(),
                label: "A".into(),
                actor: false,
            }],
            events: vec![Event {
                kind: Kind::Message,
                first: 0,
                second: 0,
                text: "self".into(),
            }],
        };
        let mut writer = Writer::default();
        value.write(&mut writer);
        for end in 0..writer.0.len() {
            assert!(Sequence::read(&mut Reader::new(&writer.0[..end])).is_err());
        }
        let mut bad = value.clone();
        bad.events[0].second = 1;
        assert!(bad.validate().is_err());
        bad = value.clone();
        bad.events = vec![bad.events[0].clone(); 129];
        assert!(bad.validate().is_err());
        bad = value;
        bad.participants.push(bad.participants[0].clone());
        assert!(bad.validate().is_err());
    }
}
