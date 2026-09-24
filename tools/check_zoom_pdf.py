#!/usr/bin/env python3
"""Verify actual exported page and word geometry across document zoom modes."""
import pathlib
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET

with tempfile.TemporaryDirectory(prefix="xfmd-zoom-pdf-") as directory:
    root = pathlib.Path(directory)
    subprocess.run([sys.argv[1], directory], check=True)
    baseline = None
    for variant in range(4):
        xml = subprocess.check_output(
            ["pdftotext", "-bbox", str(root / f"zoom-{variant}.pdf"), "-"]
        )
        pages = ET.fromstring(xml).findall(".//{*}page")
        result = [
            (dict(page.attrib), [(dict(word.attrib), word.text) for word in page.findall("{*}word")])
            for page in pages
        ]
        assert len(pages) > 1, "Fixture must exercise all pages"
        text = " ".join(word.text or "" for page in pages for word in page.findall("{*}word"))
        for marker in ["æøå", "ENDOFFROZENBUFFER"] + [f"Marker{i}" for i in range(24)]:
            assert marker in text, (variant, marker)
        if baseline is None:
            baseline = result
        else:
            assert result == baseline, f"Zoom variant {variant} changed PDF geometry or content"
        print(f"variant {variant}: {len(pages)} pages; identical page dimensions and every word box")
print("PASS actual A4/Wrap export geometry at 100%, 300%, fit-height and 50%")
