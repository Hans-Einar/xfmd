from pathlib import Path
import hashlib,json,subprocess,tarfile,tempfile
root=Path('/home/warloc/git/xfmd-sdl-navigation');ev=root/'SDP/05--Implementation/SDL-Navigation/evidence/N15b-001'
archive=root/'build-ui/release-0.9/xfmd-0.9.0-Linux.tar.gz'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
with tempfile.TemporaryDirectory(prefix='xfmd09-package-') as tmp:
 stage=Path(tmp)
 with tarfile.open(archive) as t:t.extractall(stage,filter='data')
 roots=list(stage.iterdir());assert len(roots)==1
 p=roots[0];files=sorted(f for f in p.rglob('*') if f.is_file());assert not any(f.is_symlink() for f in files)
 assert sha(p/'bin/xfmd')==sha(root/'build-ui/xfmd')
 for name in ['sdptool','sdui-preview','sdui-fyne']:
  assert sha(p/'libexec/xfmd'/name)==sha(root/'build-ui/libexec/xfmd'/name)
 version=subprocess.check_output([str(p/'bin/xfmd'),'--version'],text=True).strip();assert version.startswith('xfmd 0.9 ')
 producer=subprocess.check_output([str(p/'libexec/xfmd/sdptool'),'--version'],text=True);assert 'e194721' in producer
 record={'package':str(archive),'package_sha256':sha(archive),'binary_sha256':sha(p/'bin/xfmd'),'version':version,'producerVersion':producer,'head':subprocess.check_output(['git','rev-parse','HEAD'],cwd=root,text=True).strip(),'dirty':True,'file_count':len(files),'files_sha256':{str(f.relative_to(p)):sha(f) for f in files}}
 (ev/'package.json').write_text(json.dumps(record,indent=2)+'\n')
 print(version,len(files),'files, binary/helper hashes match')
