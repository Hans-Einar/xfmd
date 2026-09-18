#!/usr/bin/env python3
"""The executable must report its embedded build, not current repository HEAD."""
import json
from pathlib import Path
import subprocess
import sys

value = json.loads(Path(sys.argv[2]).read_text())
actual = subprocess.check_output([sys.argv[1], '--version'], text=True).strip()
assert actual == 'xfmd ' + value['display'], (actual, value)
print(actual)
