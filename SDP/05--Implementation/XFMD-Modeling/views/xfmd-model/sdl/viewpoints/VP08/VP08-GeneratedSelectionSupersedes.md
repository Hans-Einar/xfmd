# Scenario: GeneratedSelectionSupersedes — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Scenario: GeneratedSelectionSupersedes — mode NormalDesktop

```mermaid
sequenceDiagram
    participant n_NativeShell as Native Shell
    participant n_GeneratedViews as Generated Views
    participant n_ExternalViewGenerator as External View Generator
    n_NativeShell->>n_GeneratedViews: 1: Select Registered View (Generated Intents)
    n_GeneratedViews->>n_GeneratedViews: 2: Validate Selection (Generated Admission)
    n_GeneratedViews->>n_ExternalViewGenerator: 3: Cancel Previous Child (Generation Lifecycle)
    n_GeneratedViews->>n_GeneratedViews: 4: Advance Generation Sequence (Generated Admission)
    n_GeneratedViews->>n_ExternalViewGenerator: 5: Start Generation (Generation Lifecycle)
```

Source facts: f0016, f0017, f0063, f0064, f0325, f0326, f0367, f0369, f0372, f0389, f0392, f0411, f0412, f0413, f0414, f0415, f0416, f0417, f0418, f0426, f0428, f0429, f0432, f0439, f0440, f0441, f0453, f0455, f0456, f0554, f0899, f0900, f0919, f0920, f0961, f0962.

