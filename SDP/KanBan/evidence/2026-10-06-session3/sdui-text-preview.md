# SDUI — ` EditorWorkspace `

Static GUI dump. Buttons and fields are text labels; no callbacks execute.

## Layout overview

Row/column structure in terminal cells. Heights follow content; this is not measured GUI geometry.

```text
SDUI GUI dump | EditorWorkspace | 160 columns | structural preview
[ Sidebar ]       [ Open ]          [ Save ]          [ Editor ]        [ Split ]         [ Preview ]       [ Back ]          [ Forward ]       [ Light / Dark ]
Document                                                                         Document path: [/project/README.md]
### Files / Index                                                     ### Markdown source area
[ Reload ]        [ Markdown ]      [ Up ]           [ Open ]         README.md
**Folder tree**
                                                                      Text editing area.
/project/docs
                                                                      Static representation.
/project/README.md
                                                                      Open, edit and save belong to native XFMD.
**Folders / Files / Favorites**

Favorites selected: docs; README.md.

Right-click actions belong to the native UI.
Concept: editor mode. Source ownership remains in DocumentSession.
```

## Content

Markdown is rendered as content. Nested blockquotes represent groups and frames. Horizontal siblings appear in reading order here; the overview shows placement. Mermaid diagrams are omitted.

> **Frame:** ` EditorWorkspace `
>
> **Row 1 · 1 component from left to right**
>
> > **Group:** ` EditorWorkspace/toolbar `
> >
> > **Row 1 · 9 components from left to right**
> >
> > **Button:** ` Sidebar `
> >
> > ---
> >
> > **Button:** ` Open `
> >
> > ---
> >
> > **Button:** ` Save `
> >
> > ---
> >
> > **Button:** ` Editor `
> >
> > ---
> >
> > **Button:** ` Split `
> >
> > ---
> >
> > **Button:** ` Preview `
> >
> > ---
> >
> > **Button:** ` Back `
> >
> > ---
> >
> > **Button:** ` Forward `
> >
> > ---
> >
> > **Button:** ` Light / Dark `
> >
> > ---
> >
>
> ---
>
> **Row 2 · 1 component from left to right**
>
> > **Group:** ` EditorWorkspace/path `
> >
> > **Row 1 · 2 components from left to right**
> >
> > > Document
> >
> > ---
> >
> > **Input:** ` Document path ` — ` /project/README.md `
> >
> > ---
> >
>
> ---
>
> **Row 3 · 1 component from left to right**
>
> > **Frame:** ` EditorWorkspace/$r2c0 `
> >
> > **Row 1 · 2 components from left to right**
> >
> > > **Frame:** ` EditorWorkspace/$r2c0/sidebar `
> > >
> > > **Row 1 · 1 component from left to right**
> > >
> > > > ### Files / Index
> > >
> > > ---
> > >
> > > **Row 2 · 1 component from left to right**
> > >
> > > > **Group:** ` EditorWorkspace/$r2c0/sidebar/$r1c0 `
> > > >
> > > > **Row 1 · 4 components from left to right**
> > > >
> > > > **Button:** ` Reload `
> > > >
> > > > ---
> > > >
> > > > **Button:** ` Markdown `
> > > >
> > > > ---
> > > >
> > > > **Button:** ` Up `
> > > >
> > > > ---
> > > >
> > > > **Button:** ` Open `
> > > >
> > > > ---
> > > >
> > >
> > > ---
> > >
> > > **Row 3 · 1 component from left to right**
> > >
> > > > **Folder tree**
> > > >
> > > > /project/docs
> > > >
> > > > /project/README.md
> > > >
> > > > **Folders / Files / Favorites**
> > > >
> > > > Favorites selected: docs; README.md.
> > > >
> > > > Right-click actions belong to the native UI.
> > >
> > > ---
> > >
> >
> > ---
> >
> > > **Frame:** ` EditorWorkspace/$r2c0/editor `
> > >
> > > **Row 1 · 1 component from left to right**
> > >
> > > > ### Markdown source area
> > >
> > > ---
> > >
> > > **Row 2 · 1 component from left to right**
> > >
> > > > README.md
> > > >
> > > > Text editing area.
> > > >
> > > > Static representation.
> > > >
> > > > Open, edit and save belong to native XFMD.
> > >
> > > ---
> > >
> >
> > ---
> >
>
> ---
>
> **Row 4 · 1 component from left to right**
>
> > Concept: editor mode. Source ownership remains in DocumentSession.
>
> ---
>
