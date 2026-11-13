#!/usr/bin/env bash
# Install a user-owned local builder. Activation is explicit via --enable.
set -euo pipefail
exec python3 - "$0" "$@" <<'PY'
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

parser = argparse.ArgumentParser(description='Install the isolated xfmd main-branch builder and user timer. Default: install files only; --enable starts automatic publication.')
parser.add_argument('--state-dir', type=Path, default=Path.home()/'.local/state/xfmd-builder')
parser.add_argument('--cache', type=Path, default=Path.home()/'.cache/xfmd-build')
parser.add_argument('--unit-dir', type=Path, default=Path.home()/'.config/systemd/user')
parser.add_argument('--repository', default='https://github.com/Hans-Einar/xfmd.git')
parser.add_argument('--host', default='beinerud')
parser.add_argument('--ssh-port', type=int, default=22)
parser.add_argument('--known-hosts', type=Path)
parser.add_argument('--shared-engine', type=Path)
parser.add_argument('--shared-engine-commit')
parser.add_argument('--shared-engine-sha256')
parser.add_argument('--enable', action='store_true', help='Reload user units and enable/start the timer after installation')
args = parser.parse_args(sys.argv[2:])
if os.geteuid() == 0:
    parser.error('Run as the normal build user, not root')
if not 1 <= args.ssh_port <= 65535:
    parser.error('--ssh-port must be between 1 and 65535')
source = Path(sys.argv[1]).resolve().parent
known_hosts = (args.known_hosts or Path.home()/'.ssh/known_hosts').expanduser().resolve()
if not known_hosts.is_file():
    parser.error('--known-hosts must name the verified server host-key file')
state = args.state_dir.expanduser().resolve()
cache = args.cache.expanduser().resolve()
units = args.unit_dir.expanduser().resolve()
for path in [state, cache, units]:
    if any(c in str(path) for c in '\r\n\x00'):
        parser.error('Paths must not contain newline or NUL characters')
    path.mkdir(parents=True, exist_ok=True)
state.chmod(0o700)
# Refuse to replace a running watcher's configuration or executable.
import fcntl
with (state/'builder.lock').open('a') as lock:
    try:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        parser.error('A builder is active; wait for it to finish before reinstalling')
    binary = state/'watch-main.py'
    shutil.copyfile(source/'watch-main.py', binary)
    binary.chmod(0o755)
    trusted = state/'known_hosts'
    if known_hosts != trusted:
        shutil.copyfile(known_hosts, trusted)
    trusted.chmod(0o600)
    config = dict(repository=args.repository, checkout=str(state/'checkout'), cache=str(cache),
                  output=str(state/'output'), host=args.host, ssh_port=args.ssh_port,
                  known_hosts=str(trusted), engine='podman')
    for key in ('shared_engine', 'shared_engine_commit', 'shared_engine_sha256'):
        value = getattr(args, key)
        if value:
            config[key] = str(value.expanduser().resolve()) if isinstance(value, Path) else value
    config_path = state/'config.json'
    temporary = config_path.with_suffix('.tmp')
    temporary.write_text(json.dumps(config, indent=2)+'\n')
    temporary.chmod(0o600)
    temporary.replace(config_path)
    def quote(value):
        return '"'+str(value).replace('%','%%').replace('\\','\\\\').replace('"','\\"')+'"'
    service = f'''[Unit]
Description=Build and publish verified xfmd main commits
After=network-online.target

[Service]
Type=oneshot
ExecStart=/usr/bin/python3 {quote(binary)} --config {quote(config_path)}
Environment=GIT_TERMINAL_PROMPT=0
TimeoutStartSec=infinity
UMask=0077
'''
    timer = '''[Unit]
Description=Check xfmd main for a new release candidate

[Timer]
OnBootSec=1min
OnUnitInactiveSec=1min
AccuracySec=5s
Unit=xfmd-builder.service

[Install]
WantedBy=timers.target
'''
    (units/'xfmd-builder.service').write_text(service)
    (units/'xfmd-builder.timer').write_text(timer)
if args.enable:
    if units != Path.home()/'.config/systemd/user':
        parser.error('--enable requires the default user unit directory')
    subprocess.run(['systemctl','--user','daemon-reload'],check=True)
    subprocess.run(['systemctl','--user','enable','--now','xfmd-builder.timer'],check=True)
print('Installed builder config:', config_path)
print('Build cache:', cache)
print('Pipeline logs:', state/'logs')
if not args.enable:
    print('Not activated. Start explicitly: systemctl --user daemon-reload && systemctl --user enable --now xfmd-builder.timer')
print('For builds after logout/reboot, enable user lingering: sudo loginctl enable-linger '+os.getlogin() if os.isatty(0) else 'For builds after logout/reboot: sudo loginctl enable-linger "$USER"')
print('Retry a failed candidate:', binary, '--config', config_path, '--retry')
PY
