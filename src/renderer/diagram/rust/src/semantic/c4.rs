use super::*;
pub fn populate(d: &Diagram, g: &mut Graph) {
    g.c4.boundaries.push(C4Boundary { id:"global".into(), label:"global".into(), boundary_type:"global".into(), descr:None,sprite:None,tags:None,link:None,parent_boundary:String::new(),bg_color:None,border_color:None,font_color:None });
    let theme = mermaid_rs_renderer::Theme::modern();
    for r in &d.records {
        let f = &r.fields;
        match r.tag {
            50 => g.c4.c4_type = Some(f[0].clone()),
            51 => {
                let kind = match f[2].as_str() {
                    "Person" => C4ShapeKind::Person,
                    "Person_Ext" => C4ShapeKind::ExternalPerson,
                    "System" => C4ShapeKind::System,
                    "SystemDb" => C4ShapeKind::SystemDb,
                    "SystemQueue" => C4ShapeKind::SystemQueue,
                    "System_Ext" => C4ShapeKind::ExternalSystem,
                    "SystemDb_Ext" => C4ShapeKind::ExternalSystemDb,
                    "SystemQueue_Ext" => C4ShapeKind::ExternalSystemQueue,
                    "Container" => C4ShapeKind::Container,
                    "ContainerDb" => C4ShapeKind::ContainerDb,
                    "ContainerQueue" => C4ShapeKind::ContainerQueue,
                    "Container_Ext" => C4ShapeKind::ExternalContainer,
                    "ContainerDb_Ext" => C4ShapeKind::ExternalContainerDb,
                    "ContainerQueue_Ext" => C4ShapeKind::ExternalContainerQueue,
                    "Component" => C4ShapeKind::Component,
                    "ComponentDb" => C4ShapeKind::ComponentDb,
                    "ComponentQueue" => C4ShapeKind::ComponentQueue,
                    "Component_Ext" => C4ShapeKind::ExternalComponent,
                    "ComponentDb_Ext" => C4ShapeKind::ExternalComponentDb,
                    _ => C4ShapeKind::ExternalComponentQueue,
                };
                g.c4.shapes.push(C4Shape {
                    id: format!("c4_{}", f[0]),
                    label: f[1].clone(),
                    type_label: Some(f[2].replace("_Ext", " external")),
                    techn: optional(&f[3]),
                    descr: optional(&f[4]),
                    sprite: None,
                    tags: None,
                    link: None,
                    parent_boundary: parent(&f[5]),
                    kind,
                    bg_color: Some(theme.primary_color.clone()),
                    border_color: Some(theme.primary_border_color.clone()),
                    font_color: Some(theme.primary_text_color.clone()),
                });
            }
            52 => g.c4.boundaries.push(C4Boundary {
                id: format!("c4_{}", f[0]),
                label: f[1].clone(),
                boundary_type: f[2].clone(),
                descr: None,
                sprite: None,
                tags: None,
                link: None,
                parent_boundary: parent(&f[3]),
                bg_color: Some(theme.background.clone()),
                border_color: Some(theme.primary_border_color.clone()),
                font_color: Some(theme.primary_text_color.clone()),
            }),
            53 => g.c4.rels.push(C4Rel {
                kind: match f[2].as_str() {
                    "BiRel" => C4RelKind::BiRel,
                    "Rel_U" | "Rel_Up" => C4RelKind::RelUp,
                    "Rel_D" | "Rel_Down" => C4RelKind::RelDown,
                    "Rel_L" | "Rel_Left" => C4RelKind::RelLeft,
                    "Rel_R" | "Rel_Right" => C4RelKind::RelRight,
                    "Rel_Back" => C4RelKind::RelBack,
                    _ => C4RelKind::Rel,
                },
                from: format!("c4_{}", f[0]),
                to: format!("c4_{}", f[1]),
                label: f[3].clone(),
                techn: optional(&f[4]),
                descr: None,
                sprite: None,
                tags: None,
                link: None,
                offset_x: 0.,
                offset_y: 0.,
                line_color: Some(theme.line_color.clone()),
                text_color: Some(theme.primary_text_color.clone()),
            }),
            _ => {}
        }
    }
}
fn optional(s: &str) -> Option<String> {
    (!s.is_empty()).then(|| s.into())
}

fn parent(s:&str)->String {if s.is_empty(){"global".into()}else{format!("c4_{s}")}}
