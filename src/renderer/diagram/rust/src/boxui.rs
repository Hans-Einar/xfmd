//! Host wire adaptation only; normalized model and prepared child SVG are the inputs.
use mermaid_rs_renderer::boxui::{self, ChildScene, Snapshot, ValueState};
use serde_json::{Value, json};
use std::collections::{BTreeMap, HashSet};
const CONTRACT: &str = "BX-HOST/0.1-draft1";
fn fields(v: &Value, allowed: &[&str]) -> Result<(), String> {
    let object = v.as_object().ok_or("Expected object")?;
    if object.keys().any(|k| !allowed.contains(&k.as_str())) {
        return Err("Unknown BoxUI field".into());
    }
    Ok(())
}
fn string<'a>(v: &'a Value, name: &str) -> Result<&'a str, String> {
    v.get(name)
        .and_then(Value::as_str)
        .ok_or_else(|| format!("Missing string {name}"))
}
pub fn prepare(
    bytes: &[u8],
    measure: &dyn Fn(&str) -> (f64, f64),
    cancel: &dyn Fn() -> bool,
) -> Result<Vec<u8>, String> {
    if cancel() {
        return Err("BoxUI cancelled".into());
    }
    let v: Value = serde_json::from_slice(bytes).map_err(|e| e.to_string())?;
    fields(
        &v,
        &[
            "contract",
            "key",
            "model",
            "snapshot",
            "viewport",
            "palette",
            "fontSignature",
            "budgetMs",
            "childProfiles",
            "children",
        ],
    )?;
    if string(&v, "contract")? != CONTRACT {
        return Err("Unsupported BoxUI contract".into());
    }
    let model: boxui::Document =
        serde_json::from_value(v["model"].clone()).map_err(|e| e.to_string())?;
    boxui::validate(&model).map_err(|e| e.to_string())?;
    let s = &v["snapshot"];
    fields(s, &["contextRevision", "values", "commands"])?;
    string(s, "contextRevision")?;
    let mut snapshot = Snapshot::default();
    let mut seen = HashSet::new();
    for value in s["values"].as_array().ok_or("Expected values")? {
        fields(
            value,
            &[
                "id",
                "type",
                "value",
                "validity",
                "revision",
                "sourceSession",
            ],
        )?;
        let id = string(value, "id")?;
        let binding = model
            .bindings
            .iter()
            .find(|b| b.id == id && b.role == "value")
            .ok_or("Unknown value")?;
        if !seen.insert(id) || string(value, "type")? != binding.r#type {
            return Err("Duplicate or mistyped value".into());
        }
        string(value, "sourceSession")?;
        snapshot.values.insert(
            id.into(),
            ValueState {
                value: value["value"].clone(),
                validity: string(value, "validity")?.into(),
                revision: string(value, "revision")?.into(),
            },
        );
    }
    for command in s["commands"].as_array().ok_or("Expected commands")? {
        fields(
            command,
            &["id", "argumentType", "enabled", "reason", "provenance"],
        )?;
        let id = string(command, "id")?;
        let binding = model
            .bindings
            .iter()
            .find(|b| b.id == id && b.role == "command")
            .ok_or("Unknown command")?;
        if !seen.insert(id) || string(command, "argumentType")? != binding.r#type {
            return Err("Duplicate or mistyped command".into());
        }
        let enabled = command["enabled"].as_bool().ok_or("Missing enabled")?;
        let provenance = string(command, "provenance")?;
        if !["unbound", "simulated", "native"].contains(&provenance)
            || (enabled && provenance == "unbound")
        {
            return Err("Invalid command provenance".into());
        }
        snapshot.simulated |= provenance == "simulated";
        snapshot.enabled.insert(id.into(), enabled);
    }
    if seen.len() != model.bindings.len() {
        return Err("Incomplete snapshot".into());
    }
    let mut refs = HashSet::new();
    fn child_refs<'a>(n: &'a boxui::Node, refs: &mut HashSet<&'a str>) {
        if let Some(r) = &n.child_ref {
            refs.insert(r);
        }
        for c in &n.children {
            child_refs(c, refs);
        }
    }
    child_refs(&model.root, &mut refs);
    let mut children = BTreeMap::new();
    for child in v["children"].as_array().ok_or("Expected children")? {
        fields(child, &["ref", "error", "width", "height", "svg"])?;
        let id = string(child, "ref")?;
        if !refs.remove(id) {
            return Err("Duplicate/unknown child".into());
        }
        let scene = if let Some(error) = child.get("error") {
            if child.get("svg").is_some() {
                return Err("Child error and SVG".into());
            }
            ChildScene {
                width: 0.,
                height: 0.,
                svg: String::new(),
                error: error.as_str().ok_or("Invalid child error")?.into(),
            }
        } else {
            ChildScene {
                width: child["width"].as_f64().ok_or("Child width")?,
                height: child["height"].as_f64().ok_or("Child height")?,
                svg: string(child, "svg")?.into(),
                error: String::new(),
            }
        };
        children.insert(id.into(), scene);
    }
    if !refs.is_empty() {
        return Err("Missing prepared child".into());
    }
    let frame = boxui::prepare(
        &model,
        &snapshot,
        &children,
        v["viewport"]["width"].as_f64().ok_or("Viewport width")?,
        v["viewport"]["height"].as_f64().ok_or("Viewport height")?,
        v["budgetMs"].as_u64().ok_or("Budget")?,
        measure,
        cancel,
    )
    .map_err(|e| e.to_string())?;
    let diagnostics: Vec<Value> = frame
        .diagnostics
        .iter()
        .map(|e| json!({"code":"child-error","severity":"warning","message":e}))
        .collect();
    let out = json!({"contract":CONTRACT,"key":v["key"],"width":frame.width,"height":frame.height,
      "staticSvg":frame.static_svg,"previewSvg":frame.preview_svg,"controls":frame.controls,"diagnostics":diagnostics});
    let bytes = serde_json::to_vec(&out).map_err(|e| e.to_string())?;
    if bytes.len() > 8 * 1024 * 1024 {
        return Err("BoxUI frame exceeds 8 MiB".into());
    }
    Ok(bytes)
}
