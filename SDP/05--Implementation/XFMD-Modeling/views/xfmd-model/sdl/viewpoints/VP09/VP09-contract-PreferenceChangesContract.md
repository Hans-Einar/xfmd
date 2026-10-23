# Contract structure: PreferenceChangesContract

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Contract structure: PreferenceChangesContract

```mermaid
flowchart LR
    n_AdoptPreferencesDraft["AdoptPreferencesDraft (message)"]
    n_BeginPreferencesDraft["BeginPreferencesDraft (message)"]
    n_NotifyPreferenceChange["NotifyPreferenceChange (message)"]
    n_PersistPreferencesDraft["PersistPreferencesDraft (message)"]
    n_PreferenceChangesContract["PreferenceChangesContract (contract)"]
    n_ValidatePreferencesDraft["ValidatePreferencesDraft (message)"]
    n_PreferenceChangesContract -->|permits| n_AdoptPreferencesDraft
    n_PreferenceChangesContract -->|permits| n_BeginPreferencesDraft
    n_PreferenceChangesContract -->|permits| n_NotifyPreferenceChange
    n_PreferenceChangesContract -->|permits| n_PersistPreferencesDraft
    n_PreferenceChangesContract -->|permits| n_ValidatePreferencesDraft
```

Source facts: f0646, f0647, f0648, f0649, f0650.

