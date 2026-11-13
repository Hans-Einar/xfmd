#!/usr/bin/env python3
"""Build the newest origin/main commit using the verified local RPM pipeline."""
import argparse
from datetime import datetime, timezone
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import uuid


def timestamp():
    return datetime.now(timezone.utc).isoformat()


def save(path, value):
    temporary = path.with_suffix('.tmp')
    temporary.write_text(json.dumps(value, indent=2) + '\n')
    temporary.replace(path)


def command(args, *, timeout=120):
    return subprocess.check_output(list(map(str, args)), text=True, timeout=timeout,
                                   env=dict(os.environ, GIT_TERMINAL_PROMPT='0',
                                            GIT_ASKPASS='/bin/false', SSH_ASKPASS='/bin/false')).strip()


def git(checkout, *args):
    return command(['git', '-C', checkout, *args])


def prepare_checkout(config):
    checkout = Path(config['checkout'])
    repository = config['repository']
    if not checkout.exists():
        checkout.parent.mkdir(parents=True, exist_ok=True)
        command(['git', 'clone', '--no-hardlinks', '--branch', 'main', repository, checkout], timeout=180)
    if git(checkout, 'remote', 'get-url', 'origin') != repository:
        raise RuntimeError('Dedicated checkout origin differs from configured repository')
    if git(checkout, 'status', '--porcelain', '--untracked-files=all'):
        raise RuntimeError('Dedicated checkout has unexpected local changes; refusing to overwrite them')
    git(checkout, 'fetch', '--prune', 'origin', '+refs/heads/main:refs/remotes/origin/main', '--tags')
    return git(checkout, 'rev-parse', '--verify', 'refs/remotes/origin/main^{commit}')


def version_tuple(value):
    if not isinstance(value, str) or not re.fullmatch(r'\d+\.\d+\.\d+-[1-9]\d*\.el10_2', value):
        raise RuntimeError('Pipeline returned a non-release version')
    return tuple(map(int, value.removesuffix('.el10_2').replace('-', '.').split('.')))


def pipeline_command(config, commit, output):
    result = [sys.executable, str(Path(config['checkout']) / 'packaging/rpm/local-ci.py'),
              '--main', '--ref', commit, '--cache', config['cache'], '--output', str(output),
              '--publish', '--engine', config.get('engine', 'podman'), '--host', config['host'],
              '--ssh-port', str(config['ssh_port']), '--known-hosts', config['known_hosts']]
    for key in ('shared_engine', 'shared_engine_commit', 'shared_engine_sha256'):
        if config.get(key):
            result.extend(['--' + key.replace('_', '-'), config[key]])
    return result


def poll(config, state_directory, *, retry=False, plan_only=False, lock_fd=None):
    """Called under the state-directory lock. Only a zero-exit pipeline is success."""
    state_file = state_directory / 'state.json'
    state = json.loads(state_file.read_text()) if state_file.exists() else {}
    commit = prepare_checkout(config)
    prior = state.get('last_attempt', {})
    checkout = Path(config['checkout'])
    tag_fingerprint = hashlib.sha256(git(checkout, 'for-each-ref', '--format=%(refname) %(objectname)', 'refs/tags').encode()).hexdigest()
    if prior.get('commit') == commit and prior.get('tag_fingerprint') == tag_fingerprint and (not retry or prior.get('status') == 'published') and not plan_only:
        print(f"No new main commit; previous attempt {prior.get('status')} ({commit[:12]}).", flush=True)
        return 0
    observed = prior.get('commit')
    if observed and observed != commit:
        ancestry = git(checkout, 'rev-list', '--first-parent', commit).splitlines()
        if observed not in ancestry:
            raise RuntimeError('origin/main first-parent history was rewritten; manual reconciliation required')
    git(checkout, 'checkout', '--detach', commit)
    suffix = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ') + '-' + uuid.uuid4().hex[:8]
    output = Path(config['output']) / (commit[:12] + '-' + suffix)
    pipeline = pipeline_command(config, commit, output)
    attempt = dict(commit=commit, tag_fingerprint=tag_fingerprint, started_at=timestamp(), output=str(output), status='planning')
    if not plan_only:
        state['last_attempt'] = attempt
        save(state_file, state)
    try:
        plan = json.loads(command([*pipeline, '--plan'], timeout=60))
        if plan.get('commit') != commit or plan.get('channel') != 'stable' or plan.get('publish') is not True:
            raise RuntimeError('Pipeline plan does not describe the selected stable publication')
        version = plan['package_version']
        version_tuple(version)
        # An unrelated tag must not repeatedly retry an expensive failed build.
        if prior.get('commit') == commit and prior.get('version') == version and (not retry or prior.get('status') == 'published') and not plan_only:
            prior['tag_fingerprint'] = tag_fingerprint
            state['last_attempt'] = prior
            save(state_file, state)
            print(f'No new candidate version; previous attempt {prior.get("status")} ({version}).', flush=True)
            return 0
        previous = state.get('last_success', {})
        if previous and version_tuple(version) <= version_tuple(previous['version']):
            raise RuntimeError('Candidate version does not advance the last successful release')
        if plan_only:
            print(json.dumps(plan, indent=2), flush=True)
            return 0
        logs = state_directory / 'logs'
        logs.mkdir(exist_ok=True)
        log_path = logs / (output.name + '.log')
        attempt.update(version=version, status='building', log=str(log_path))
        save(state_file, state)
        print(f'Building {version} from {commit}; log: {log_path}', flush=True)
        with log_path.open('w') as log:
            # Build and package verification containers must traverse the
            # mounted output root. Keep build directories at
            # normal 0755 permissions even if the service itself uses umask 0077.
            # Inherit the lock so a surviving pipeline also excludes a new build.
            result = subprocess.run(pipeline, stdout=log, stderr=subprocess.STDOUT,
                                    env=dict(os.environ, GIT_TERMINAL_PROMPT='0',
                                            GIT_ASKPASS='/bin/false', SSH_ASKPASS='/bin/false'), umask=0o022,
                                    pass_fds=() if lock_fd is None else (lock_fd,))
        if result.returncode:
            raise RuntimeError(f'Local pipeline exited {result.returncode}; inspect {log_path}')
        attempt.update(status='published', finished_at=timestamp())
        state['last_success'] = dict(attempt)
        save(state_file, state)
        print(f'Published and verified {version}; output: {output}', flush=True)
        return 0
    except Exception as error:
        if not plan_only:
            attempt.update(status='failed', finished_at=timestamp(), error=str(error))
            save(state_file, state)
        raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--retry', action='store_true', help='Retry the current failed/interrupted commit once')
    parser.add_argument('--plan', action='store_true', help='Fetch and inspect the next candidate without building or recording an attempt')
    args = parser.parse_args()
    config_path = args.config.expanduser().resolve()
    config = json.loads(config_path.read_text())
    directory = config_path.parent
    with (directory / 'builder.lock').open('a') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            print('Another build watcher is active; leaving it to finish.', flush=True)
            return 0
        try:
            return poll(config, directory, retry=args.retry, plan_only=args.plan, lock_fd=lock.fileno())
        except Exception as error:
            print(f'Build watcher: {error}', file=sys.stderr, flush=True)
            return 1


if __name__ == '__main__':
    raise SystemExit(main())
