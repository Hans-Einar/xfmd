# Scenario: PreviewResourcesPrepared — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Scenario: PreviewResourcesPrepared — mode NormalDesktop

```mermaid
sequenceDiagram
    participant n_Preview as Preview
    participant n_Interpreter as Interpreter
    participant n_VisualResources as Visual Resources
    participant n_Renderer as Renderer
    participant n_NativeShell as Native Shell
    n_Preview->>n_Interpreter: 1: Parse Snapshot (Interpretation Calls)
    n_Interpreter-->>n_Preview: 2: Parsed Snapshot (Interpretation Calls) reply-to 1
    n_Preview->>n_VisualResources: 3: Prepare Embedded Resources (Resource Preparation)
    n_Preview->>n_VisualResources: 4: Prepare Diagram Scenes (Resource Preparation)
    n_Preview->>n_Preview: 5: Accept Worker Ticket (Preview Admission)
    n_Preview->>n_Renderer: 6: Layout Model (Layout Calls)
    n_Renderer-->>n_Preview: 7: Model Laid Out (Layout Calls) reply-to 6
    n_Preview->>n_NativeShell: 8: Present Frame (Presentation Events)
```

Source facts: f0004, f0005, f0479, f0481, f0482, f0490, f0491, f0503, f0505, f0506, f0509, f0510, f0541, f0542, f0543, f0559, f0614, f0615, f0616, f0617, f0618, f0669, f0670, f0671, f0672, f0677, f0678, f0679, f0681, f0690, f0691, f0692, f0693, f0694, f0697, f0702, f0705, f0706, f0710, f0714, f0722, f0723, f0724, f0725, f0726, f0727, f0728, f0729, f0730, f0731, f0732, f0851, f0852, f0859, f0862, f0863, f0988, f0989.

