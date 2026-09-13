#!/usr/bin/env python3
"""ENOSPC at final fsync must leave the old PDF untouched."""
import pathlib
import subprocess
import sys
import tempfile
with tempfile.TemporaryDirectory(prefix='xfmd-pdf-full-') as directory:
    root = pathlib.Path(directory)
    path = root/'existing.pdf'
    path.write_text('original')
    subprocess.run(['strace','-o',str(root/'trace'),'-e','inject=fsync:error=ENOSPC:when=1',sys.argv[1],str(path)],check=True)
    assert 'ENOSPC' in (root/'trace').read_text()
    assert path.read_text() == 'original'
    assert not list(root.glob('*.xfmd-pdf-*'))
print('PASS ENOSPC at PDF flush preserves target and removes temporary output')
