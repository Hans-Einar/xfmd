#[derive(Default)]
pub struct Writer(pub Vec<u8>);
impl Writer {
    pub fn u32(&mut self, v: u32) {
        self.0.extend(v.to_le_bytes())
    }
    pub fn number(&mut self, v: f64) {
        self.0.extend(v.to_le_bytes())
    }
    pub fn text(&mut self, v: &str) {
        self.u32(v.len() as u32);
        self.0.extend(v.as_bytes())
    }
}
pub struct Reader<'a> {
    data: &'a [u8],
    at: usize,
}
impl<'a> Reader<'a> {
    pub fn new(data: &'a [u8]) -> Self {
        Self { data, at: 0 }
    }
    fn bytes(&mut self, n: usize) -> Result<&'a [u8], String> {
        let end = self.at.checked_add(n).ok_or("Wire overflow")?;
        let out = self
            .data
            .get(self.at..end)
            .ok_or("Truncated diagram data")?;
        self.at = end;
        Ok(out)
    }
    pub fn u32(&mut self) -> Result<u32, String> {
        Ok(u32::from_le_bytes(self.bytes(4)?.try_into().unwrap()))
    }
    pub fn number(&mut self) -> Result<f64, String> {
        let n = f64::from_le_bytes(self.bytes(8)?.try_into().unwrap());
        if !n.is_finite() || n.abs() > 1e7 {
            return Err("Invalid geometry".into());
        }
        Ok(n)
    }
    pub fn count(&mut self, max: u32) -> Result<u32, String> {
        let n = self.u32()?;
        if n > max {
            return Err("Diagram limit exceeded".into());
        }
        Ok(n)
    }
    pub fn text(&mut self) -> Result<String, String> {
        let n = self.count(65536)?;
        Ok(std::str::from_utf8(self.bytes(n as usize)?)
            .map_err(|_| "Invalid UTF-8")?
            .to_owned())
    }
    pub fn finish(&self) -> Result<(), String> {
        if self.at != self.data.len() {
            Err("Trailing diagram data".into())
        } else {
            Ok(())
        }
    }
}
