# Standalone Mermaid source inspection — 2026-10-03

Owner-supplied source: `/home/warloc/git/SDP-vNow/SDP/04--Design/SDPTool/ModelGovernance/review/diagrams/VP01-feature-ModelGovernance.mmd`.
SHA-256: `5041a9b585dee4cdcd9b0472556948bfc8f4cb2d1257689f22a50e1ef1316a63`. The adjacent .mmd file is a verbatim evidence copy,
not a generated replacement or an edit to the upstream source.

XFMD inspected HEAD: 41fa4948c5377edd792ba3f503e6b92b0fd4717a plus shared dirty
working tree. Installed CLI reports `xfmd 0.4 sprint/009/phase/057-favorites:206+dirty`.

The source is a raw flowchart LR containing five nodes and four contributes-to
edges; it has no Markdown code fence. Source-path handling is the observed gap:
InputPolicy::plainText returns true for non-.md extensions, including .mmd;
FileOpenPolicy classifies readable unknown extensions as Text; openTarget selects
Editor. CmarkInterpreter returns literal Code for a plainText snapshot, before
Markdown/Mermaid interpretation. ModelBuilder dispatches MermaidBlockBuilder only
for code blocks with the mermaid fence label.

DiagramReadingTest reads .mmd fixtures but wraps them in a Markdown mermaid fence
and sets plainText false. These tests do not establish standalone .mmd opening.
UR-039 and FUNC-023 currently describe explicit fenced Mermaid recognition.

This is source inspection and CLI identity evidence, not a newly executed native
GUI reproduction or proof that this particular diagram has no rendering defects.
No source/renderer fix, build or installation was performed. Standalone input
support is KB-XFMD-023; broader Mermaid construct compatibility remains #020.
