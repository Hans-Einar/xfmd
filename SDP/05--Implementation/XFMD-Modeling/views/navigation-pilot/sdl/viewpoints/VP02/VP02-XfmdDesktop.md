# Logical decomposition: XfmdDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `3002709d67729fd3595ab27c484794d77b675ed28f927db0aec1e23234d288e2`.

## Logical decomposition: XfmdDesktop

```mermaid
flowchart LR
    n_Application["Application (unit)"]
    n_Interpreter["Interpreter (unit)"]
    n_Renderer["Renderer (unit)"]
    n_XfmdDesktop["XfmdDesktop (container)"]
    n_XfmdDesktop -->|contains| n_Application
    n_XfmdDesktop -->|contains| n_Interpreter
    n_XfmdDesktop -->|contains| n_Renderer
```

Source facts: f0051, f0052, f0053.

