#!/usr/bin/env python3
"""Probe the installed executable on an isolated display with installed resources."""
import ctypes as C
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import time

def window_titles(display):
    x = C.CDLL('libX11.so.6')
    x.XOpenDisplay.argtypes = [C.c_char_p]
    x.XOpenDisplay.restype = C.c_void_p
    x.XDefaultRootWindow.argtypes = [C.c_void_p]
    x.XDefaultRootWindow.restype = C.c_ulong
    x.XQueryTree.argtypes = [C.c_void_p, C.c_ulong, C.POINTER(C.c_ulong), C.POINTER(C.c_ulong), C.POINTER(C.POINTER(C.c_ulong)), C.POINTER(C.c_uint)]
    x.XFetchName.argtypes = [C.c_void_p, C.c_ulong, C.POINTER(C.c_char_p)]
    x.XFree.argtypes = [C.c_void_p]
    x.XCloseDisplay.argtypes = [C.c_void_p]
    d = x.XOpenDisplay(display.encode())
    assert d
    root, parent, count = C.c_ulong(), C.c_ulong(), C.c_uint()
    children = C.POINTER(C.c_ulong)()
    titles = []
    try:
        assert x.XQueryTree(d, x.XDefaultRootWindow(d), C.byref(root), C.byref(parent), C.byref(children), C.byref(count))
        for i in range(count.value):
            name = C.c_char_p()
            if x.XFetchName(d, children[i], C.byref(name)) and name.value:
                titles.append(name.value.decode(errors='replace'))
                x.XFree(name)
        if children:
            x.XFree(children)
        return titles
    finally:
        x.XCloseDisplay(d)


expected = sys.argv[1]
metadata = json.loads(Path('/usr/share/doc/xfmd/xfmd-build.json').read_text())
assert metadata['display'] == expected and not metadata['dirty'], metadata
assert metadata['commit'] == sys.argv[2], metadata
for path in ['/usr/share/applications/xfmd.desktop', '/usr/share/icons/hicolor/64x64/apps/xfmd.png',
             '/usr/share/xfmd/math/.clatexmath-res_root', '/usr/share/doc/xfmd/FOX-LICENSE']:
    assert Path(path).is_file(), path
with tempfile.TemporaryDirectory() as temporary:
    base = Path(temporary)
    fixture = base / 'package.md'
    fixture.write_text('# RPM verification\n\nNative **Markdown** and $x^2 + y^2$.\n')
    read_fd, write_fd = os.pipe()
    server = subprocess.Popen(['Xvfb', '-displayfd', str(write_fd), '-screen', '0', '1280x900x24', '-nolisten', 'tcp'], pass_fds=(write_fd,))
    os.close(write_fd)
    application = None
    try:
        with os.fdopen(read_fd) as pipe:
            display = pipe.readline().strip()
        assert display
        environment = dict(os.environ, DISPLAY=':' + display, HOME=temporary, XDG_CONFIG_HOME=str(base/'.config'))
        application = subprocess.Popen(['/usr/bin/xfmd', str(fixture)], env=environment, stderr=subprocess.PIPE)
        title = ''
        for attempt in range(40):
            assert application.poll() is None, application.communicate()[1]
            for title in window_titles(':' + display):
                if title.endswith('xfmd ' + expected):
                    break
            if title.endswith('xfmd ' + expected):
                break
            time.sleep(.25)
        assert title.endswith('xfmd ' + expected), title
        time.sleep(2)
        assert application.poll() is None
        print('Installed native window:', title)
    finally:
        if application is not None and application.poll() is None:
            application.terminate()
            application.wait(timeout=10)
        server.terminate()
        server.wait(timeout=10)
