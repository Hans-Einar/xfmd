# Native use-case diagrams

Session0005 N3c adds a bounded `usecase-beta` profile to the XFMD working build.
The [official syntax](https://mermaid.js.org/syntax/usecase.html) defines the language;
this page lists the subset implemented here. No browser or Mermaid JavaScript is
involved. The existing installation is unchanged until a release is selected.

```mermaid
usecase-beta
direction LR
actor Reader("Reader")
systemBoundary Viewer["Viewer"]
  Browse("Browse project")
  Read("Read source")
end
Reader -- Browse
Browse ..> : include Read
```

Actors use stick figures. Parenthesized use cases use ellipses; square brackets
select rectangles. System boundaries use `systemBoundary ID["Label"]`, declarations
and `end`. Only one level is supported; boundaries must contain an element.
Multiple independent boundaries are allowed. Use explicit ASCII identifiers
(letters, digits and underscore, excluding the reserved double-underscore prefix).
Unicode is supported in plain labels. Whole-line `%%` comments are allowed.

Relations are top-level, one per line, with whitespace-separated IDs/operators:

| Syntax | Meaning |
| --- | --- |
| `A -- B` | Undirected association |
| `A --> B` / `A <-- B` | Association with the indicated direction |
| `A -- "caption" --> B` / `A -- "caption" -- B` | Labelled association |
| `A ..> : include B` | A includes B; both endpoints must be use cases |
| `A ..> : extend B` | A extends base B; both endpoints must be use cases |
| `A --\|> B` | A specializes B; both actors or both use cases |

An association caption containing include/extend remains an association. Forward
references are resolved after declarations; undeclared endpoints become ellipse
use cases. Actors require explicit declarations. Isolated declarations remain
visible. Source declaration order does not change endpoint kinds.

Unsupported syntax is rejected visibly: styles, metadata, icons, business variants,
notes, JSON nodes, inline endpoint declarations, quoted-only generated identifiers,
nested boundaries and additional association markers. Labels follow XFMD's existing
plain-text restrictions; no HTML, entities, embedded quotes or multiline Markdown.
A label consisting only of `---` is rejected because the shared native text renderer
reserves that line as a compartment divider.
Use supported statements rather than expecting unsupported suffixes to be ignored.
Existing source/model/layout resource bounds and cooperative deadlines remain.

The fork owns shapes, SVG and UML arrowheads. All use-case diagrams use the
semantic Libavoid path, including system boundaries. Actor ports attach to visible
strokes; captions are separate routing obstacles. Group placement keeps outsiders
outside system boundaries. This is not a guarantee of optimal placement. Wide LR diagrams may have small
text at fit-to-page scale; explicit TB direction can be more readable. Preview and PDF
use the same SVG. [Profile/contract](../../SDP/04--Design/SDL-Navigation/UseCase-Profile.md)
records ownership, semantic fields and producer handoff. SDL VP01 aggregate
selection remains separate work; support here does not generate absent model facts.
