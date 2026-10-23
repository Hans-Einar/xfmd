# Contract structure: DocumentStateShape

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: DocumentStateShape

```mermaid
flowchart LR
    n_DocumentBytes["DocumentBytes (field)"]
    n_DocumentIdentity["DocumentIdentity (field)"]
    n_DocumentPath["DocumentPath (field)"]
    n_DocumentRevision["DocumentRevision (field)"]
    n_DocumentStateShape["DocumentStateShape (contract)"]
    n_SavedBaselineBytes["SavedBaselineBytes (field)"]
    n_DocumentStateShape -->|has-field| n_DocumentBytes
    n_DocumentStateShape -->|has-field| n_DocumentIdentity
    n_DocumentStateShape -->|has-field| n_DocumentPath
    n_DocumentStateShape -->|has-field| n_DocumentRevision
    n_DocumentStateShape -->|has-field| n_SavedBaselineBytes
```

Source facts: f0217, f0218, f0219, f0220, f0221.

