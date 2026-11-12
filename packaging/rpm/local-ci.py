#!/usr/bin/env python3
"""Build, verify and optionally publish one exact committed XFMD revision locally."""
import argparse
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
BASE_IMAGE = 'docker.io/library/almalinux@sha256:ba31c3299856068f77bc10574cad51b0a4f6a3dafb882668656e1639bee63db6'


def run(args, **kwargs):
    return subprocess.run(list(map(str, args)), check=True, **kwargs)


def git(*args):
    return subprocess.check_output(['git', '-C', str(ROOT), *args], text=True).strip()


def metadata(ref):
    commit = git('rev-parse', '--verify', '--end-of-options', ref + '^{commit}')
    product = git('show', commit + ':VERSION').strip()
    if not re.fullmatch(r'(0|[1-9][0-9]*)\.(0|[1-9][0-9]*)', product):
        raise ValueError('Invalid product version')
    if git('rev-parse', '--is-shallow-repository') != 'false':
        raise ValueError('Full history required')
    count = int(git('rev-list', '--first-parent', '--count', commit))
    version, release = product + '.0', str(count) + '.el10_2'
    return dict(commit=commit, display=product, version=version, release=release,
                package_version=version + '-' + release, channel='stable')


def verification_container():
    return ['podman', 'run', '--rm', '--security-opt', 'label=disable', '--shm-size=512m',
            '--tmpfs', '/tmp:rw,exec,mode=1777', '-v', shutil.which('Xvfb')+':/usr/local/bin/Xvfb:ro']


def verified_existing(output, values, producer_pin):
    source = output/'source'
    verified = json.loads((output/'verified.json').read_text())
    provenance = json.loads((output/'provenance.json').read_text())
    expected = f"xfmd-{values['package_version']}.x86_64.rpm"
    if (verified.get('commit') != values['commit'] or verified.get('rpm') != expected or
            provenance.get('commit') != values['commit'] or
            provenance.get('producer_pin') != producer_pin or
            provenance.get('package_version') != values['package_version'] or
            hashlib.sha256((output/expected).read_bytes()).hexdigest() != verified.get('sha256')):
        raise ValueError('Existing RPM does not match exact verified candidate/provenance')
    actual = subprocess.check_output(['git', '-C', source, 'rev-parse', 'HEAD'], text=True).strip()
    dirty = subprocess.check_output(['git', '-C', source, 'status', '--porcelain'], text=True).strip()
    if actual != values['commit'] or dirty:
        raise ValueError('Existing publication source is not the clean exact candidate')
    if json.loads((source/'cmake/sdptool-source.json').read_text()) != producer_pin:
        raise ValueError('Existing source producer pin differs from candidate')
    metadata = json.loads((output/'build-metadata.json').read_text())
    producer = json.loads((output/'sdptool-build.json').read_text())
    if (metadata.get('commit') != values['commit'] or metadata.get('dirty') is not False or
            metadata.get('display') != values['display'] or
            producer.get('commit') != producer_pin['commit'] or
            producer.get('sha256') != producer_pin['sha256'] or
            producer.get('version') != producer_pin['version']):
        raise ValueError('Existing application/producer identity differs from candidate')
    return verified


def publish_verified(args, values, output, source, container, verified, producer_pin):
    expected = verified['rpm']
    path = str(args.known_hosts).replace('%', '%%').replace('\\', '\\\\').replace('"', '\\"')
    ssh = ['ssh', '-o', 'BatchMode=yes', '-o', 'StrictHostKeyChecking=yes', '-o', 'UserKnownHostsFile="'+path+'"',
           '-p', str(args.ssh_port), args.host,
           'sudo -n -u xfmd-publish env SSH_ORIGINAL_COMMAND="publish stable" /usr/local/libexec/xfmd-receive']
    with (output/'publish.log').open('w') as log:
        tar = subprocess.Popen(['tar', '-C', str(output), '-cf', '-', '--', expected], stdout=subprocess.PIPE)
        try:
            run(ssh, stdin=tar.stdout, stdout=log, stderr=subprocess.STDOUT)
        finally:
            tar.stdout.close()
            tar_status = tar.wait()
        if tar_status:
            raise RuntimeError('Upload archive failed')
    signed = output/'signed'
    signed.mkdir(exist_ok=True)
    url = 'https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/stable/x86_64/Packages/'+expected
    run(['curl', '-fsSL', '--retry', '3', url, '-o', signed/expected])
    identity_format = '%{NAME}|%{VERSION}|%{RELEASE}|%{ARCH}|%{PAYLOADDIGEST}'
    original = subprocess.check_output(['rpm', '-qp', '--qf', identity_format, (output/expected)], stderr=subprocess.DEVNULL)
    downloaded = subprocess.check_output(['rpm', '-qp', '--qf', identity_format, signed/expected], stderr=subprocess.DEVNULL)
    if original != downloaded:
        raise RuntimeError('Signed download differs from the verified RPM payload')
    with tempfile.TemporaryDirectory(prefix='xfmd-rpm-trust-') as trust:
        run(['rpmkeys', '--dbpath', trust, '--import', source/'packaging/repository/RPM-GPG-KEY-beinerud'])
        report = subprocess.check_output(['rpmkeys', '--dbpath', trust, '--checksig', '--verbose', signed/expected], text=True, env=dict(os.environ, LC_ALL='C'))
        if not re.search(r'Signature.*: OK$', report, re.M):
            raise RuntimeError('Repository signature missing or invalid')
        (output/'signature.txt').write_text(report)
    with (output/'public-dnf.log').open('w') as log:
        run([*container, '-v', str(source/'packaging')+':/probe:ro', BASE_IMAGE, 'bash',
             '/probe/rpm/verify-public.sh', values['package_version'], values['display'], values['commit'], producer_pin['commit']], stdout=log, stderr=subprocess.STDOUT)
    (output/'published.json').write_text(json.dumps(dict(verified, url=url,
        signed_sha256=hashlib.sha256((signed/expected).read_bytes()).hexdigest()), indent=2)+'\n')
    print('Published; signature, public DNF and installed application verified:', url, flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ref', default='HEAD')
    parser.add_argument('--main', action='store_true', help='Use the main publication version contract')
    parser.add_argument('--publish', action='store_true')
    parser.add_argument('--publish-existing', action='store_true', help='Publish verified --output bytes without rebuilding')
    parser.add_argument('--plan', action='store_true')
    parser.add_argument('--engine', choices=['podman'], default='podman')
    parser.add_argument('--host', default='beinerud')
    parser.add_argument('--ssh-port', type=int, default=22)
    parser.add_argument('--known-hosts', type=Path, default=Path.home()/'.ssh/known_hosts')
    parser.add_argument('--output', type=Path)
    parser.add_argument('--cache', type=Path, default=Path.home()/'.cache/xfmd-build')
    args = parser.parse_args()
    if args.publish_existing and not args.output:
        parser.error('--publish-existing requires --output')
    args.publish = args.publish or args.publish_existing
    if not re.fullmatch(r'[A-Za-z0-9_.@-]+', args.host) or args.host.startswith('-'):
        parser.error('Invalid SSH host')
    if not 1 <= args.ssh_port <= 65535:
        parser.error('Invalid SSH port')
    known_hosts = args.known_hosts.expanduser().resolve()
    if not known_hosts.is_file() or any(c in str(known_hosts) for c in '\r\n\x00'):
        parser.error('Expected verified known_hosts file')
    args.known_hosts = known_hosts
    values = metadata(args.ref)
    producer_pin = json.loads(git('show', values['commit'] + ':cmake/sdptool-source.json'))
    output = (args.output or ROOT/'build/local-ci'/values['commit'][:12]).resolve()
    cache = args.cache.expanduser().resolve()
    plan = dict(values, producer_pin=producer_pin, source_ref=args.ref, output=str(output), cache=str(cache), publish=args.publish, publish_existing=args.publish_existing,
                base_image=BASE_IMAGE, build_host=os.uname().nodename)
    print(json.dumps(plan, indent=2), flush=True)
    if args.plan:
        return
    if output.exists() and not args.publish_existing:
        parser.error('Output exists; choose a fresh output directory')
    for command in ['podman', 'git', 'ssh', 'curl', 'rpmkeys', 'rpm', 'tar', 'Xvfb']:
        if not shutil.which(command):
            parser.error('Missing command: ' + command)
    if args.publish_existing:
        cache.mkdir(parents=True, exist_ok=True)
        with (cache/'build.lock').open('a') as lock:
            fcntl.flock(lock, fcntl.LOCK_EX)
            verified = verified_existing(output, values, producer_pin)
            publish_verified(args, values, output, output/'source', verification_container(), verified, producer_pin)
        return
    output.mkdir(parents=True)
    cache.mkdir(parents=True, exist_ok=True)
    # Serialize shared Cargo outputs, including manual runs versus the timer.
    with (cache/'build.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        source = output/'source'
        run(['git', 'clone', '--no-hardlinks', '--no-checkout', ROOT, source], stdout=subprocess.DEVNULL)
        run(['git', '-C', source, 'checkout', '--detach', values['commit']], stdout=subprocess.DEVNULL)
        if json.loads((source/'cmake/sdptool-source.json').read_text()) != producer_pin:
            raise ValueError('Cloned producer pin differs from selected commit')
        recipe = source/'packaging/rpm/Containerfile'
        recipe_hash = hashlib.sha256(recipe.read_bytes()).hexdigest()
        image = 'localhost/xfmd-rpm-builder:' + recipe_hash[:16]
        with (output/'image.log').open('w') as log:
            run(['podman', 'build', '-t', image, '-f', recipe, recipe.parent], stdout=log, stderr=subprocess.STDOUT)
        plan['xvfb_sha256'] = hashlib.sha256(Path(shutil.which('Xvfb')).read_bytes()).hexdigest()
        plan['builder_image'] = subprocess.check_output(['podman', 'image', 'inspect', image, '--format', '{{.Id}}'], text=True).strip()
        (output/'provenance.json').write_text(json.dumps(plan, indent=2)+'\n')
        container = verification_container()
        with (output/'build.log').open('w') as log:
            print('Building and testing; log:', log.name, flush=True)
            run([*container, '-v', str(source)+':/source:ro', '-v', str(output)+':/output:rw',
                 '-v', str(cache)+':/cache:rw', image, 'bash', '/source/packaging/rpm/container-build.sh',
                 values['version'], values['release']], stdout=log, stderr=subprocess.STDOUT)
        rpms = list(output.glob('*.rpm'))
        expected = f"xfmd-{values['package_version']}.x86_64.rpm"
        if len(rpms) != 1 or rpms[0].name != expected:
            raise RuntimeError('Unexpected RPM output')
        verify_input = output/'verify-input'
        verify_input.mkdir()
        shutil.copy2(rpms[0], verify_input/expected)
        predecessor = json.loads((source/'packaging/rpm/predecessor.json').read_text())
        if not re.fullmatch(r'xfmd-[0-9.]+-[0-9]+[.]el10_2[.]x86_64[.]rpm', predecessor['rpm']):
            raise ValueError('Invalid pinned predecessor identity')
        expected_url = 'https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/stable/x86_64/Packages/' + predecessor['rpm']
        if predecessor['url'] != expected_url:
            raise ValueError('Unexpected predecessor download origin')
        predecessor_file = verify_input/'previous-xfmd.rpm'
        run(['curl', '-fsSL', '--retry', '3', predecessor['url'], '-o', predecessor_file])
        if hashlib.sha256(predecessor_file.read_bytes()).hexdigest() != predecessor['signed_sha256']:
            raise ValueError('Published predecessor digest mismatch')
        shutil.copy2(source/'packaging/repository/RPM-GPG-KEY-beinerud', verify_input)
        shutil.copy2(source/'packaging/rpm/predecessor.json', output)
        shutil.copy2(source/'packaging/rpm/predecessor.json', verify_input)
        with (output/'verify.log').open('w') as log:
            print('Verifying clean installation; log:', log.name, flush=True)
            run([*container, '-v', str(verify_input)+':/packages:ro', '-v', str(source/'packaging/rpm')+':/probe:ro',
                 BASE_IMAGE, 'bash', '/probe/verify-install.sh', expected, values['display'], values['commit'], producer_pin['commit']], stdout=log, stderr=subprocess.STDOUT)
        verified = dict(commit=values['commit'], rpm=expected, sha256=hashlib.sha256(rpms[0].read_bytes()).hexdigest())
        (output/'verified.json').write_text(json.dumps(verified, indent=2)+'\n')
        if not args.publish:
            print('Local build, tests and clean RPM verification passed.', flush=True)
            return
        publish_verified(args, values, output, source, container, verified, producer_pin)


if __name__ == '__main__':
    main()
