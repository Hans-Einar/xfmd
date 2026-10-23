#!/usr/bin/env python3
"""Reproduce the XFMD modeling inventory; this is not semantic validation."""
import hashlib
import json
import re
import subprocess
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
IDS = re.compile(r"\b(?:UC|UR|SR|AT|FTR|FUNC)-\d{3}\b")


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def build():
    model_index = ROOT / "SDP/SDL/XFMD/README.md"
    model_map_path = ROOT / "SDP/SDL/XFMD/model-map.json"
    model_map = json.loads(model_map_path.read_text())
    model_symbols = {ident: [item["symbol"]] for ident, item in model_map["identities"].items()}
    model_entry = model_map["entry"]
    requirements = ROOT / "xfmd_requirements.md"
    text = requirements.read_text()
    occurrences = {}
    for number, line in enumerate(text.splitlines(), 1):
        for ident in set(IDS.findall(line)):
            occurrences.setdefault(ident, []).append(number)
    definitions = {}
    for number, line in enumerate(text.splitlines(), 1):
        match = re.match(r"\| ((?:UC|UR|SR)-\d{3}) \|", line)
        if match:
            definitions.setdefault(match[1], []).append(number)
    blueprints = []
    for folder in ("feature", "functionality"):
        for path in sorted((ROOT / "src/blueprint" / folder).glob("*.md")):
            content = path.read_text()
            metadata = dict(re.findall(r"^(\w+): (.*)$", content.split("---", 2)[1], re.M))
            blueprints.append({
                "id": metadata["id"], "title": path.stem.split("--", 1)[1],
                "path": str(path.relative_to(ROOT)), "sha256": digest(path),
                "owner": metadata["owner"], "declaredStatus": metadata["status"],
                "requirements": IDS.findall(metadata.get("requirements", "")),
                "referencedIds": sorted(set(IDS.findall(content))),
                "sdl": None, "sdui": None, "modelStatus": "not-authored",
            })
            entry = blueprints[-1]
            if entry["id"] in model_symbols:
                entry["sdl"] = {"path": model_entry,
                                "symbols": model_symbols[entry["id"]],
                                "scope": "blueprint structural inventory; declared scenarios are not executable C++"}
                entry["modelStatus"] = "structural-model-linked"
            if entry["id"] in {"FUNC-005", "FUNC-010", "FUNC-011", "FUNC-014", "FUNC-018", "FUNC-032"}:
                entry["sdui"] = {"path": "SDP/SDL/XFMD/SDUI/XfmdDesktop/Desktop.sdui",
                                 "scope": "static state samples, not native interaction"}
    identities = []
    for ident, lines in sorted(occurrences.items()):
        if not ident.startswith(("UC-", "UR-", "SR-", "AT-")):
            continue
        disposition = "current-definition" if ident in definitions else "reference-only"
        if ident in {"UC-006", "UR-010", "SR-014", "AT-010", "AT-024"}:
            disposition = "future-xfw-integration"
        elif ident in {"UR-043", "SR-026", "AT-068"}:
            disposition = "historical-boxui-reservation"
        elif ident.startswith("AT-"):
            disposition = "acceptance-specification-not-test-evidence"
        identities.append({"id": ident, "sourceLines": lines,
                           "definitionLines": definitions.get(ident, []),
                           "disposition": disposition,
                           "blueprints": [b["id"] for b in blueprints if ident in b["referencedIds"]],
                           "sdl": {"path": model_entry,
                                   "symbols": model_symbols[ident]} if ident in model_symbols else None,
                           "sdui": None, "evidenceReview": "pending"})
    source_files = []
    for path in sorted((ROOT / "src").rglob("*")):
        if not path.is_file() or path.suffix not in {".cpp", ".h", ".rs", ".toml"}:
            continue
        rel = str(path.relative_to(ROOT))
        # Filename references are discovery hints, never proof of ownership.
        matches = [b["id"] for b in blueprints
                   if path.name in (ROOT / b["path"]).read_text()]
        source_files.append({"path": rel, "sha256": digest(path),
                             "sourceLayer": path.relative_to(ROOT / "src").parts[0],
                             "blueprintFilenameMentions": matches,
                             "semanticOwnershipReview": "pending"})
    inputs = ["xfmd_requirements.md", "softwareArchitecture.md", "src/blueprint/README.md",
              "sprints/README.md", "docs/working-method.md", "CMakeLists.txt", "VERSION"]
    if model_index.exists():
        inputs += ["SDP/SDL/XFMD/README.md", model_entry, "SDP/SDL/XFMD/model-map.json",
                   "SDP/SDL/XFMD/SDUI/XfmdDesktop/Desktop.sdui"]
    data = {"method": "Identity/metadata/file inventory, not semantic model or test coverage",
            "head": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip(),
            "inputs": {name: digest(ROOT / name) for name in inputs},
            "blueprints": blueprints, "requirementIdentities": identities,
            "sourceFiles": source_files,
            "reservationsAbsentFromDefinitions": ["FTR-011", "FUNC-027", "FUNC-028",
                                                   "FUNC-029", "FUNC-030", "UR-043", "SR-026", "AT-069"]}
    (HERE / "inventory.json").write_text(json.dumps(data, indent=2) + "\n")
    rows = ["# Generated XFMD modeling inventory", "",
            "Regenerate with `python3 SDP/02--Requirements/XFMD-Modeling/build_inventory.py`.",
            "See [Baseline](Baseline.md) for interpretation and [inventory.json](inventory.json)",
            "for every identity occurrence, source file and input hash. This is an inventory,",
            "not an SDL-generated viewpoint or a semantic completeness claim.", "",
            f"Baseline HEAD: `{data['head']}`; current input hashes are in the JSON.", "",
            f"{len(blueprints)} blueprints; {len(source_files)} C++/Rust source and manifest files.", "",
            "| Blueprint | Title | Declared owner | Requirement IDs | Model status |",
            "| --- | --- | --- | --- | --- |"]
    for b in blueprints:
        rows.append(f"| [{b['id']}](../../../{b['path']}) | {b['title']} | {b['owner']} | {', '.join(b['requirements'])} | {b['modelStatus']} |")
    rows += ["", "## Requirement identity inventory", "",
             "Repeated definitions may be amendments; they are not silently discarded.",
             "Acceptance references are specifications, not proof that checks ran.", "",
             "| Family | Referenced identities | Table definitions |",
             "| --- | --- | --- |"]
    for prefix in ("UC", "UR", "SR", "AT"):
        count = sum(i["id"].startswith(prefix + "-") for i in identities)
        defined = sum(i.startswith(prefix + "-") for i in definitions)
        rows.append(f"| {prefix} | {count} | {defined if prefix != 'AT' else 'Embedded acceptance clauses'} |")
    unmentioned = [s for s in source_files if not s["blueprintFilenameMentions"]]
    rows += ["", f"{len(unmentioned)} source files have no literal filename mention in the blueprint bodies.",
             "These are candidates for ownership review (GAP-XFMD-DOC-001), not proven missing designs.",
             "Model links cover every existing blueprint structurally. They do not establish full behavioral coverage.",
             "See [Coverage](Coverage.md) for all requirement concerns, amendments and source-family responsibilities.", ""]
    (HERE / "Inventory.md").write_text("\n".join(rows))
    print(f"Inventory: {len(blueprints)} blueprints, {len(identities)} requirement identities, "
          f"{len(source_files)} source files, {len(unmentioned)} without filename mentions")


if __name__ == "__main__":
    build()
