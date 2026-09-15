use std::collections::BTreeSet;
#[derive(Default)]
pub struct Profile {
    pub nodes: BTreeSet<String>,
    pub edges: Vec<(String, String)>,
    pub parents: Vec<u32>,
}
struct Cursor<'a> {
    s: &'a str,
    at: usize,
}
impl<'a> Cursor<'a> {
    fn rest(&self) -> &'a str {
        &self.s[self.at..]
    }
    fn space(&mut self) {
        while self.rest().starts_with(char::is_whitespace) {
            self.at += self.rest().chars().next().unwrap().len_utf8();
        }
    }
    fn take(&mut self, t: &str) -> bool {
        self.space();
        if self.rest().starts_with(t) {
            self.at += t.len();
            true
        } else {
            false
        }
    }
    fn id(&mut self) -> Result<String, String> {
        self.space();
        let start = self.at;
        while let Some(c) = self.rest().chars().next() {
            if c.is_ascii_alphanumeric() || c == '_' || c == '.' {
                self.at += 1
            } else {
                break;
            }
        }
        if self.at == start {
            return Err("Expected node identifier (letters, digits, underscore or dot)".into());
        }
        Ok(self.s[start..self.at].into())
    }
    fn label(&mut self, close: &str) -> Result<(), String> {
        let mut quote = false;
        let mut escaped = false;
        while !self.rest().is_empty() {
            if !quote && self.rest().starts_with(close) {
                self.at += close.len();
                return Ok(());
            }
            let c = self.rest().chars().next().unwrap();
            self.at += c.len_utf8();
            if escaped {
                escaped = false;
                continue;
            }
            if c == '\\' {
                escaped = true;
                continue;
            }
            if c == '"' {
                quote = !quote;
                continue;
            }
            if c == '<' || c == '>' || c == '`' || (!quote && "[]{}()".contains(c)) {
                return Err("Label construct outside XFMD Flowchart 1".into());
            }
        }
        Err("Unclosed diagram label".into())
    }
    fn node(&mut self, p: &mut Profile) -> Result<String, String> {
        let id = self.id()?;
        p.nodes.insert(id.clone());
        if self.take("((") {
            self.label("))")?
        } else if self.take("[") {
            self.label("]")?
        } else if self.take("(") {
            self.label(")")?
        } else if self.take("{") {
            self.label("}")?
        }
        Ok(id)
    }
    fn done(&mut self) -> bool {
        self.space();
        self.rest().is_empty()
    }
}
// Split only outside labels/quotes; comments do not interpret keywords inside labels.
fn statements(s: &str) -> Result<Vec<String>, String> {
    let mut out = Vec::new();
    let mut buf = String::new();
    let mut depth = 0usize;
    let mut quote = false;
    let mut escaped = false;
    let mut comment = false;
    let mut chars = s.chars().peekable();
    while let Some(c) = chars.next() {
        if comment {
            if c != '\n' {
                continue;
            }
            comment = false;
        }
        if escaped {
            buf.push(c);
            escaped = false;
            continue;
        }
        if c == '\\' && quote {
            escaped = true;
            buf.push(c);
            continue;
        }
        if c == '"' {
            quote = !quote;
            buf.push(c);
            continue;
        }
        if !quote {
            if depth == 0 && c == '%' && chars.peek() == Some(&'%') {
                chars.next();
                if chars.peek() == Some(&'{') {
                    return Err("Mermaid init directives are not supported".into());
                }
                comment = true;
                continue;
            }
            if "[({".contains(c) {
                depth += 1;
                if depth > 8 {
                    return Err("Diagram nesting limit exceeded".into());
                }
            }
            if "])}".contains(c) {
                depth = depth.checked_sub(1).ok_or("Unbalanced diagram label")?;
            }
            if depth == 0 && (c == '\n' || c == ';') {
                if !buf.trim().is_empty() {
                    out.push(buf.trim().into())
                }
                buf.clear();
                continue;
            }
        }
        buf.push(c);
    }
    if quote || depth != 0 {
        return Err("Unclosed diagram label".into());
    }
    if !buf.trim().is_empty() {
        out.push(buf.trim().into())
    }
    Ok(out)
}
pub fn inspect(source: &str) -> Result<Profile, String> {
    if source.len() > 65536 {
        return Err("Diagram exceeds 64 KiB".into());
    }
    let statements = statements(source)?;
    let mut profile = Profile::default();
    let mut stack = Vec::new();
    let first = statements.first().ok_or("Missing flowchart header")?;
    let mut header = first.split_whitespace();
    if !matches!(header.next(), Some("flowchart" | "graph"))
        || !matches!(header.next(), Some("LR" | "RL" | "TD" | "TB" | "BT"))
        || header.next().is_some()
    {
        return Err(
            "Expected flowchart/graph and direction; other Mermaid types are not supported".into(),
        );
    }
    for statement in statements.iter().skip(1) {
        let mut c = Cursor {
            s: statement,
            at: 0,
        };
        if statement == "end" {
            stack.pop().ok_or("Unexpected subgraph end")?;
            continue;
        }
        if statement.starts_with("subgraph ") {
            c.at = 9;
            c.id()?;
            if c.take("[") {
                c.label("]")?
            }
            if !c.done() {
                return Err("Unsupported subgraph header; use subgraph ID [label]".into());
            }
            profile
                .parents
                .push(stack.last().copied().unwrap_or(u32::MAX));
            stack.push((profile.parents.len() - 1) as u32);
            if stack.len() > 8 || profile.parents.len() > 32 {
                return Err("Subgraph limit exceeded".into());
            }
            continue;
        }
        if statement.starts_with("direction ") {
            let d = statement[10..].trim();
            if !matches!(d, "LR" | "RL" | "TD" | "TB" | "BT") {
                return Err("Invalid direction".into());
            }
            continue;
        }
        let mut previous = c.node(&mut profile)?;
        while !c.done() {
            let arrow = [
                "<-->", "<-.->", "<==>", "-->", "---", "-.->", "-.-", "==>", "===", "<--",
            ]
            .iter()
            .find(|a| c.rest().starts_with(**a))
            .copied()
            .ok_or("Syntax outside XFMD Flowchart 1 (unsupported directive, shape or edge)")?;
            c.at += arrow.len();
            if c.take("|") {
                c.label("|")?
            }
            let next = c.node(&mut profile)?;
            profile.edges.push((previous, next.clone()));
            previous = next;
        }
    }
    if !stack.is_empty() {
        return Err("Unclosed subgraph".into());
    }
    if profile.nodes.len() > 128 || profile.edges.len() > 512 {
        return Err("Diagram graph limit exceeded (128 nodes / 512 edges)".into());
    }
    Ok(profile)
}
