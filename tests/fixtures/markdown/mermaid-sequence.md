# SDL/SDP — sekvensvisninger (kandidateksempler)

Dette er dokumentasjonseksempler, ikke vedtatt SDL eller kjørbar logikk.

## APT-import

Import oppretter et validert utkast; valg av aktiv APT er en separat handling.

```mermaid
sequenceDiagram
    actor Operator as Operatør
    participant UI as Operator UI
    participant Service as APT service
    Operator->>UI: Import APT
    UI->>Service: Command import, expected revision
    activate Service
    alt valid input
        Service-->>UI: Accepted draft revision
    else invalid input
        Service-->>UI: Rejected input
    else revision conflict
        Service-->>UI: Conflict, no overwrite
    end
    deactivate Service
    Note over UI,Service: Import does not activate the draft
    UI-->>Operator: Report outcome
```

## Parallelle aktiviteter og repetisjon

Deltakere beskriver ansvar, ikke implisitte tråder eller scheduling.

```mermaid
sequenceDiagram
    participant UI as Brukergrensesnitt
    participant Service as Måletjeneste
    UI->>Service: Start reading
    opt observation available
        Service-->>UI: Current observation
    end
    UI->>Service: Next operation
    loop bounded retry
        UI->>Service: Request accepted revision with provenance
        Service-->>UI: No newer revision
    end
    UI->>Service: Observe activities
    par sampling
        Service-->>UI: Measurement available
    and configuration
        UI->>Service: Request configuration
    end
    UI->>UI: Preserve local state
    Note right of Service: Åpen dokumentasjon uten implisitt scheduling
```

## Eksisterende flowchart

```mermaid
flowchart LR
    Input[Source artifact] -->|validate| Draft[Draft revision]
```

Vanlig Markdown etter diagrammene kan fortsatt merkes og kopieres.
