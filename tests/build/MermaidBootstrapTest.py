#!/usr/bin/env python3
"""Exercise source integrity and non-destructive bootstrap in an isolated root."""
import hashlib
import io
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile
import tempfile
import unittest


class MermaidBootstrapTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        for name in ('tools', 'cmake', 'third_party'):
            (self.root / name).mkdir()
        shutil.copyfile(Path(__file__).resolve().parents[2] / 'tools/bootstrap_mermaid.py',
                        self.root / 'tools/bootstrap_mermaid.py')
        self.source = {'repository': 'unused/offline', 'commit': 'test-commit',
                       'archive': 'third_party/source.tar.xz'}
        self.archive = self.root / self.source['archive']
        self.dest = self.root / '.deps/mermaid-rs-renderer'
        self.make_archive('raise SystemExit(0)\n')

    def make_archive(self, verifier):
        with tarfile.open(self.archive, 'w:xz') as archive:
            data = verifier.encode()
            entry = tarfile.TarInfo('mermaid-rs-renderer-test-commit/tools/verify_libavoid.py')
            entry.size = len(data)
            archive.addfile(entry, io.BytesIO(data))
        self.source['sha256'] = hashlib.sha256(self.archive.read_bytes()).hexdigest()
        self.write_manifest()

    def write_manifest(self):
        (self.root / 'cmake/mermaid-source.json').write_text(json.dumps(self.source))

    def run_bootstrap(self):
        return subprocess.run([sys.executable, str(self.root / 'tools/bootstrap_mermaid.py')],
                              capture_output=True, text=True)

    def test_extract_and_idempotent_preservation(self):
        self.assertEqual(self.run_bootstrap().returncode, 0)
        self.assertEqual((self.dest / '.xfmd-pin').read_text(),
                         self.source['commit'] + ':' + self.source['sha256'])
        (self.dest / 'local-note').write_text('preserve')
        self.assertEqual(self.run_bootstrap().returncode, 0)
        self.assertEqual((self.dest / 'local-note').read_text(), 'preserve')

    def test_bad_checksum_does_not_install(self):
        self.archive.write_bytes(self.archive.read_bytes() + b'corruption')
        result = self.run_bootstrap()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('checksum mismatch', result.stderr)
        self.assertFalse(self.dest.exists())

    def test_remote_manifest_still_uses_verified_archive(self):
        del self.source['archive']
        self.write_manifest()
        # Substitute transport only: the real checksum, extraction and verifier run.
        program = '''
import io, pathlib, runpy, sys
from unittest.mock import patch
root = pathlib.Path(sys.argv[1])
with patch('urllib.request.urlopen', return_value=io.BytesIO(
        (root / 'third_party/source.tar.xz').read_bytes())) as download:
    runpy.run_path(str(root / 'tools/bootstrap_mermaid.py'), run_name='__main__')
    assert download.call_count == 1
    assert download.call_args.args[0].endswith('/tar.gz/test-commit')
'''
        result = subprocess.run([sys.executable, '-c', program, str(self.root)],
                                capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue((self.dest / '.xfmd-pin').exists())

    def test_failed_vendor_verification_does_not_install(self):
        self.make_archive('raise SystemExit(1)\n')
        self.assertNotEqual(self.run_bootstrap().returncode, 0)
        self.assertFalse(self.dest.exists())

    def test_existing_unknown_or_old_source_is_preserved(self):
        self.dest.mkdir(parents=True)
        (self.dest / 'local-note').write_text('preserve')
        for marker in (None, 'old-pin'):
            if marker:
                (self.dest / '.xfmd-pin').write_text(marker)
            result = self.run_bootstrap()
            self.assertNotEqual(result.returncode, 0)
            self.assertIn('preserve/move', result.stderr)
            self.assertEqual((self.dest / 'local-note').read_text(), 'preserve')

    def test_archive_outside_root_is_rejected(self):
        self.source['archive'] = '../outside.tar.xz'
        self.write_manifest()
        result = self.run_bootstrap()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('inside the repository', result.stderr)
        self.assertFalse(self.dest.exists())


if __name__ == '__main__':
    unittest.main()
