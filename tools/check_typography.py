#!/usr/bin/env python3
"""Independent PDF reader checks against the production glyph painter."""
import subprocess
import sys
import tempfile
import unicodedata
from pathlib import Path
with tempfile.TemporaryDirectory(prefix='xfmd-typography-') as directory:
    subprocess.run([sys.argv[1], directory], check=True)
    pdf = Path(directory) / 'typography.pdf'
    info = subprocess.check_output(['pdfinfo', str(pdf)], text=True)
    assert '595.276 x 841.89' in info, info
    text = unicodedata.normalize('NFC', subprocess.check_output(['pdftotext', str(pdf), '-'], text=True))
    for expected in ['XFMD', 'æøå', 'office', 'é', '日本語', 'Bold', 'code']:
        assert expected in text, (expected, text)
    print('PASS independent PDF A4/Unicode extraction')
