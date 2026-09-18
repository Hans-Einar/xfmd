#!/usr/bin/env python3
"""Derive reproducible build identity without a mutable per-commit counter."""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile


def git(source, *args):
    try:
        return subprocess.check_output(
            ['git', '-C', str(source), *args], text=True, stderr=subprocess.PIPE
        ).strip()
    except (OSError, subprocess.CalledProcessError) as error:
        raise RuntimeError(f'Cannot read build history: {error}') from error


def identity(source, environment=None):
    source = Path(source).resolve()
    environment = os.environ if environment is None else environment
    version = (source / 'VERSION').read_text().strip()
    if not re.fullmatch(r'(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)', version):
        raise ValueError('VERSION must contain major.minor, for example 0.1')
    result = dict(version=version, branch='source', commit_number=None,
                  commit=None, pull_request=None, dirty=False)
    label = 'source'
    if (source / '.git').exists():
        if git(source, 'rev-parse', '--is-shallow-repository') != 'false':
            raise ValueError('Full Git history is required: git fetch --unshallow')
        result['commit_number'] = int(git(source, 'rev-list', '--count', 'HEAD'))
        result['commit'] = git(source, 'rev-parse', 'HEAD')
        branch = git(source, 'rev-parse', '--abbrev-ref', 'HEAD')
        if branch == 'HEAD':
            head = environment.get('GITHUB_HEAD_REF', '')
            branch = 'ci/' + head if environment.get('GITHUB_ACTIONS') == 'true' and head else 'detached'
        result['branch'] = branch
        label = branch
        if branch == 'main':
            subjects = git(source, 'log', '--first-parent', '--merges', '--format=%s')
            for subject in subjects.splitlines():
                match = re.match(r'^Merge pull request #([0-9]+)\b', subject)
                if match:
                    result['pull_request'] = int(match[1])
                    label = 'PR#' + match[1]
                    break
        result['dirty'] = bool(git(source, 'status', '--porcelain', '--untracked-files=normal'))
    number = result['commit_number'] if result['commit_number'] is not None else 'unknown'
    result['display'] = f"{version} {label}:{number}" + ('+dirty' if result['dirty'] else '')
    return result


def write_changed(path, content):
    path = Path(path)
    if path.exists() and path.read_text() == content:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile(mode='w', dir=path.parent, delete=False) as output:
        output.write(content)
        temporary = output.name
    os.replace(temporary, path)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    try:
        value = identity(args.source)
        write_changed(args.output / 'VersionGenerated.h',
                      '#pragma once\n#define XFMD_BUILD_VERSION ' + json.dumps(value['display']) + '\n')
        write_changed(args.output / 'xfmd-build.json', json.dumps(value, indent=2) + '\n')
    except (OSError, ValueError, RuntimeError) as error:
        parser.exit(1, f'XFMD build identity: {error}\n')


if __name__ == '__main__':
    main()
