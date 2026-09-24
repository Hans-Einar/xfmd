# Blueprint templates

Use the [feature template](Feature-TEMPLATE.md) for a coherent capability and the
[functionality template](Functionality-TEMPLATE.md) for a bounded service. Both use
the same eight English headings; chapter 5 is always **Plumbing**. The validator
also accepts the complete legacy Norwegian heading set for untouched documents.
Mixed, missing, reordered or empty headings are invalid. Write revised objects in English.

1. Reserve the next ID in the [register](../README.md); preserve it on rename.
2. Copy to feature/Feature-NNN--English-Slug.md or functionality/Functionality-NNN--English-Slug.md.
3. Fill every metadata field. Use comma-separated IDs; uses names consumed
   functionality IDs or none. Metadata is not a runtime class or directory rule.
4. State requirements and concrete calls/source files; future symbols remain Planned.
5. Cover normal flow, cancellation/failure, ownership and actual/planned consumers.
6. Update the requirement matrix and run both blueprint checks from the repository root.

Metadata values: kind Feature/Functionality; audience User/System/Integration;
role Workflow/Service/Adapter/Mechanism; owner application/interpreter/renderer;
status Proposed/Ready/Implemented/Verified/Retired; scope FirstRelease/Future.
See the [working method](../../../docs/working-method.md) for status gates.
Paths in code spans may be planned; Markdown file links must exist. Planned tests
are never presented as executed evidence.
