#!/usr/bin/env python3
"""Reject toolkit/parser/application implementation includes across pure layers."""
import re
import sys
from pathlib import Path
root = Path(__file__).resolve().parents[1]
errors = []
for layer in ('contracts', 'interpreter', 'renderer'):
    for path in (root / 'src' / layer).rglob('*'):
        if path.suffix not in ('.h', '.cpp'):
            continue
        for line in path.read_text().splitlines():
            match = re.match(r'\s*#include\s*[<"]([^>"]+)', line)
            if not match:
                continue
            inc = match.group(1)
            banned = ('application/', 'fx.h', 'FX')
            if layer != 'interpreter':
                banned += ('cmark', 'md4c', 'interpreter/')
            if layer != 'renderer':
                banned += ('renderer/',)
            if inc.startswith(banned):
                errors.append(f'{path.relative_to(root)}: forbidden dependency {inc}')
if errors:
    print('\n'.join(errors), file=sys.stderr)
    sys.exit(1)
print('PASS: pure-layer include boundaries')
