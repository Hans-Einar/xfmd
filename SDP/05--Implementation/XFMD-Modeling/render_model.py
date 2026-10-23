#!/usr/bin/env python3
"""Validate/render the whole-inventory model baseline with explicit, already-built producer tools."""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
MODEL = ROOT / "SDP/SDL/XFMD"


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--sdl", required=True)
    p.add_argument("--sdui", required=True)
    p.add_argument("--output", type=Path, default=HERE / "views/xfmd-model")
    args = p.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    sdl, sdui = Path(args.sdl).resolve(), Path(args.sdui).resolve()
    baseline = json.loads((HERE / "evidence/XM-M2/probe-results.json").read_text())
    for name, binary in (("sdl", sdl), ("sdui", sdui)):
        if sha(binary) != baseline["binaries"][name]["sha256"]:
            raise RuntimeError(f"{name} binary differs from XM-M2; establish a new tool baseline first")
    design = MODEL / "XfmdDesktop.design"
    ui = MODEL / "SDUI/XfmdDesktop/Desktop.sdui"
    commands = []

    def run(argv, destination):
        result = subprocess.run([str(a) for a in argv], capture_output=True, text=True, timeout=60)
        commands.append({"argv": [str(a) for a in argv], "exitCode": result.returncode,
                         "stderr": result.stderr})
        if result.returncode:
            raise RuntimeError(result.stderr or result.stdout)
        if destination:
            destination.write_text(result.stdout)
        return result.stdout

    run([sdl, "check", design], output / "sdl-check.json")
    canonical = run([sdl, "format", design], None)
    if canonical != design.read_text():
        raise RuntimeError("Authored SDL is not canonical; review the formatter output")
    run([sdl, "ast", design], output / "sdl-ast.json")
    run([sdl, "viewpoints", design, "--output", output / "sdl", "--project", "xfmd-model",
         "--format", "static", "--viewpoint", "VP01,VP02,VP07,VP08,VP09"], output / "sdl-generation.json")
    for entry in ("SplitWorkspace", "EditorWorkspace", "PreviewWorkspace", "PreferencesConcept", "PublicationConcept", "SdpProjectWorkspace"):
        for fmt, suffix in (("ast", "json"), ("dump", "txt"), ("svg", "svg")):
            run([sdui, ui, "--entry", entry, "--format", fmt, "--width", "1440", "--height", "900"],
                output / f"{entry}.{suffix}")
    manifest = {"scope": "Whole blueprint structural inventory and declared workflow examples; conceptual SDUI, not full executable semantics or native verification",
                "producerCommit": baseline["producerCommit"],
                "sources": {str(p.relative_to(ROOT)): sha(p) for p in (design, ui)},
                "tools": {"sdl": {"path": str(sdl), "sha256": sha(sdl)},
                          "sdui": {"path": str(sdui), "sha256": sha(sdui)}},
                "commands": commands,
                "outputs": {str(p.relative_to(output)): sha(p) for p in sorted(output.rglob("*"))
                            if p.is_file() and p.name != "provenance.json"}}
    (output / "provenance.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"PASS: SDL checks/views and six SDUI concept exports; {len(manifest['outputs'])} outputs")


if __name__ == "__main__":
    main()
