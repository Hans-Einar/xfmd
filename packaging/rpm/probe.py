#!/usr/bin/env python3
"""Probe the installed executable on an isolated display with installed resources."""
import ctypes as C
import hashlib
import json
import os
from pathlib import Path
import subprocess
import signal
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
producer = Path('/usr/libexec/xfmd/sdptool')
producer_metadata = json.loads(Path('/usr/share/doc/xfmd/sdptool-build.json').read_text())
assert producer_metadata['commit'] == sys.argv[3], producer_metadata
assert producer.is_file() and os.access(producer, os.X_OK)
assert hashlib.sha256(producer.read_bytes()).hexdigest() == producer_metadata['binary_sha256']
producer_identity = json.loads(subprocess.check_output([producer, '--version', '--json'], text=True))
assert producer_identity['revision'] == producer_metadata['commit'], producer_identity
assert producer_identity['version'] == producer_metadata['version'], producer_identity

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
        project = base/'producer-project'
        (project/'SDP/SDL').mkdir(parents=True)
        (project/'SDP/SDL/Native.design').write_text('language design-core version 0.5.\nunit Desktop.\n')
        discovery = json.loads(subprocess.check_output([producer, str(project), 'discover', '--navigation', 'lazy-sdl', '--json'], text=True))
        assert discovery['capabilities']['readingNavigation'] == '1'
        assert discovery['navigation']['presentation'] == 'producer-reading/1'
        runtime = base/'runtime'
        runtime.mkdir(mode=0o700)
        environment.pop('XFMD_SDP_TOOL', None)
        environment['XDG_RUNTIME_DIR'] = str(runtime)
        trace = base/'native-producer.trace'
        native = subprocess.Popen(['strace', '-f', '-e', 'trace=execve', '-o', str(trace),
                                   '/usr/bin/xfmd', str(project)], env=environment,
                                  stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, start_new_session=True)
        try:
            for attempt in range(80):
                assert native.poll() is None, native.communicate()[1]
                observed = trace.read_text() if trace.exists() else ''
                if 'execve("/usr/libexec/xfmd/sdptool"' in observed and '"discover"' in observed:
                    break
                time.sleep(.25)
            else:
                raise AssertionError('Installed native application did not invoke its private producer: ' + observed)
            print('Installed native private producer:', producer_identity['revision'])
        finally:
            if native.poll() is None:
                os.killpg(native.pid, signal.SIGTERM)
                native.wait(timeout=10)

    finally:
        if application is not None and application.poll() is None:
            application.terminate()
            application.wait(timeout=10)
        server.terminate()
        server.wait(timeout=10)
