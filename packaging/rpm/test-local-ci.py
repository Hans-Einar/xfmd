#!/usr/bin/env python3
"""Check RPM ordering and provenance using real disposable Git history."""
import importlib.util
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location('local_ci', Path(__file__).with_name('local-ci.py'))
ci = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ci)


class VersionTests(unittest.TestCase):
    def test_merge_counts_once_and_keeps_exact_candidate(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            def git(*args):
                return subprocess.check_output(['git', '-C', str(root), *args], text=True, stderr=subprocess.DEVNULL).strip()
            git('init', '-b', 'main')
            git('config', 'user.name', 'CI Test')
            git('config', 'user.email', 'ci@example.invalid')
            git('config', 'commit.gpgsign', 'false')
            (root/'VERSION').write_text('0.9\n')
            git('add', 'VERSION'); git('commit', '-m', 'Initial')
            with patch.object(ci, 'ROOT', root):
                first = ci.metadata('HEAD')
                self.assertEqual(first['package_version'], '0.9.0-1.el10_2')
                git('switch', '-c', 'feature')
                git('commit', '--allow-empty', '-m', 'One')
                git('commit', '--allow-empty', '-m', 'Two')
                git('switch', 'main')
                git('merge', '--no-ff', 'feature', '-m', 'Integrate')
                merged = ci.metadata('HEAD')
                self.assertEqual(merged['package_version'], '0.9.0-2.el10_2')
                self.assertEqual(merged['commit'], git('rev-parse', 'HEAD'))
                (root/'VERSION').write_text('9.9\n')
                self.assertEqual(ci.metadata('HEAD'), merged)  # Working edits never enter a release.
                self.assertEqual(ci.metadata(first['commit']), first)
                git('add', 'VERSION'); git('commit', '-m', 'New series')
                self.assertEqual(ci.metadata('HEAD')['package_version'], '9.9.0-3.el10_2')
                (root/'VERSION').write_text('invalid\n')
                git('add', 'VERSION'); git('commit', '-m', 'Invalid')
                with self.assertRaises(ValueError): ci.metadata('HEAD')


if __name__ == '__main__':
    unittest.main()
