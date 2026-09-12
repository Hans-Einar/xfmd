#!/usr/bin/env python3
"""Run a GUI test in its own X server; never operate the user's desktop."""
import os
import subprocess
import sys
import tempfile
with tempfile.TemporaryFile() as log:
    read_fd, write_fd = os.pipe()
    server = subprocess.Popen(['Xvfb', '-displayfd', str(write_fd), '-screen', '0', '1280x900x24', '-nolisten', 'tcp'], pass_fds=(write_fd,), stdout=log, stderr=log)
    os.close(write_fd)
    try:
        with os.fdopen(read_fd) as pipe:
            display = pipe.readline().strip()
        if not display:
            raise RuntimeError('Xvfb failed to start')
        env = dict(os.environ, DISPLAY=':' + display)
        result = subprocess.run(sys.argv[1:], env=env, timeout=60)
        sys.exit(result.returncode)
    finally:
        server.terminate()
        server.wait(timeout=5)
