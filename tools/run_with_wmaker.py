#!/usr/bin/env python3
"""Inside run_with_xvfb.py only: start a WM on the disposable test display."""
import os
import subprocess
import sys
import tempfile
from pathlib import Path
# A test HOME is the guard against accidentally starting a second WM in the user's session.
if not Path(os.environ.get('HOME', '')).name.startswith('xfmd-gui-home-'):
    raise SystemExit('Use run_with_xvfb.py to provide an isolated display and HOME')
with tempfile.TemporaryFile(mode='w+') as log:
    wm = subprocess.Popen(['wmaker', '--no-autolaunch', '--dont-restore', '--static'], stdout=log, stderr=log)
    try:
        result = subprocess.run(sys.argv[1:], timeout=45)
        if result.returncode:
            log.seek(0)
            sys.stderr.write(log.read())
        sys.exit(result.returncode)
    finally:
        wm.terminate()
        try:
            wm.wait(timeout=5)
        except subprocess.TimeoutExpired:
            wm.kill()
            wm.wait()
