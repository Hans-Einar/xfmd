#!/usr/bin/env python3
"""Validate xfmd's Markdown blueprint structure without third-party packages.

This checks documentation, not the existence/correctness of planned C++ code.
Run from anywhere: python3 tools/validate_blueprints.py [--root PATH]
"""

import argparse
import re
import sys
from pathlib import Path
from urllib.parse import unquote, urlsplit

HEADINGS = [
    '## 1. Hensikt og avgrensning',
    '## 2. Krav og akseptanse',
    '## 3. Kontrakter og eierskap',
    '## 4. Atferd, tilstand og feil',
    '## 5. Plumbing',
    '## 6. Gjenbruk og avhengigheter',
    '## 7. Verifikasjon',
    '## 8. Status, risiko og endringskonsekvenser',
]
ENUMS = {
    'kind': {'Feature', 'Functionality'},
    'audience': {'User', 'System', 'Integration'},
    'role': {'Workflow', 'Service', 'Adapter', 'Mechanism'},
    'owner': {'application', 'interpreter', 'renderer'},
    'status': {'Proposed', 'Ready', 'Implemented', 'Verified', 'Retired'},
    'scope': {'FirstRelease', 'Future'},
}
REQUIREMENT = re.compile(r'\b(?:UR|SR)-\d{3}\b')
TEST = re.compile(r'\bAT-\d{3}\b')


def without_code(text):
    """Fenced examples are not actual links or document headings."""
    return re.sub(r'^```[^\n]*\n.*?^```\s*$', '', text, flags=re.M | re.S)


def check(root):
    errors = []

    def fail(path, message):
        errors.append(f'{path.relative_to(root)}: {message}')

    requirements_file = root / 'xfmd_requirements.md'
    registry_file = root / 'src/blueprint/README.md'
    if not requirements_file.is_file() or not registry_file.is_file():
        return ['Missing requirements or blueprint registry.'], 0, 0
    requirements_text = requirements_file.read_text()
    requirements = {}
    for line in requirements_text.splitlines():
        match = re.match(r'^\| ((?:UR|SR)-\d{3}) \|', line)
        if match:
            ident = match.group(1)
            if ident in requirements:
                fail(requirements_file, f'duplicate requirement {ident}')
            requirements[ident] = set(TEST.findall(line))
    if not requirements:
        fail(requirements_file, 'no requirement definitions')
    acceptance_ids = set().union(*requirements.values()) if requirements else set()

    registry_rows = {}
    for line in registry_file.read_text().splitlines():
        match = re.match(r'^\| ((?:UR|SR)-\d{3}) \|', line)
        if match:
            ident = match.group(1)
            if ident in registry_rows:
                fail(registry_file, f'duplicate matrix row {ident}')
            registry_rows[ident] = line
    for ident in registry_rows.keys() - requirements.keys():
        fail(registry_file, f'undefined matrix requirement {ident}')

    base = root / 'src/blueprint'
    files = sorted((base / 'feature').glob('*.md')) + sorted((base / 'functionality').glob('*.md'))
    objects = {}
    for path in files:
        text = path.read_text()
        front = re.match(r'\A---\n(.*?)\n---\n', text, re.S)
        if not front:
            fail(path, 'missing metadata block')
            continue
        meta = {}
        for line in front.group(1).splitlines():
            key, sep, value = line.partition(':')
            if not sep or key in meta:
                fail(path, f'invalid/duplicate metadata line: {line}')
                continue
            meta[key] = value.strip()
        for field in ['id', *ENUMS, 'requirements', 'uses']:
            if not meta.get(field):
                fail(path, f'missing field {field}')
        for field, allowed in ENUMS.items():
            if meta.get(field) not in allowed:
                fail(path, f'invalid {field}: {meta.get(field)}')
        ident = meta.get('id', '')
        if not re.fullmatch(r'(?:FTR|FUNC)-\d{3}', ident):
            fail(path, f'invalid ID {ident}')
            continue
        if ident in objects:
            fail(path, f'duplicate ID {ident}')
        expected_kind = 'Feature' if ident.startswith('FTR') else 'Functionality'
        expected_folder = 'feature' if expected_kind == 'Feature' else 'functionality'
        if meta.get('kind') != expected_kind or path.parent.name != expected_folder:
            fail(path, 'ID, kind and folder disagree')
        if not re.fullmatch(rf'{expected_kind}-{ident.split("-")[1]}--[A-Za-z0-9]+(?:-[A-Za-z0-9]+)*\.md', path.name):
            fail(path, 'filename must match kind/ID and English slug pattern')
        headings = re.findall(r'^## .+$', without_code(text), re.M)
        if headings != HEADINGS:
            fail(path, 'expected the eight fixed headings, with chapter 5 Plumbing')
        for heading in HEADINGS:
            if heading in text:
                section = text.split(heading, 1)[1].split('\n## ', 1)[0].strip()
                if not section:
                    fail(path, f'empty section {heading}')
        reqs = [x.strip() for x in meta.get('requirements', '').split(',') if x.strip()]
        if not reqs or len(set(reqs)) != len(reqs):
            fail(path, 'requirements must be nonempty and unique')
        for req in reqs:
            if req not in requirements:
                fail(path, f'undefined requirement {req}')
        for req in set(REQUIREMENT.findall(text)) - requirements.keys():
            fail(path, f'undefined requirement reference {req}')
        for at in set(TEST.findall(text)) - acceptance_ids:
            fail(path, f'undefined acceptance reference {at}')
        missing_tests = set().union(*(requirements.get(r, set()) for r in reqs)) - set(TEST.findall(text))
        if missing_tests:
            fail(path, f'missing acceptance references: {sorted(missing_tests)}')
        uses = [x.strip() for x in meta.get('uses', '').split(',') if x.strip() and x.strip() != 'none']
        if len(set(uses)) != len(uses):
            fail(path, 'duplicate uses reference')
        objects[ident] = (path, meta, reqs, uses)

        plumbing = text.split('## 5. Plumbing', 1)[-1].split('\n## 6.', 1)[0]
        rows = [line for line in plumbing.splitlines() if re.match(r'^\| \d+ \|', line)]
        if not rows:
            fail(path, 'no numbered plumbing rows')
        for row in rows:
            cols = [x.strip() for x in row.strip('|').split('|')]
            if len(cols) != 7 or any(not col for col in cols):
                fail(path, f'plumbing needs seven nonempty columns: {row}')
                continue
            status = cols[-1]
            if status not in {'Planned', 'Implemented'}:
                fail(path, 'plumbing status must be Planned or Implemented')
            source = cols[3].strip('`')
            if not re.fullmatch(r'src/[A-Za-z0-9_/.-]+\.(?:cpp|h)', source):
                fail(path, f'invalid plumbing source path: {source}')
            if status == 'Implemented' and not (root / source).is_file():
                fail(path, f'implemented source does not exist: {source}')
            if meta.get('status') in {'Implemented', 'Verified'} and status != 'Implemented':
                fail(path, 'implemented object still has planned plumbing')
        if meta.get('status') == 'Verified' and not re.search(r'^Evidence: .+', text, re.M):
            fail(path, 'Verified requires an Evidence: line with actual test evidence')

    for ident, (path, meta, reqs, uses) in objects.items():
        for target in uses:
            if target == ident or target not in objects:
                fail(path, f'unknown/self dependency {target}')
            elif objects[target][1].get('kind') != 'Functionality':
                fail(path, f'uses must refer to functionality, not {target}')
        for req in reqs:
            if ident not in registry_rows.get(req, ''):
                fail(registry_file, f'{req} matrix omits consumer {ident}')
        if f']({path.relative_to(base).as_posix()})' not in registry_file.read_text():
            fail(registry_file, f'missing object link {ident}')

    for req, tests in requirements.items():
        row = registry_rows.get(req)
        if not row:
            fail(registry_file, f'missing matrix row {req}')
            continue
        consumers = set(re.findall(r'\b(?:FTR|FUNC)-\d{3}\b', row))
        deferred = 'Deferred:' in row and '| Future |' in row
        if not consumers and not deferred:
            fail(registry_file, f'{req} has no consumer or explicit Future deferral')
        for ident in consumers:
            if ident not in objects or req not in objects[ident][2]:
                fail(registry_file, f'{req} has inconsistent consumer {ident}')
        if not tests <= set(TEST.findall(row)):
            fail(registry_file, f'{req} matrix omits acceptance ID')

    # Only actual Markdown links are checked. Planned paths remain plain code spans.
    for path in root.rglob('*.md'):
        if any(part in {'.git', 'build', '__pycache__'} or part.startswith('build-') for part in path.relative_to(root).parts):
            continue
        text = without_code(path.read_text())
        for match in re.finditer(r'!?\[[^\]\n]*\]\(([^)\n]+)\)', text):
            target = match.group(1).strip().strip('<>')
            parsed = urlsplit(target)
            if parsed.scheme or parsed.netloc or not parsed.path:
                continue
            destination = (path.parent / unquote(parsed.path)).resolve()
            if not destination.is_relative_to(root) or not destination.exists():
                fail(path, f'broken/outside local link: {target}')
    return errors, len(objects), len(requirements)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    errors, count, req_count = check(args.root.resolve())
    if errors:
        print('\n'.join(errors), file=sys.stderr)
        print(f'FAILED: {len(errors)} error(s)', file=sys.stderr)
        return 1
    print(f'OK: {count} blueprints, {req_count} requirements; metadata, matrix, plumbing and local file links checked.')
    print('Not checked: external links, heading anchors, actual C++ call graph or application behavior.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
