#!/usr/bin/env python3
"""Fetch the pinned Mermaid source; apply only the reviewed text measurement seam."""
import hashlib
import io
import pathlib
import subprocess
import tarfile
import urllib.request
ROOT = pathlib.Path(__file__).resolve().parents[1]
PIN = '3726ccbffe0e8032361eb9668694b24f77858060'
SHA = '8bedf9632b455e829998e0ae9f6429b7a5e6fc8460e0a4ccf7df85b08ff94066'
DEST = ROOT / '.deps/mermaid-rs-renderer'
PATCH = ROOT / 'cmake/patches/mermaid-measurements.patch'
stamp = PIN + ':' + hashlib.sha256(PATCH.read_bytes()).hexdigest()
if not DEST.exists():
    data = urllib.request.urlopen('https://codeload.github.com/1jehuang/mermaid-rs-renderer/tar.gz/' + PIN, timeout=60).read()
    if hashlib.sha256(data).hexdigest() != SHA:
        raise SystemExit('Mermaid source checksum mismatch')
    DEST.parent.mkdir(exist_ok=True)
    with tarfile.open(fileobj=io.BytesIO(data)) as archive:
        archive.extractall(DEST.parent, filter='data')
    (DEST.parent / ('mermaid-rs-renderer-' + PIN)).rename(DEST)
if not (DEST / '.xfmd-pin').exists():
    subprocess.run(['patch', '-p1', '-i', str(PATCH)], cwd=DEST, check=True)
    (DEST / '.xfmd-pin').write_text(stamp)
elif (DEST / '.xfmd-pin').read_text() != stamp:
    raise SystemExit('Mermaid pin/patch changed; remove .deps/mermaid-rs-renderer and bootstrap again')
