#!/usr/bin/env python3
"""Build the committed matching SDPTool source pin; never consume local helpers."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tarfile


def source_pin(root):
    pin = json.loads((root/'cmake/sdptool-source.json').read_text())
    if (pin.get('repository') != 'Hans-Einar/SDP' or
            not re.fullmatch(r'[0-9a-f]{40}', pin.get('commit', '')) or
            not re.fullmatch(r'[0-9a-f]{64}', pin.get('sha256', '')) or
            not re.fullmatch(r'[0-9A-Za-z.+-]+', pin.get('version', '')) or
            pin.get('archive') != 'third_party/sdptool-' + pin['commit'] + '.tar.gz'):
        raise ValueError('Invalid committed SDPTool source pin')
    archive = root/pin['archive']
    if hashlib.sha256(archive.read_bytes()).hexdigest() != pin['sha256']:
        raise ValueError('SDPTool source archive digest mismatch')
    return pin, archive


def extract_source(archive, destination, commit):
    with tarfile.open(archive, 'r:gz') as source:
        if source.pax_headers.get('comment') != commit:
            raise ValueError('SDPTool Git archive commit does not match pin')
        for member in source.getmembers():
            path = Path(member.name)
            if (path.is_absolute() or '..' in path.parts or
                    not path.parts or path.parts[0] not in ('SDPTool', 'SDL', 'SDUI') or
                    not (member.isfile() or member.isdir())):
                raise ValueError('Unsafe SDPTool archive member')
        source.extractall(destination, filter='data')


def json_stream(data):
    decoder = json.JSONDecoder()
    while data.strip():
        item, end = decoder.raw_decode(data.lstrip())
        yield item
        data = data.lstrip()[end:]


def build(root, work, output):
    pin, archive = source_pin(root)
    extract_source(archive, work, pin['commit'])
    module = work/'SDPTool'
    env = dict(os.environ, CGO_ENABLED='0', GOTOOLCHAIN='local', GOWORK='off')
    output.mkdir(parents=True, exist_ok=True)
    binary = output/'sdptool'
    flags = ('-s -w -X github.com/Hans-Einar/SDP/SDPTool.BuildRevision=' + pin['commit'] +
             ' -X github.com/Hans-Einar/SDP/SDPTool.BuildVersion=' + pin['version'])
    subprocess.run(['go', 'build', '-trimpath', '-mod=readonly', '-buildvcs=false',
                    '-ldflags', flags, '-o', str(binary), './cmd/sdptool'], cwd=module, env=env, check=True)
    # Match RPM's idempotent stripped-ELF normalization before recording its digest.
    subprocess.run(['strip', '-R', '.comment', '-R', '.note', binary], check=True)
    identity = json.loads(subprocess.check_output([binary, '--version', '--json'], text=True))
    if identity.get('revision') != pin['commit'] or identity.get('version') != pin['version']:
        raise ValueError('Built SDPTool identity does not match committed source pin')
    # Downloaded dependencies are checked against source go.sum; retain their licenses.
    subprocess.run(['go', 'mod', 'download'], cwd=module, env=env, check=True)
    modules = subprocess.check_output(['go', 'list', '-m', '-json', 'all'], cwd=module, env=env, text=True)
    inventory = []
    for dependency in json_stream(modules):
        resolved = dependency.get('Replace', dependency)
        item = {key: dependency[key] for key in ('Path', 'Version', 'Sum') if key in dependency}
        directory = resolved.get('Dir')
        if directory and 'Version' in resolved:
            destination = output/'licenses'/re.sub(r'[^A-Za-z0-9_.@-]', '_', dependency['Path'] + '@' + resolved['Version'])
            notices = [p for p in Path(directory).iterdir() if p.is_file() and
                       p.name.lower().startswith(('license', 'copying', 'notice', 'authors'))]
            if not notices:
                raise ValueError('No dependency license/notice found: ' + dependency['Path'])
            destination.mkdir(parents=True, exist_ok=True)
            for notice in notices:
                shutil.copy2(notice, destination/notice.name)
        inventory.append(item)
    go_root = Path(subprocess.check_output(['go', 'env', 'GOROOT'], text=True).strip())
    standard_notices = output/'licenses/go-stdlib'
    standard_notices.mkdir(parents=True, exist_ok=True)
    license_root = go_root if (go_root/'LICENSE').is_file() else Path('/usr/share/licenses/golang')
    if not (license_root/'LICENSE').is_file():
        raise ValueError('Go standard library license is missing')
    for name in ('LICENSE', 'PATENTS'):
        if (license_root/name).is_file(): shutil.copy2(license_root/name, standard_notices/name)
    for base in (go_root/'src/vendor', go_root/'src/crypto/internal/boring'):
        if base.is_dir():
            for notice in base.rglob('*'):
                if notice.is_file() and notice.name.lower().startswith(('license', 'copying', 'notice')):
                    destination = standard_notices/notice.relative_to(go_root/'src')
                    destination.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copy2(notice, destination)
    (output/'modules.json').write_text(json.dumps(inventory, indent=2)+'\n')
    provenance = dict(pin, binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),
                      go_version=subprocess.check_output(['go', 'version'], text=True).strip(),
                      cgo_enabled=False, buildvcs=False, rpm_note_strip=True)
    (output/'sdptool-build.json').write_text(json.dumps(provenance, indent=2)+'\n')
    shutil.copy2(archive, output/archive.name)
    print(json.dumps(provenance, indent=2))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--work', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    build(args.source.resolve(), args.work.resolve(), args.output.resolve())
