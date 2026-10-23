# Use case: ConfigurePreferences

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Use case: ConfigurePreferences

```mermaid
flowchart LR
    n_ApplicationPreferences["ApplicationPreferences (functionality)"]
    n_ConfigurableScrolling["ConfigurableScrolling (feature)"]
    n_ConfigurePreferences["ConfigurePreferences (usecase)"]
    n_ReaderAuthor["ReaderAuthor (actor)"]
    n_ApplicationPreferences -->|contributes-to| n_ConfigurePreferences
    n_ConfigurableScrolling -->|supports| n_ConfigurePreferences
    n_ReaderAuthor -->|pursues| n_ConfigurePreferences
```

Source facts: f0048, f0106, f0788.

