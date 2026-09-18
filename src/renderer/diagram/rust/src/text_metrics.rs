//! Synchronous borrowed C metrics; no callback or context outlives layout().
use mermaid_rs_renderer::layout::TextBlock;
use std::ffi::c_void;
pub type Callback =
    unsafe extern "C" fn(*mut c_void, *const u8, u64, f64, *mut f64, *mut f64) -> u32;
#[derive(Clone, Copy)]
pub struct Metrics {
    pub context: *mut c_void,
    pub callback: Callback,
}
pub struct Context {
    pub metrics: Metrics,
    pub no_wrap: std::collections::HashSet<String>,
}
pub fn measure(context: usize, text: &str, size: f32, max_width: f32, wrap: bool) -> TextBlock {
    // The scoped call owns this stack value throughout synchronous layout.
    let context = unsafe { &*(context as *const Context) };
    let metrics = &context.metrics;
    let wrap = wrap && !context.no_wrap.contains(text);
    let line_metrics = |s: &str| {
        let (mut width, mut height) = (0., 0.);
        let status = unsafe {
            (metrics.callback)(
                metrics.context,
                s.as_ptr(),
                s.len() as u64,
                size as f64,
                &mut width,
                &mut height,
            )
        };
        assert!(
            status == 0
                && width.is_finite()
                && height.is_finite()
                && width >= 0.
                && height > 0.
                && width < 1e7
                && height < 1e7,
            "Invalid external text metrics"
        );
        (width as f32, height as f32)
    };
    let mut lines = vec![];
    for line in text.split('\n') {
        if wrap && max_width > 0. && line_metrics(line).0 > max_width {
            let mut current = String::new();
            for word in line.split_whitespace() {
                let candidate = if current.is_empty() {
                    word.into()
                } else {
                    format!("{current} {word}")
                };
                if !current.is_empty() && line_metrics(&candidate).0 > max_width {
                    lines.push(std::mem::take(&mut current));
                    current = word.into();
                } else {
                    current = candidate;
                }
            }
            lines.push(current);
        } else {
            lines.push(line.into());
        }
    }
    let (mut width, mut height) = (0f32, 0f32);
    for line in &lines {
        let (w, h) = line_metrics(line);
        width = width.max(w);
        height += h;
    }
    TextBlock {
        lines,
        width,
        height,
    }
}
