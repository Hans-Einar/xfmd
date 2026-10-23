#!/usr/bin/env python3
"""Check generated SDL AST against existing authorities, identities and provenance.

This checks the authored model, not runtime C++ conformance or native SDUI behavior.
"""
import hashlib
import json
import re
from pathlib import Path
from xml.etree import ElementTree

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    out = HERE / "views/xfmd-model"
    mapping = json.loads((ROOT / "SDP/SDL/XFMD/model-map.json").read_text())
    identities = mapping["identities"]
    ast = json.loads((out / "sdl-ast.json").read_text())
    assert ast["valid"] and not ast["diagnostics"]
    symbols, statements = ast["symbols"], ast["ast"]["statements"]
    relations = {(s["subject"]["name"], s["verb"], s["object"]["name"])
                 for s in statements if s["node"] == "Relation"}
    parents = {b: a for a, verb, b in relations if verb == "contains"}
    blueprints = {}
    for folder in ("feature", "functionality"):
        for path in (ROOT / "src/blueprint" / folder).glob("*.md"):
            metadata = dict(re.findall(r"^(\w+): (.*)$", path.read_text().split("---", 2)[1], re.M))
            blueprints[metadata["id"]] = metadata
    assert set(blueprints) == {k for k in identities if k.startswith(("FTR-", "FUNC-"))}
    for ident, bp in blueprints.items():
        entry = identities[ident]
        symbol = entry["symbol"]
        assert symbols[symbol]["kind"] == ("feature" if ident.startswith("FTR-") else "functionality")
        if ident.startswith("FUNC-"):
            owners = {a for a, verb, b in relations if verb == "owns" and b == symbol}
            assert owners == {entry["owner"]}, ident
            ancestors = {entry["owner"]}
            current = entry["owner"]
            while current in parents:
                current = parents[current]
                ancestors.add(current)
            assert bp["owner"].capitalize() in ancestors, (ident, ancestors)
            allocations = [s for s in statements if s["node"] == "Allocation"
                           and s["subject"]["name"] == symbol]
            assert len(allocations) == 1 and allocations[0]["container"]["name"] == "XfmdDesktop", ident
        else:
            expected = {identities[x]["symbol"] for x in re.findall(r"FUNC-\d+", bp["uses"])}
            actual = {a for a, verb, b in relations if verb == "contributes-to" and b == symbol}
            assert expected == actual, (ident, expected ^ actual)
    requirements = (ROOT / "xfmd_requirements.md").read_text()
    current_uc = set(re.findall(r"^\| (UC-\d{3}) \|", requirements, re.M)) - {"UC-006"}
    assert current_uc == {k for k in identities if k.startswith("UC-")}
    expected_requirements = set(re.findall(r"^\| ((?:UR|SR)-\d{3}) \|", requirements, re.M))
    coverage = (ROOT / "SDP/02--Requirements/XFMD-Modeling/Coverage.md").read_text().split("## Amendments")[0]
    assert expected_requirements == set(re.findall(r"\b(?:UR|SR)-\d{3}\b", coverage))
    scenarios = {name for name, declaration in symbols.items() if declaration["kind"] == "scenario"}
    assert scenarios == {s["symbol"] for s in mapping["scenarios"]}
    steps = {}
    for scenario in mapping["scenarios"]:
        rows = sorted((s for s in statements if s["node"] == "Step" and
                       s["subject"]["name"] == scenario["symbol"]), key=lambda s: s["ordinal"]["value"])
        assert len(rows) == scenario["steps"]
        steps[scenario["symbol"]] = [r["message"]["name"] for r in rows]
    saved = steps["SaveThenReadFails"]
    assert saved.index("CurrentWritten") < saved.index("MarkCurrentSaved") < saved.index("ReadCandidate")
    invalid = steps["InvalidSelectionRetainsChild"]
    assert "RejectInvalidSelection" in invalid and "CancelPreviousChild" not in invalid
    accepted = steps["GeneratedSelectionSupersedes"]
    assert accepted.index("ValidateSelection") < accepted.index("CancelPreviousChild")
    lease = steps["GeneratedReplacementReleasesOldLease"]
    assert lease.index("GeneratedDocumentChanged") < lease.index("QueuePreviousLease") < lease.index("InstallNewLease")
    inventory = json.loads((ROOT / "SDP/02--Requirements/XFMD-Modeling/inventory.json").read_text())
    for row in inventory["blueprints"]:
        assert row["sdl"]["path"] == mapping["entry"] and row["sha256"] == sha(ROOT / row["path"])
    for path, digest in inventory["inputs"].items():
        assert sha(ROOT / path) == digest, path
    for row in inventory["sourceFiles"]:
        assert sha(ROOT / row["path"]) == row["sha256"], row["path"]
    for entry in ("SplitWorkspace", "EditorWorkspace", "PreviewWorkspace", "PreferencesConcept", "PublicationConcept", "SdpProjectWorkspace"):
        ElementTree.parse(out / (entry + ".svg"))
    manifest = json.loads((out / "provenance.json").read_text())
    for path, digest in manifest["sources"].items():
        assert sha(ROOT / path) == digest, path
    for path, digest in manifest["outputs"].items():
        assert sha(out / path) == digest, path
    print(f"PASS: {len(blueprints)} blueprint mappings/owners/contributions, {len(current_uc)} current use cases,")
    print(f"{len(expected_requirements)} requirement IDs, {len(scenarios)} declared scenarios, six SVG compositions,")
    print(f"{len(inventory['sourceFiles'])} source hashes and {len(manifest['outputs'])} generated outputs; no runtime or independent-review claim.")


if __name__ == "__main__":
    main()
