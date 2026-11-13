#!/usr/bin/env python3
"""Check RPM ordering and provenance using real disposable Git history."""
import importlib.util
import hashlib
import json
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


class ExistingPublicationTests(unittest.TestCase):
    def test_exact_verified_bytes_and_clean_source_are_required(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)
            source = output/'source'
            (source/'cmake').mkdir(parents=True)
            pin = dict(commit='a'*40, sha256=hashlib.sha256(b'shared engine').hexdigest(), version='2.3.0')
            (output/'shared-engine').mkdir()
            (output/'shared-engine/gh-sdp').write_bytes(b'shared engine')
            (source/'VERSION').write_text('0.10\n')
            def git(*args):
                return subprocess.check_output(['git', '-C', str(source), *args], text=True, stderr=subprocess.DEVNULL).strip()
            git('init', '-b', 'feature')
            git('config', 'user.name', 'Publication test')
            git('config', 'user.email', 'publication@example.invalid')
            git('config', 'commit.gpgsign', 'false')
            git('add', '.'); git('commit', '-m', 'Exact candidate')
            with patch.object(ci, 'ROOT', source): values = ci.metadata('HEAD')
            rpm = output/f"xfmd-{values['package_version']}.x86_64.rpm"
            payload = b'exact locally verified package bytes'
            rpm.write_bytes(payload)
            records = {
                'verified.json': dict(commit=values['commit'], rpm=rpm.name, sha256=hashlib.sha256(payload).hexdigest()),
                'provenance.json': dict(commit=values['commit'], package_version=values['package_version'], shared_identity=pin),
                'build-metadata.json': dict(commit=values['commit'], dirty=False, display='0.10'),
                'shared-engine.json': pin,
            }
            probes = output/'native-probes'
            probes.mkdir()
            for name in ('SdpReadingGuiTest', 'SdpActivityGuiTest'): (probes/name).write_bytes(b'verified native probe')
            records['native-probes.json'] = {p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in probes.iterdir()}
            for name, record in records.items(): (output/name).write_text(json.dumps(record))
            self.assertEqual(ci.verified_existing(output, values, pin), records['verified.json'])
            rpm.write_bytes(payload + b'changed')
            with self.assertRaises(ValueError): ci.verified_existing(output, values, pin)
            rpm.write_bytes(payload)
            for name, field, value in [('provenance.json','commit','other'),
                                       ('build-metadata.json','dirty',True),
                                       ('shared-engine.json','commit','other')]:
                invalid = dict(records[name], **{field:value})
                (output/name).write_text(json.dumps(invalid))
                with self.assertRaises(ValueError): ci.verified_existing(output, values, pin)
                (output/name).write_text(json.dumps(records[name]))
            (output/'shared-engine/gh-sdp').write_bytes(b'tampered engine')
            with self.assertRaises(ValueError): ci.verified_existing(output, values, pin)
            (output/'shared-engine/gh-sdp').write_bytes(b'shared engine')
            (source/'VERSION').write_text('0.11\n')
            with self.assertRaises(ValueError): ci.verified_existing(output, values, pin)


if __name__ == '__main__':
    unittest.main()
