//! Host adapter to the separately implemented BoxUI producer. No source parsing here.
use mermaid_rs_renderer::boxui::{self, TextExtent, TextMetrics};
struct Metrics<'a>(&'a dyn Fn(&str) -> (f64, f64, f64));
impl TextMetrics for Metrics<'_> {
    fn measure(&self, text: &str) -> TextExtent {
        let (width, height, baseline) = (self.0)(text);
        TextExtent {
            width,
            height,
            baseline,
        }
    }
    fn font_family(&self) -> &str {
        "DejaVu Sans"
    }
    fn font_size(&self) -> f64 {
        16.
    }
}
pub fn prepare(
    bytes: &[u8],
    measure: &dyn Fn(&str) -> (f64, f64, f64),
    cancel: &dyn Fn() -> bool,
) -> Result<Vec<u8>, String> {
    let diagnostic = |e: boxui::BoxUiError| serde_json::to_string(&e.diagnostic).unwrap();
    let request = boxui::decode_prepare_json(bytes).map_err(diagnostic)?;
    let frame = boxui::prepare_boxui_cancellable(&request, &Metrics(measure), cancel)
        .map_err(diagnostic)?;
    serde_json::to_vec(&frame).map_err(|e| e.to_string())
}
