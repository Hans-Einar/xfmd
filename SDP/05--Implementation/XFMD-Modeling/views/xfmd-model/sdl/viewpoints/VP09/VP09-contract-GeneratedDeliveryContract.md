# Contract structure: GeneratedDeliveryContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: GeneratedDeliveryContract

```mermaid
flowchart LR
    n_DeliverGeneratedMain["DeliverGeneratedMain (message)"]
    n_GeneratedDeliveryContract["GeneratedDeliveryContract (contract)"]
    n_GeneratedMainAccepted["GeneratedMainAccepted (message)"]
    n_GeneratedMainRejected["GeneratedMainRejected (message)"]
    n_GeneratedDeliveryContract -->|permits| n_DeliverGeneratedMain
    n_GeneratedDeliveryContract -->|permits| n_GeneratedMainAccepted
    n_GeneratedDeliveryContract -->|permits| n_GeneratedMainRejected
```

Source facts: f0375, f0376, f0377.

