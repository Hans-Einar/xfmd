#!/usr/bin/env python3
"""Probe the installed executable on an isolated display with installed resources."""
import ctypes as C
import hashlib
import json
import os
from pathlib import Path
import subprocess
import signal
import shutil
import socket
import sys
import tempfile
import time
import uuid

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
assert not Path('/usr/libexec/xfmd/sdptool').exists(), 'Upgrade retained private engine'
assert not shutil.which('gh'), 'Qualification must run with gh absent'
try:
    with socket.create_connection(('1.1.1.1', 443), timeout=1):
        raise AssertionError('Qualification network namespace permits external networking')
except OSError:
    pass
producer_metadata = json.loads(Path('/shared-engine.json').read_text())
assert producer_metadata['commit'] == sys.argv[3], producer_metadata
engine = Path('/shared-engine/gh-sdp')
assert hashlib.sha256(engine.read_bytes()).hexdigest() == producer_metadata['sha256']
producer_identity = json.loads(subprocess.check_output([engine, '--version', '--json'], text=True))
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
        # Establish the public entrypoint through the real engine setup operation.
        # The bundle is test input, deliberately outside the XFMD package.
        environment.pop('XFMD_SDP_TOOL', None)
        for key in list(environment):
            if key.startswith(('GH_', 'GITHUB_')):
                environment.pop(key)
        subprocess.run([engine, 'setup'], env=environment, check=True)
        producer = base/'.local/bin/sdptool'
        assert producer.is_symlink() and producer.resolve() == engine.resolve()
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
                if ('execve("' + str(producer) + '"') in observed and '"discover"' in observed:
                    break
                time.sleep(.25)
            else:
                raise AssertionError('Installed native application did not invoke registered shared runtime: ' + observed)
            print('Installed native registered shared runtime:', producer_identity['revision'])
        finally:
            if native.poll() is None:
                os.killpg(native.pid, signal.SIGTERM)
                native.wait(timeout=10)

        # Native tools are exact-candidate test artifacts, outside the RPM. They
        # exercise the installed producer and installed private FOX/resources.
        workflow_environment = dict(environment, LD_LIBRARY_PATH='/usr/lib64/xfmd')
        subprocess.run(['/native-probes/SdpReadingGuiTest'], env=workflow_environment, check=True, timeout=65)
        for folder in ['ProjectManagement', 'KanBan']:
            (project/'SDP'/folder).mkdir(exist_ok=True)
        (project/'SDP/KanBan/board.json').write_text(json.dumps({'projectId':'RPM'}))
        (project/'SDP/owner.md').write_text('# Disposable package verification authority\nCreate the isolated native Activity fixture.\n')
        def activity(*args):
            return json.loads(subprocess.check_output([producer, str(project), 'activity', *args, '--json'], text=True))
        catalog = activity('list')['data']
        request = dict(operationId=str(uuid.uuid4()), expectedHistoryRevision=catalog['historyRevision'],
                       title='RPM native workflow', owner='project-owner', mandate='Verify installed Activity rendering.',
                       scope=['SDL'], reason='Disposable package verification fixture')
        request_file = base/'create-activity.json'
        request_file.write_text(json.dumps(request))
        created = activity('create', '--request', str(request_file), '--authority-source', 'owner.md')['data']
        assert created['activityId']
        activity_environment = dict(workflow_environment, XFMD_ACTIVITY_REAL_PROJECT=str(project),
                                    XFMD_ACTIVITY_REAL_GROUP='Planned')
        subprocess.run(['/native-probes/SdpActivityGuiTest'], env=activity_environment, check=True, timeout=65)
        print('Installed native reading/render/source/history and real Activity workflows verified:', producer_identity['revision'])

    finally:
        if application is not None and application.poll() is None:
            application.terminate()
            application.wait(timeout=10)
        server.terminate()
        server.wait(timeout=10)
