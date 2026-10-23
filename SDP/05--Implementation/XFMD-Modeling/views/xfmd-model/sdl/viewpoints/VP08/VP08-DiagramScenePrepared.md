# Scenario: DiagramScenePrepared — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Scenario: DiagramScenePrepared — mode NormalDesktop

```mermaid
sequenceDiagram
    participant n_Interpreter as Interpreter
    participant n_Preview as Preview
    participant n_VisualResources as Visual Resources
    participant n_Renderer as Renderer
    n_Interpreter->>n_Interpreter: 1: Parse Mermaid Block (Diagram Interpretation)
    n_Preview->>n_VisualResources: 2: Prepare Typed Diagram (Resource Preparation)
    n_VisualResources->>n_Renderer: 3: Layout Typed Diagram (Diagram Layout Calls)
    n_Renderer-->>n_VisualResources: 4: Typed Diagram Laid Out (Diagram Layout Calls) reply-to 3
    n_VisualResources->>n_VisualResources: 5: Cache Compatible Scene (Resource Preparation)
```

Source facts: f0059, f0060, f0126, f0128, f0131, f0133, f0134, f0137, f0138, f0139, f0140, f0141, f0142, f0143, f0144, f0486, f0487, f0511, f0512, f0610, f0611, f0675, f0676, f0707, f0847, f0848, f0859, f0861, f0865, f0941, f0942, f0943, f0985, f0986, f0987, f0991, f0992.

