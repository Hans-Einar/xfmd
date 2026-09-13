#!/usr/bin/env python3
"""Compare whole-document PDF against Cairo preview, without image-library dependencies."""
import pathlib
import re
import subprocess
import sys
import tempfile

def ppm(path):
    with path.open('rb') as stream:
        assert stream.readline().strip() == b'P6'
        line = stream.readline()
        while line.startswith(b'#'):
            line = stream.readline()
        w, h = map(int, line.split())
        assert stream.readline().strip() == b'255'
        data = stream.read()
        assert len(data) == w*h*3
        return w, h, data

def ink(data):
    return {i//3 for i in range(0, len(data), 3) if min(data[i:i+3]) < 180}

def unmatched(source, target, width, height):
    count = 0
    for at in source - target:
        y, x = divmod(at, width)
        if not any(yy*width+xx in target
                   for yy in range(max(0,y-2), min(height,y+3))
                   for xx in range(max(0,x-2), min(width,x+3))):
            count += 1
    return count / max(1,len(source))

with tempfile.TemporaryDirectory(prefix='xfmd-pdf-fidelity-') as directory:
    root = pathlib.Path(directory)
    subprocess.run([sys.argv[1], directory], check=True)
    count = int((root/'count').read_text())
    info = subprocess.check_output(['pdfinfo', str(root/'export.pdf')], text=True)
    assert re.search(r'Pages:\s+'+str(count)+r'\b', info), info
    assert re.search(r'Page size:\s+595\.\d+ x 841\.\d+ pts', info), info
    text = subprocess.check_output(['pdftotext', '-layout', str(root/'export.pdf'), '-'], text=True)
    for marker in ['æøå', 'office', '日本語', 'ENDOFFROZENBUFFER'] + ['Marker'+str(i) for i in range(45)] + ['TableHeader', '↗'] + ['TableRow'+str(i) for i in range(80)]:
        assert marker in text, marker
    subprocess.run(['pdftoppm','-r','96',str(root/'export.pdf'),str(root/'pdf')],check=True)
    outputs = sorted(root.glob('pdf-*.ppm'))
    assert len(outputs) == count
    for page, path in enumerate(outputs,1):
        w,h,screen = ppm(root/f'screen-{page}.ppm')
        pw,ph,pdf = ppm(path)
        assert (w,h)==(pw,ph)
        a,b=ink(screen),ink(pdf)
        missing,extra=unmatched(a,b,w,h),unmatched(b,a,w,h)
        assert max(missing,extra) < .015, (page,missing,extra)
        print(f'page {page}: ink outside 2px tolerance {missing:.3%}/{extra:.3%}')
print('PASS A4, page count, Unicode, all content and every page raster')
