#!/usr/bin/env python3
"""Exercise fresh export; verify labels remain text and diagrams are vector-only."""
import pathlib, subprocess, sys, tempfile
with tempfile.TemporaryDirectory(prefix='xfmd-mermaid-pdf-') as folder:
    pdf=pathlib.Path(folder)/'mermaid.pdf'
    expected=pathlib.Path(folder)/'labels.txt'
    fixture=[sys.argv[1],str(pdf),str(expected)]
    if len(sys.argv)>2:
        fixture.append(sys.argv[2]) # Optional real Markdown gallery.
    subprocess.run(fixture,check=True)
    extracted=subprocess.check_output(['pdftotext','-layout',str(pdf),'-'],text=True)
    actual=' '.join(extracted.split())
    # Spatial extraction interleaves wrapped captions in adjacent columns.
    # Content-stream order preserves each SVG text block independently.
    raw=' '.join(subprocess.check_output(['pdftotext','-raw',str(pdf),'-'],text=True).split())
    for label in expected.read_text().splitlines():
        assert ' '.join(label.split()) in actual or ' '.join(label.split()) in raw, (label, actual)
    images=subprocess.check_output(['pdfimages','-list',str(pdf)],text=True).splitlines()
    assert len(images)==2, images
    info=subprocess.check_output(['pdfinfo',str(pdf)],text=True)
    assert '595.276 x 841.89 pts (A4)' in info, info
    print('PASS: all labels including Unicode extracted; native vector A4 diagrams')
