# Contract structure: GeneratedAdmissionContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: GeneratedAdmissionContract

```mermaid
flowchart LR
    n_AdvanceGenerationSequence["AdvanceGenerationSequence (message)"]
    n_GeneratedAdmissionContract["GeneratedAdmissionContract (contract)"]
    n_InspectDirtyMain["InspectDirtyMain (message)"]
    n_RejectInvalidSelection["RejectInvalidSelection (message)"]
    n_ValidateSelection["ValidateSelection (message)"]
    n_GeneratedAdmissionContract -->|permits| n_AdvanceGenerationSequence
    n_GeneratedAdmissionContract -->|permits| n_InspectDirtyMain
    n_GeneratedAdmissionContract -->|permits| n_RejectInvalidSelection
    n_GeneratedAdmissionContract -->|permits| n_ValidateSelection
```

Source facts: f0369, f0370, f0371, f0372.

