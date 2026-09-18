#!/usr/bin/env python3
"""Exercise history semantics using disposable real Git repositories."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('build_identity', ROOT / 'tools/build_identity.py')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class BuildIdentityTest(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.source = Path(self.folder.name) / 'source'
        self.source.mkdir()
        self.git('init', '-b', 'main')
        self.git('config', 'user.name', 'Version Test')
        self.git('config', 'user.email', 'version@example.invalid')
        self.git('config', 'commit.gpgsign', 'false')
        (self.source / 'VERSION').write_text('0.1\n')
        (self.source / '.gitignore').write_text('/build/\n')
        self.commit('Initial version')

    def git(self, *args):
        return subprocess.check_output(['git', '-C', str(self.source), *args],
                                       text=True, stderr=subprocess.PIPE).strip()

    def commit(self, title):
        self.git('add', '.')
        self.git('commit', '-m', title)

    def identity(self):
        return module.identity(self.source, {})

    def test_branch_and_each_commit_increment(self):
        self.assertEqual(self.identity()['display'], '0.1 main:1')
        self.git('switch', '-c', 'sprint/001/phase/042-build-identity')
        (self.source / 'code').write_text('one')
        self.commit('First milestone')
        a = self.identity()
        self.assertEqual(a['commit_number'], 2)
        self.assertEqual(a['display'], '0.1 sprint/001/phase/042-build-identity:2')
        (self.source / 'code').write_text('two')
        self.commit('Second milestone')
        self.assertEqual(self.identity()['commit_number'], 3)

    def test_merge_and_main_followup_keep_pr(self):
        self.git('switch', '-c', 'phase')
        (self.source / 'code').write_text('one')
        self.commit('Milestone')
        self.git('switch', 'main')
        self.git('merge', '--no-ff', 'phase', '-m', 'Merge pull request #35 from example/phase')
        self.assertEqual(self.identity()['display'], '0.1 PR#35:3')
        (self.source / 'notes').write_text('Documentation')
        self.commit('Document result')
        self.assertEqual(self.identity()['display'], '0.1 PR#35:4')

    def test_dirty_and_ignored_build_files(self):
        (self.source / 'build').mkdir()
        (self.source / 'build' / 'artifact').write_text('ignored')
        self.assertFalse(self.identity()['dirty'])
        (self.source / 'new').write_text('untracked')
        self.assertTrue(self.identity()['display'].endswith('+dirty'))
        self.git('add', 'new')
        self.assertTrue(self.identity()['dirty'])
        self.commit('Track file')
        (self.source / 'new').write_text('changed')
        self.assertTrue(self.identity()['dirty'])

    def test_detached_and_ci_test_checkout(self):
        self.git('checkout', '--detach')
        self.assertEqual(self.identity()['display'], '0.1 detached:1')
        ci = module.identity(self.source, {'GITHUB_ACTIONS': 'true', 'GITHUB_HEAD_REF': 'phase'})
        self.assertEqual(ci['display'], '0.1 ci/phase:1')

    def test_shallow_is_rejected(self):
        target = Path(self.folder.name) / 'shallow'
        subprocess.run(['git', 'clone', '--depth', '1', self.source.as_uri(), str(target)],
                       check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        with self.assertRaisesRegex(ValueError, 'unshallow'):
            module.identity(target, {})

    def test_archive_and_invalid_product_version(self):
        archive = Path(self.folder.name) / 'archive'
        archive.mkdir()
        (archive / 'VERSION').write_text('2.3\n')
        value = module.identity(archive, {})
        self.assertEqual(value['display'], '2.3 source:unknown')
        self.assertIsNone(value['commit_number'])
        (archive / 'VERSION').write_text('2.3.4')
        with self.assertRaises(ValueError):
            module.identity(archive, {})

    def test_metadata_stable_and_refreshed_after_commit(self):
        output = self.source / 'build'
        command = ['python3', str(ROOT / 'tools/build_identity.py'), '--source', str(self.source),
                   '--output', str(output)]
        subprocess.run(command, check=True)
        header = output / 'VersionGenerated.h'
        before = header.stat().st_mtime_ns
        subprocess.run(command, check=True)
        self.assertEqual(header.stat().st_mtime_ns, before)
        (self.source / 'notes').write_text('new milestone')
        self.commit('Update')
        subprocess.run(command, check=True)
        value = json.loads((output / 'xfmd-build.json').read_text())
        self.assertEqual(value['display'], '0.1 main:2')
        self.assertIn('0.1 main:2', header.read_text())
        self.assertEqual(value['commit'], self.git('rev-parse', 'HEAD'))


if __name__ == '__main__':
    unittest.main()
