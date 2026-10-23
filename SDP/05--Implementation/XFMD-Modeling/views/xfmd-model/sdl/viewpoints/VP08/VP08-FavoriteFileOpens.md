# Scenario: FavoriteFileOpens — mode NormalDesktop

[Viewpoint](index.md) · [Navigator](../../navigator.md)

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

## Scenario: FavoriteFileOpens — mode NormalDesktop

```mermaid
sequenceDiagram
    participant n_NativeShell as Native Shell
    participant n_Application as Application
    participant n_Navigation as Navigation
    participant n_Documents as Documents
    participant n_DocumentStorage as Document Storage
    n_NativeShell->>n_Application: 1: Favorite File Chosen (Workspace Intents)
    n_Application->>n_Navigation: 2: Open Target (Navigation Calls)
    n_Navigation->>n_Documents: 3: Admit Document (Document Calls)
    n_Documents->>n_DocumentStorage: 4: Read Candidate (Storage Calls)
    n_DocumentStorage-->>n_Documents: 5: Candidate Read (Storage Calls) reply-to 4
    n_Documents->>n_Documents: 6: Replace Document (Document State Changes)
    n_Documents->>n_Navigation: 7: Commit Visit (Document Notifications)
    n_Documents-->>n_Navigation: 8: Document Opened (Document Calls) reply-to 3
    n_Navigation-->>n_Application: 9: Target Opened (Navigation Calls) reply-to 2
```

Source facts: f0010, f0011, f0036, f0037, f0044, f0067, f0068, f0069, f0102, f0103, f0167, f0169, f0170, f0191, f0193, f0196, f0197, f0198, f0212, f0215, f0225, f0227, f0234, f0235, f0237, f0241, f0243, f0249, f0252, f0327, f0328, f0329, f0330, f0331, f0332, f0333, f0334, f0335, f0336, f0337, f0338, f0339, f0340, f0574, f0577, f0579, f0580, f0581, f0582, f0583, f0585, f0586, f0595, f0596, f0782, f0783, f0853, f0854, f0921, f0923, f0926, f0929, f0930, f0931, f1023, f1027.

