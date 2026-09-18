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
