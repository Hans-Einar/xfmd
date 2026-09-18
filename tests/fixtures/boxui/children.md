# Embedded diagram families

```boxui
boxui 0.1
{
  "profile": "boxui/0.1",
  "documentId": "child-0",
  "bindings": [],
  "root": {
    "id": "root",
    "kind": "column",
    "version": 1,
    "children": [
      {
        "id": "diagram",
        "kind": "diagram",
        "version": 1,
        "label": "Child diagram — Æøå",
        "family": "flowchart",
        "source": "flowchart LR\nA[Blåbær] --> B[Ready]"
      }
    ]
  }
}
```

Surrounding Markdown.

```boxui
boxui 0.1
{
  "profile": "boxui/0.1",
  "documentId": "child-1",
  "bindings": [],
  "root": {
    "id": "root",
    "kind": "column",
    "version": 1,
    "children": [
      {
        "id": "diagram",
        "kind": "diagram",
        "version": 1,
        "label": "Child diagram — Æøå",
        "family": "sequenceDiagram",
        "source": "sequenceDiagram\nparticipant A\nparticipant B\nA->>B: Command"
      }
    ]
  }
}
```

Surrounding Markdown.

```boxui
boxui 0.1
{
  "profile": "boxui/0.1",
  "documentId": "child-2",
  "bindings": [],
  "root": {
    "id": "root",
    "kind": "column",
    "version": 1,
    "children": [
      {
        "id": "diagram",
        "kind": "diagram",
        "version": 1,
        "label": "Child diagram — Æøå",
        "family": "stateDiagram-v2",
        "source": "stateDiagram-v2\n[*] --> Current\nCurrent --> Stale: age"
      }
    ]
  }
}
```

Surrounding Markdown.

