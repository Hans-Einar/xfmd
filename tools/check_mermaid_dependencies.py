#!/usr/bin/env python3
"""Check the locked native-only dependency graph and pure Rust layer boundaries."""
import json,pathlib,subprocess,tomllib
root=pathlib.Path(__file__).resolve().parents[1]
metadata=json.loads(subprocess.check_output(['cargo','metadata','--locked','--offline','--format-version','1'],cwd=root))
packages={p['name']:p for p in metadata['packages']}
for banned in ('clap','resvg','usvg','web-sys','wasm-bindgen'):
    assert banned not in packages, f'Unexpected renderer/CLI dependency: {banned}'
for layer,forbidden in [('renderer','xfmd-mermaid-interpreter'),('interpreter','xfmd-diagram-layout')]:
    manifest=root / ('src/renderer/diagram/rust/Cargo.toml' if layer=='renderer' else 'src/interpreter/mermaid/rust/Cargo.toml')
    data=tomllib.loads(manifest.read_text())
    assert forbidden not in data['dependencies']
for package in packages.values():
    if package['name'].startswith('xfmd-'):
        continue
    assert package['license'], f'Missing license metadata: {package["name"]}'
print(f'PASS: {len(packages)} locked packages; no CLI/browser/SVG raster renderer; independent Rust layers')
