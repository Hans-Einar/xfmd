# Contract structure: ReferenceInterpretationContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: ReferenceInterpretationContract

```mermaid
flowchart LR
    n_ParseReference["ParseReference (message)"]
    n_ReferenceInterpretationContract["ReferenceInterpretationContract (contract)"]
    n_ReferenceParsed["ReferenceParsed (message)"]
    n_ReferenceInterpretationContract -->|permits| n_ParseReference
    n_ReferenceInterpretationContract -->|permits| n_ReferenceParsed
```

Source facts: f0809, f0810.

