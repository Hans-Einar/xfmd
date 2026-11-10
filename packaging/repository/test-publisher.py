#!/usr/bin/env python3
"""Exercise release policy and the restricted upload boundary without a server."""
import copy
import io
from pathlib import Path
import runpy
import tarfile
import tempfile
import types
import unittest
from unittest.mock import patch

HERE = Path(__file__).resolve().parent
promote = runpy.run_path(str(HERE / 'xfmd-promote'))
receive = runpy.run_path(str(HERE / 'xfmd-receive'))
validate = promote['validate_identity']


class UploadBoundary(unittest.TestCase):
    valid = ['xfmd-0.9.0-20.el10_2.x86_64.rpm']

    def upload(self, names, member_type=tarfile.REGTYPE, command='publish stable'):
        stream = io.BytesIO()
        with tarfile.open(fileobj=stream, mode='w') as archive:
            for name in names:
                entry = tarfile.TarInfo(name)
                entry.type = member_type
                entry.size = 3
                archive.addfile(entry, io.BytesIO(b'rpm'))
        stream.seek(0)
        with tempfile.TemporaryDirectory() as directory:
            with patch.dict(receive['main'].__globals__, BASE=Path(directory)), \
                    patch('os.environ', {'SSH_ORIGINAL_COMMAND': command}), \
                    patch('sys.stdin', types.SimpleNamespace(buffer=stream)), \
                    patch('subprocess.run') as run:
                try:
                    receive['main']()
                finally:
                    self.assertEqual(list(Path(directory).iterdir()), [])
                run.assert_called_once()
                self.assertEqual(run.call_args.args[0][:4],
                                 ['/usr/bin/sudo', '-n', '/usr/local/sbin/xfmd-promote', 'stable'])

    def test_single_xfmd_batch(self):
        self.upload(self.valid)

    def test_traversal_links_duplicates_and_counts(self):
        for names in ([], self.valid * 2, ['../' + self.valid[0]],
                      ['sub/' + self.valid[0]], ['minibrowser-1.0.0-1.el10_2.x86_64.rpm']):
            with self.assertRaises(ValueError): self.upload(names)
        for kind in (tarfile.SYMTYPE, tarfile.LNKTYPE, tarfile.DIRTYPE):
            with self.assertRaises(ValueError): self.upload(self.valid, member_type=kind)

    def test_invalid_command(self):
        for command in ('publish stable; id', 'publish other', 'bash', 'publish stable\n'):
            with self.assertRaises(ValueError): self.upload(self.valid, command=command)

    def test_package_identity(self):
        good = ('xfmd-0.9.0-20.el10_2.x86_64.rpm', 'xfmd', '0', '0.9.0', '20.el10_2', 'x86_64')
        validate(*good)
        for index, bad in enumerate(('other.rpm', 'other', '1', '0.9.0^dirty', '0.el10_2', 'aarch64')):
            values = list(good); values[index] = bad
            with self.assertRaises(ValueError): validate(*values)


if __name__ == '__main__':
    unittest.main()
