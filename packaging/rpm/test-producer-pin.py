#!/usr/bin/env python3
"""Verify producer archive identity and hostile-member rejection before builds."""
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import subprocess
import tarfile
import tempfile
import unittest

spec = importlib.util.spec_from_file_location('producer', Path(__file__).with_name('build-producer.py'))
producer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(producer)


class ProducerPinTests(unittest.TestCase):
    def test_committed_archive_and_digest_are_both_required(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            def git(*args, **kwargs):
                return subprocess.check_output(['git', '-C', str(root), *args], stderr=subprocess.DEVNULL, **kwargs)
            git('init', '-b', 'feature')
            git('config', 'user.name', 'Pin test')
            git('config', 'user.email', 'pin@example.invalid')
            git('config', 'commit.gpgsign', 'false')
            (root/'SDPTool').mkdir()
            (root/'SDPTool/input.go').write_text('package example\n')
            git('add', '.')
            git('commit', '-m', 'Exact producer')
            commit = git('rev-parse', 'HEAD', text=True).strip()
            (root/'third_party').mkdir()
            archive = root/'third_party'/('sdptool-' + commit + '.tar.gz')
            archive.write_bytes(git('archive', '--format=tar.gz', commit, 'SDPTool'))
            pin = dict(repository='Hans-Einar/SDP', commit=commit, version='2.2.0-dev',
                       archive=str(archive.relative_to(root)), sha256=hashlib.sha256(archive.read_bytes()).hexdigest())
            (root/'cmake').mkdir()
            manifest = root/'cmake/sdptool-source.json'
            manifest.write_text(json.dumps(pin))
            self.assertEqual(producer.source_pin(root)[0], pin)
            producer.extract_source(archive, root/'extract', commit)
            self.assertTrue((root/'extract/SDPTool/input.go').is_file())
            with self.assertRaises(ValueError): producer.extract_source(archive, root/'wrong', '0'*40)
            archive.write_bytes(archive.read_bytes() + b'changed')
            with self.assertRaises(ValueError): producer.source_pin(root)

    def test_archive_rejects_links_and_escaping_members(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name, kind in [('SDPTool/link', tarfile.SYMTYPE), ('../escape', tarfile.REGTYPE),
                               ('SDPTool/../../escape', tarfile.REGTYPE), ('unrelated/file', tarfile.REGTYPE)]:
                archive = root/'bad.tar.gz'
                with tarfile.open(archive, 'w:gz', pax_headers={'comment': 'a'*40}) as out:
                    member = tarfile.TarInfo(name)
                    member.type = kind
                    member.linkname = '/outside'
                    out.addfile(member, io.BytesIO())
                with self.assertRaises(ValueError): producer.extract_source(archive, root/'extract', 'a'*40)


if __name__ == '__main__':
    unittest.main()
