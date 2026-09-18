pub fn parse(bytes: &[u8]) -> Result<Vec<u8>, String> {
    let parsed = mermaid_rs_renderer::boxui::parse_boxui_bytes(bytes)
        .map_err(|e| serde_json::to_string(&e.diagnostic).unwrap())?;
    serde_json::to_vec(&parsed).map_err(|e| e.to_string())
}
