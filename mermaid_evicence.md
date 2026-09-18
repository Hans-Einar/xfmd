# Mermaid — praktiske eksempler i XFMD

Dette dokumentet oppdateres etter hvert implementert inkrement. Eksemplene
er forklarende SDL/SDP-kandidater, ikke vedtatt språk eller bevis for beståtte tester.
Start fra build-mappen med `./xfmd ../mermaid_evicence.md`.

## Flowchart — ansvar og krav

```mermaid
flowchart LR
    UC[APT import] -->|motivates| R[Preserve active selection]
    F[Import responsibility] -->|satisfies| R
    F -->|produces| D[Validated draft]
```

## Sequence 1 — APT-import

Import oppretter et utkast, men aktiverer det ikke. Aktiveringsbaren beskriver
aktivitet hos tjenesten, ikke aktivering av APT-utkastet eller en implisitt tråd.

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

## Sequence 1 — valg og samtidige aktiviteter

Parallelle fragmenter fastsetter ingen konkret scheduling.

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

## Sekvens: nestede forløp og asynkrone meldinger

Import er fortsatt forskjellig fra aktivering. Piler angir meldingsform, ikke scheduling.

```mermaid
sequenceDiagram
    actor Operator as Operatør
    participant UI as Operator UI
    participant Service as APT service
    participant Store as Draft store
    Operator->>UI: Import APT
    UI-)Service: Command with expected revision
    activate Service
    alt input valid
        Note over Service,Store: Import and activation are distinct
        opt draft changed
            Service->>Store: Read accepted revision
            Store-->>Service: Revision and provenance
        end
        alt revision matches
            Service->>Store: Commit draft
            Store-->>Service: Accepted
            Service--)UI: Draft imported
        else revision conflict
            Service--)UI: Conflict, no overwrite
        end
    else invalid input
        Service--)UI: Rejected input
    end
    deactivate Service
    par user feedback
        UI-->>Operator: Report import outcome
    and background observations
        loop bounded observation window
            Store-)UI: Datagram with revision
            Note right of UI: Late data does not restore currentness
        end
    end
```

## State — målingens aktualitet

En sen observasjon beholder Stale; source-session-reset er en egen overgang. Vakter evalueres ikke.

```mermaid
stateDiagram-v2
    direction LR
    [*] --> Missing
    Missing --> Current : accepted observation
    Current --> Stale : freshness window expires
    Stale --> Stale : late observation in same session
    Stale --> Current : newer accepted revision
    Current --> Missing : source session reset
    Stale --> Missing : source session reset
    Missing --> [*] : session closed
```

## State — samtidige regioner

To regioner beskriver logiske tilstander, ikke automatisk to tråder.

```mermaid
stateDiagram-v2
    [*] --> Session
    state Session {
        [*] --> Missing
        Missing --> Current : observation accepted
        Current --> Stale : expires
        --
        [*] --> Connected
        Connected --> Reconnecting : link lost
        Reconnecting --> Connected : session restored
    }
    Session --> [*] : reset
```

## State — choice, fork og join

Forgrening og samling vises eksplisitt.

```mermaid
stateDiagram-v2
    state Valid <<choice>>
    state Fanout <<fork>>
    state Joined <<join>>
    [*] --> Valid
    Valid --> Rejected : invalid
    Valid --> Fanout : valid
    Fanout --> Persist
    Fanout --> Notify
    Persist --> Joined
    Notify --> Joined
    Joined --> [*]
    Rejected --> [*]
```

## Class — kandidatmetamodell

Eierskap, bruk og realisering er forskjellige relasjoner. Dette vedtar ingen SDL-regler.

```mermaid
classDiagram
    direction TB
    class Unit {
        <<design constituent>>
        +String identity
        +provideCapability()
    }
    class Container
    class Functionality
    class Function
    class Dataset
    class Datagram
    class Command
    class Value
    Container --|> Unit : specializes
    Unit "1" *-- "0..*" Functionality : owns
    Functionality "1" o-- "1..*" Function : groups
    Function ..|> Functionality : realizes
    Function ..> Dataset : reads
    Datagram --> Dataset : describes occurrence of
    Command ..> Functionality : requests
    Dataset "1" *-- "1..*" Value : contains
```

## Requirement — APT-import

Verifies kobler testspesifikasjon til krav; diagrammet hevder ikke at testen har bestått.

```mermaid
requirementDiagram
    requirement ImportAPT {
        id: APT-001
        text: "Import validates input and expected revision"
        risk: high
        verifymethod: test
    }
    functionalRequirement RejectConflict {
        id: APT-002
        text: "Reject stale expected revision without overwriting"
        risk: high
        verifymethod: test
    }
    element ImportService {
        type: service responsibility
        docref: design proposal
    }
    element ConflictTest {
        type: test specification
        docref: acceptance example
    }
    ImportService - satisfies -> ImportAPT
    ConflictTest - verifies -> RejectConflict
    RejectConflict - refines -> ImportAPT
    ImportService - traces -> RejectConflict
```

## ER — proveniens

Dataset-instans og Datagram-forekomst har ulike identiteter og er ikke nødvendigvis én til én.

```mermaid
erDiagram
    SOURCE_ARTIFACT {
        string artifact_id PK
        string checksum
    }
    DATASET_INSTANCE {
        string dataset_id PK
        string artifact_id FK
    }
    ACCEPTED_REVISION {
        string revision_id PK
        string dataset_id FK
    }
    DATAGRAM_OCCURRENCE {
        string occurrence_id PK
        string revision_id FK
        string session_id
    }
    SOURCE_ARTIFACT ||--o{ DATASET_INSTANCE : originates
    DATASET_INSTANCE ||--|{ ACCEPTED_REVISION : has
    ACCEPTED_REVISION ||..o{ DATAGRAM_OCCURRENCE : reported_by
```
