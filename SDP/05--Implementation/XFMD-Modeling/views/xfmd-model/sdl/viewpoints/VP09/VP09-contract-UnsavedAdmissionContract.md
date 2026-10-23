# Contract structure: UnsavedAdmissionContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: UnsavedAdmissionContract

```mermaid
flowchart LR
    n_ChooseCancel["ChooseCancel (message)"]
    n_ChooseSave["ChooseSave (message)"]
    n_ChooseUnsaved["ChooseUnsaved (message)"]
    n_UnsavedAdmissionContract["UnsavedAdmissionContract (contract)"]
    n_UnsavedAdmissionContract -->|permits| n_ChooseCancel
    n_UnsavedAdmissionContract -->|permits| n_ChooseSave
    n_UnsavedAdmissionContract -->|permits| n_ChooseUnsaved
```

Source facts: f0952, f0953, f0954.

