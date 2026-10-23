# Contract structure: InterpretationCallsContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: InterpretationCallsContract

```mermaid
flowchart LR
    n_InterpretationCallsContract["InterpretationCallsContract (contract)"]
    n_ParseSnapshot["ParseSnapshot (message)"]
    n_ParsedSnapshot["ParsedSnapshot (message)"]
    n_InterpretationCallsContract -->|permits| n_ParseSnapshot
    n_InterpretationCallsContract -->|permits| n_ParsedSnapshot
```

Source facts: f0481, f0482.

