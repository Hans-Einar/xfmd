#!/usr/bin/env python3
"""Check pilot identity/coverage/provenance; not a replacement SDL validator."""
import hashlib
import json
import re
from pathlib import Path
from xml.etree import ElementTree

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def main():
    req = (ROOT / "xfmd_requirements.md").read_text()
    expected = set(re.findall(r"^\| ((?:UR|SR)-\d{3}) \|", req, re.M))
    coverage = (ROOT / "SDP/02--Requirements/XFMD-Modeling/Coverage.md").read_text()
    mapped = set(re.findall(r"\b(?:UR|SR)-\d{3}\b", coverage.split("## Amendments")[0]))
    assert expected == mapped, ("requirement map", expected - mapped, mapped - expected)
    out = HERE / "views/navigation-pilot"
    ast = json.loads((out / "sdl-ast.json").read_text())
    assert ast["valid"] and not ast["diagnostics"]
    inventory = json.loads((ROOT / "SDP/02--Requirements/XFMD-Modeling/inventory.json").read_text())
    symbols = set(ast["symbols"])
    mapped_ids = []
    for blueprint in inventory["blueprints"]:
        # The current inventory is larger; verify only identities in the frozen pilot AST.
        if blueprint["sdl"] and set(blueprint["sdl"]["symbols"]) <= symbols:
            mapped_ids.append(blueprint["id"])
    assert len(mapped_ids) == 17  # 3 Features and 14 Functionality identities in this bounded pilot.
    for entry in ("FilesClean", "FilesDirtyRejected", "IndexClean"):
        ElementTree.parse(out / (entry + ".svg"))
        assert (out / (entry + ".txt")).stat().st_size > 0
    manifest = json.loads((out / "provenance.json").read_text())
    for path, expected_hash in manifest["sources"].items():
        assert hashlib.sha256((ROOT / path).read_bytes()).hexdigest() == expected_hash, path
    for path, expected_hash in manifest["outputs"].items():
        assert hashlib.sha256((out / path).read_bytes()).hexdigest() == expected_hash, path
    print(f"PASS: {len(expected)} requirement IDs assigned, {len(mapped_ids)} blueprint identities linked;")
    print("three well-formed SVG states and current source/output hashes; no behavioral-completeness claim")


if __name__ == "__main__":
    main()
