use serde_json::json;
pub fn parse(bytes: &[u8]) -> Result<Vec<u8>, String> {
    let source = std::str::from_utf8(bytes).map_err(|e| e.to_string())?;
    let parsed = mermaid_rs_renderer::boxui::parse(source).map_err(|e| e.to_string())?;
    serde_json::to_vec(
        &json!({"contract":"BX-HOST/0.1-draft1", "model":parsed.model,
      "childSources":parsed.child_sources,"diagnostics":[]}),
    )
    .map_err(|e| e.to_string())
}
