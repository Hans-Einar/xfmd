pub mod semantic;
pub mod sequence;
pub mod wire;
use wire::{Reader, Writer};
#[derive(Clone, Debug, PartialEq)]
pub struct Node {
    pub id: String,
    pub label: String,
    pub shape: u32,
}
#[derive(Clone, Debug, PartialEq)]
pub struct Edge {
    pub from: u32,
    pub to: u32,
    pub label: String,
    pub arrows: u32,
    pub style: u32,
}
#[derive(Clone, Debug, PartialEq)]
pub struct Group {
    pub id: String,
    pub label: String,
    pub direction: u32,
    pub parent: u32,
    pub nodes: Vec<u32>,
}
#[derive(Clone, Debug, PartialEq)]
pub struct Model {
    pub semantic: Option<semantic::Diagram>,
    pub sequence: Option<sequence::Sequence>,
    pub direction: u32,
    pub nodes: Vec<Node>,
    pub edges: Vec<Edge>,
    pub groups: Vec<Group>,
}
impl Model {
    pub fn write(&self, w: &mut Writer) {
        w.u32(3); // Semantic model wire version.
        w.u32(if self.semantic.is_some() {
            2
        } else {
            u32::from(self.sequence.is_some())
        });
        if let Some(s) = &self.semantic {
            s.write(w);
            return;
        }
        if let Some(s) = &self.sequence {
            s.write(w);
            return;
        }
        w.u32(self.direction);
        w.u32(self.nodes.len() as u32);
        for n in &self.nodes {
            w.text(&n.id);
            w.text(&n.label);
            w.u32(n.shape);
        }
        w.u32(self.edges.len() as u32);
        for e in &self.edges {
            w.u32(e.from);
            w.u32(e.to);
            w.text(&e.label);
            w.u32(e.arrows);
            w.u32(e.style);
        }
        w.u32(self.groups.len() as u32);
        for g in &self.groups {
            w.text(&g.id);
            w.text(&g.label);
            w.u32(g.direction);
            w.u32(g.parent);
            w.u32(g.nodes.len() as u32);
            for n in &g.nodes {
                w.u32(*n);
            }
        }
    }
    pub fn read(r: &mut Reader) -> Result<Self, String> {
        if r.u32()? != 3 {
            return Err("Unsupported diagram model version".into());
        }
        match r.count(2)? {
            2 => {
                return Ok(Self {
                    semantic: Some(semantic::Diagram::read(r)?),
                    sequence: None,
                    direction: 0,
                    nodes: vec![],
                    edges: vec![],
                    groups: vec![],
                });
            }
            1 => {
                return Ok(Self {
                    sequence: Some(sequence::Sequence::read(r)?),
                    semantic: None,
                    direction: 0,
                    nodes: vec![],
                    edges: vec![],
                    groups: vec![],
                });
            }
            _ => {}
        }
        let direction = r.u32()?;
        if direction > 3 {
            return Err("Invalid direction".into());
        }
        let mut nodes = Vec::new();
        for _ in 0..r.count(128)? {
            let id = r.text()?;
            let label = r.text()?;
            let shape = r.u32()?;
            if id.is_empty() || shape > 3 || nodes.iter().any(|n: &Node| n.id == id) {
                return Err("Invalid node".into());
            }
            nodes.push(Node { id, label, shape });
        }
        let mut edges = Vec::new();
        for _ in 0..r.count(512)? {
            let from = r.u32()?;
            let to = r.u32()?;
            let label = r.text()?;
            let arrows = r.u32()?;
            let style = r.u32()?;
            if from as usize >= nodes.len() || to as usize >= nodes.len() || arrows > 3 || style > 2
            {
                return Err("Invalid edge".into());
            }
            edges.push(Edge {
                from,
                to,
                label,
                arrows,
                style,
            });
        }
        let mut groups = Vec::new();
        for i in 0..r.count(32)? {
            let id = r.text()?;
            let label = r.text()?;
            let direction = r.u32()?;
            let parent = r.u32()?;
            if direction > 4 || (parent != u32::MAX && parent >= i) {
                return Err("Invalid group".into());
            }
            let mut members = Vec::new();
            for _ in 0..r.count(128)? {
                let n = r.u32()?;
                if n as usize >= nodes.len() {
                    return Err("Invalid group member".into());
                }
                members.push(n);
            }
            groups.push(Group {
                id,
                label,
                direction,
                parent,
                nodes: members,
            });
        }
        Ok(Model {
            sequence: None,
            semantic: None,
            direction,
            nodes,
            edges,
            groups,
        })
    }
}
