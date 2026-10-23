#!/usr/bin/env python3
"""Deterministic producer/process failures; never used by the application."""
import json, os, pathlib, subprocess, sys, time
mode = sys.argv[1]
if mode == 'sleep':
    time.sleep(10)
elif mode == 'descendant':
    subprocess.Popen([sys.executable, '-c', 'import time,pathlib,sys;time.sleep(1);pathlib.Path(sys.argv[1]).touch()', sys.argv[2]])
    time.sleep(10)
elif mode == 'overflow':
    sys.stdout.write('x' * (33 * 1024 * 1024))
elif mode == 'stderr-overflow':
    sys.stderr.write('x' * 70000)
elif mode == 'malformed':
    print('{invalid')
elif mode == 'error':
    print(json.dumps({'schema':'sdptool/0.2','error':{'code':'stale','message':'Source changed; refresh'}}),file=sys.stderr)
    sys.exit(1)
elif mode == 'fd-is-closed':
    try:
        os.fstat(int(sys.argv[2]))
    except OSError:
        print(json.dumps({'schema':'sdptool/0.2','operation':'select','entry':'/tmp/closed.md', 'directory':'/tmp','revision':'abc'}))
    else:
        print('Inherited application descriptor',file=sys.stderr)
        sys.exit(1)
elif mode == 'closed-streams':
    os.close(1)
    os.close(2)
    time.sleep(10)
elif mode == 'success':
    print(json.dumps({'schema':'sdptool/0.2','operation':'select','entry':sys.argv[2], 'directory':'/tmp/output','revision':'abc'}))
else:
    sys.exit(2)
