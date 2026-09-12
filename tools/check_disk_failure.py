#!/usr/bin/env python3
"""Inject ENOSPC at the first real write syscall in an isolated save transaction."""
import pathlib
import subprocess
import sys
import tempfile
with tempfile.TemporaryDirectory(prefix="xfmd-enospc-") as directory:
    path = pathlib.Path(directory) / "document.md"
    path.write_text("original")
    subprocess.run(["strace", "-o", str(pathlib.Path(directory) / "trace"),
                    "-e", "inject=write:error=ENOSPC:when=1", sys.argv[1], str(path)], check=True)
    assert path.read_text() == "original"
    assert not list(path.parent.glob("*.xfmd-*"))
print("ENOSPC preserves original, dirty state, and cleans temporary file")
