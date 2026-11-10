#!/usr/bin/env python3
"""Exercise watcher publication/failure controls without containers or uploads."""
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location('watch_main', Path(__file__).with_name('watch-main.py'))
watch = importlib.util.module_from_spec(spec)
spec.loader.exec_module(watch)


class WatcherTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.commit = 'a' * 40
        self.tags = 'refs/tags/v1.0.0 tag-object'
        self.version = '0.9.0-20.el10_2'
        self.fail_plan = False
        self.config = dict(checkout=str(self.root/'checkout'), cache=str(self.root/'cache'),
                           output=str(self.root/'output'), host='builder@example.com', ssh_port=2222,
                           known_hosts=str(self.root/'known hosts'))
        self.config['repository'] = 'https://example.com/repository.git'
        self.patches = [patch.object(watch, 'prepare_checkout', side_effect=lambda c: self.commit),
                        patch.object(watch, 'git', side_effect=self.git),
                        patch.object(watch, 'command', side_effect=self.command),
                        patch.object(watch.subprocess, 'run', return_value=subprocess.CompletedProcess([], 0))]
        for p in self.patches:
            self.addCleanup(p.stop)
        self.prepare, self.git_mock, self.command_mock, self.run = [p.start() for p in self.patches]

    def git(self, checkout, *args):
        if args[0] == 'for-each-ref': return self.tags
        if args[0] == 'rev-list': return self.commit + '\n' + 'a'*40
        return ''

    def command(self, args, **kwargs):
        if self.fail_plan: raise RuntimeError('No semantic anchor tag')
        self.assertIn('--plan', args)
        return json.dumps(dict(commit=self.commit, package_version=self.version, channel='stable', publish=True))

    def poll(self, **kwargs):
        with contextlib.redirect_stdout(io.StringIO()):
            return watch.poll(self.config, self.root, **kwargs)

    def state(self): return json.loads((self.root/'state.json').read_text())

    def test_success_is_once_and_contains_exact_pipeline_options(self):
        self.poll()
        state = self.state()
        self.assertEqual(state['last_success']['status'], 'published')
        args = self.run.call_args.args[0]
        self.assertEqual(self.run.call_args.kwargs['umask'], 0o022)
        environment = self.run.call_args.kwargs['env']
        self.assertEqual(environment['GIT_TERMINAL_PROMPT'], '0')
        self.assertEqual(environment['GIT_ASKPASS'], '/bin/false')
        self.assertEqual(environment['SSH_ASKPASS'], '/bin/false')
        self.assertIn('--main', args)
        self.assertIn('--publish', args)
        self.assertEqual(args[args.index('--ref')+1], self.commit)
        self.assertEqual(args[args.index('--known-hosts')+1], self.config['known_hosts'])
        self.assertFalse(Path(state['last_attempt']['output']).exists()) # Pipeline creates it, not watcher.
        self.poll()
        self.poll(retry=True) # Retrying an already published commit is a no-op.
        self.run.assert_called_once()

    def test_failed_build_waits_for_retry_then_new_commits_advance(self):
        self.run.return_value = subprocess.CompletedProcess([], 9)
        with self.assertRaisesRegex(RuntimeError, 'exited 9'): self.poll()
        self.assertNotIn('last_success', self.state())
        self.poll()
        self.run.assert_called_once()
        self.run.return_value = subprocess.CompletedProcess([], 0)
        self.poll(retry=True)
        self.assertEqual(self.run.call_count, 2)
        self.commit = 'b'*40
        self.version = '0.9.0-21.el10_2'
        self.poll()
        self.assertEqual(self.state()['last_success']['commit'], self.commit)

    def test_late_tag_recovers_planning_failure_without_new_commit(self):
        self.fail_plan = True
        with self.assertRaisesRegex(RuntimeError, 'anchor'): self.poll()
        self.poll() # No repeated plan failure while refs are unchanged.
        self.assertEqual(self.command_mock.call_count, 1)
        self.tags += '\nrefs/tags/v1.0.0 new-anchor'
        self.fail_plan = False
        self.poll()
        self.run.assert_called_once()
        self.assertEqual(self.state()['last_success']['version'], self.version)

    def test_unrelated_tag_does_not_retry_failed_same_version(self):
        self.run.return_value = subprocess.CompletedProcess([], 9)
        with self.assertRaises(RuntimeError): self.poll()
        self.tags += '\nrefs/tags/unrelated tag'
        self.poll()
        self.run.assert_called_once()
        self.assertEqual(self.state()['last_attempt']['status'], 'failed')

    def test_backward_version_and_rewritten_main_cannot_publish(self):
        self.poll()
        self.commit = 'b'*40
        with self.assertRaisesRegex(RuntimeError, 'advance'): self.poll()
        self.run.assert_called_once()
        self.commit = 'c'*40
        self.git_mock.side_effect = lambda checkout, *args: self.tags if args[0] == 'for-each-ref' else self.commit
        with self.assertRaisesRegex(RuntimeError, 'rewritten'): self.poll()
        self.run.assert_called_once()

    def test_plan_never_records_or_builds(self):
        self.poll(plan_only=True)
        self.assertFalse((self.root/'state.json').exists())
        self.run.assert_not_called()

    def test_another_lock_holder_prevents_any_work(self):
        config = self.root/'config.json'
        config.write_text(json.dumps(self.config))
        with (self.root/'builder.lock').open('a') as lock:
            watch.fcntl.flock(lock, watch.fcntl.LOCK_EX | watch.fcntl.LOCK_NB)
            with patch('sys.argv', ['watch-main', '--config', str(config)]), contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(watch.main(), 0)
        self.prepare.assert_not_called()


class CheckoutTests(unittest.TestCase):
    def test_git_commands_disable_terminal_and_graphical_password_prompts(self):
        with patch.object(watch.subprocess, 'check_output', return_value='ok') as execute:
            self.assertEqual(watch.command(['git', 'fetch']), 'ok')
        environment = execute.call_args.kwargs['env']
        self.assertEqual(environment['GIT_TERMINAL_PROMPT'], '0')
        self.assertEqual(environment['GIT_ASKPASS'], '/bin/false')
        self.assertEqual(environment['SSH_ASKPASS'], '/bin/false')

    def test_dedicated_clone_fetch_and_dirty_checkout_protection(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            remote, checkout = root/'remote', root/'checkout'
            subprocess.run(['git', 'init', '-q', '-b', 'main', str(remote)], check=True)
            def git(*args):
                return subprocess.check_output(['git', '-C', str(remote), *args], text=True).strip()
            git('config', 'user.email', 'test@example.com')
            git('config', 'user.name', 'Test')
            (remote/'source').write_text('first')
            git('add', 'source'); git('commit', '-qm', 'first'); git('tag', 'v1.0.0')
            config = dict(repository=str(remote), checkout=str(checkout))
            self.assertEqual(watch.prepare_checkout(config), git('rev-parse', 'HEAD'))
            (remote/'source').write_text('second')
            git('commit', '-qam', 'second')
            self.assertEqual(watch.prepare_checkout(config), git('rev-parse', 'HEAD'))
            (checkout/'source').write_text('local edit')
            with self.assertRaisesRegex(RuntimeError, 'local changes'):
                watch.prepare_checkout(config)
            self.assertEqual((checkout/'source').read_text(), 'local edit')


class InstallerTests(unittest.TestCase):
    def test_install_generates_isolated_units_without_activation(self):
        with tempfile.TemporaryDirectory(prefix='builder install ') as temporary:
            root = Path(temporary)
            command = [str(Path(__file__).with_name('install-builder.sh')),
                       '--state-dir', str(root/'state'), '--cache', str(root/'cache'),
                       '--unit-dir', str(root/'units')]
            result = subprocess.run(command, check=True, capture_output=True, text=True)
            self.assertIn('Not activated', result.stdout)
            config = json.loads((root/'state/config.json').read_text())
            self.assertEqual(config['checkout'], str(root/'state/checkout'))
            self.assertEqual(config['cache'], str(root/'cache'))
            service = (root/'units/xfmd-builder.service').read_text()
            self.assertIn('TimeoutStartSec=infinity', service)
            self.assertIn('"'+str(root/'state/watch-main.py')+'"', service)
            self.assertIn('OnUnitInactiveSec=1min', (root/'units/xfmd-builder.timer').read_text())
            subprocess.run(command, check=True, capture_output=True) # Repeat installation is safe.


if __name__ == '__main__': unittest.main()
