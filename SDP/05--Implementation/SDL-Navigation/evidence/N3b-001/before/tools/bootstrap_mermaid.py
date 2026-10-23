#!/usr/bin/env python3
"""Fetch the reviewed fork exactly; never patch or replace existing source."""
import hashlib
import io
import json
import pathlib
import subprocess
import tarfile
import tempfile
import urllib.request
ROOT = pathlib.Path(__file__).resolve().parents[1]
source = json.loads((ROOT / 'cmake/mermaid-source.json').read_text())
PIN = source['commit']
DEST = ROOT / '.deps/mermaid-rs-renderer'
stamp = PIN + ':' + source['sha256']
if DEST.exists():
    marker = DEST / '.xfmd-pin'
    if not marker.exists() or marker.read_text() != stamp:
        raise SystemExit('Mermaid pin changed: preserve/move existing .deps/mermaid-rs-renderer before bootstrap')
else:
    data = urllib.request.urlopen(
        'https://codeload.github.com/' + source['repository'] + '/tar.gz/' + PIN,
        timeout=60).read()
    if hashlib.sha256(data).hexdigest() != source['sha256']:
        raise SystemExit('Mermaid source checksum mismatch')
    DEST.parent.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(dir=DEST.parent) as tmp:
        with tarfile.open(fileobj=io.BytesIO(data)) as archive:
            archive.extractall(tmp, filter='data')
        extracted = pathlib.Path(tmp) / ('mermaid-rs-renderer-' + PIN)
        subprocess.run(['python3', str(extracted / 'tools/verify_libavoid.py')], check=True)
        (extracted / '.xfmd-pin').write_text(stamp)
        extracted.rename(DEST)
