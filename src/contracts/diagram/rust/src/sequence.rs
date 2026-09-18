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
}
impl Kind {
    pub fn read(r: &mut Reader) -> Result<Self, String> {
        use Kind::*;
        Ok([
            Message, Reply, NoteLeft, NoteRight, NoteOver, Activate, Deactivate, Alt, Opt, Loop,
            Par, Else, And, End,
        ][r.count(13)? as usize])
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
        let mut frame = None;
        let mut messages = 0;
        let mut section_start = 0;
        let mut previous_note = false;
        for e in &self.events {
            if e.first as usize >= active.len() || e.second as usize >= active.len() {
                return Err("Invalid sequence participant reference".into());
            }
            match e.kind {
                Message | Reply => messages += 1,
                Activate => {
                    if previous_note {
                        return Err(
                            "Sequence 1: activation after a note needs an intervening message"
                                .into(),
                        );
                    }
                    if frame.is_some() {
                        return Err(
                            "Sequence 1: activation inside fragments is not supported".into()
                        );
                    }
                    active[e.first as usize] += 1;
                }
                Deactivate => {
                    if previous_note {
                        return Err(
                            "Sequence 1: deactivation after a note needs an intervening message"
                                .into(),
                        );
                    }
                    if frame.is_some() {
                        return Err(
                            "Sequence 1: activation inside fragments is not supported".into()
                        );
                    }
                    let a = &mut active[e.first as usize];
                    *a = a.checked_sub(1).ok_or("Unbalanced activation")?;
                }
                NoteLeft | NoteRight | NoteOver if frame.is_some() => {
                    return Err("Sequence 1: notes inside fragments are not supported".into());
                }
                Alt | Opt | Loop | Par => {
                    if previous_note {
                        return Err(
                            "Sequence 1: place a message between a note and a fragment".into()
                        );
                    }
                    if frame.replace(e.kind).is_some() {
                        return Err("Sequence 1: nested fragments are not supported".into());
                    }
                    section_start = messages;
                }
                Else | And => {
                    if !matches!((frame, e.kind), (Some(Alt), Else) | (Some(Par), And))
                        || section_start == messages
                    {
                        return Err("Invalid/empty sequence branch".into());
                    }
                    section_start = messages;
                }
                End => {
                    if frame.take().is_none() || section_start == messages {
                        return Err("Invalid/empty fragment end".into());
                    }
                }
                _ => {}
            }
            if matches!(e.kind, NoteLeft | NoteRight | NoteOver) {
                previous_note = true;
            }
            if matches!(e.kind, Message | Reply) {
                previous_note = false;
            }
        }
        if frame.is_some() || active.iter().any(|a| *a != 0) || messages == 0 {
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
