import datetime,hashlib,json,os,pathlib,shutil,subprocess,tarfile,tempfile
root=pathlib.Path('/home/warloc/git/xfmd-sdl-navigation')
prefix=pathlib.Path('/home/warloc/.local')
evidence=root/'SDP/05--Implementation/SDL-Navigation/evidence/N15b-001'
package=json.loads((evidence/'package.json').read_text())
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
archive=pathlib.Path(package['package'])
assert sha(archive)==package['package_sha256'], 'Package checksum differs from verified candidate'
stamp=datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%SZ')
record=prefix/'state/xfmd/installations'/stamp
record.mkdir(parents=True)
shutil.copy2(__file__,record/'install.py')
previous_version=subprocess.check_output([str(prefix/'bin/xfmd'),'--version'],text=True).strip()
with tempfile.TemporaryDirectory(prefix='xfmd-09-install-') as tmp:
    stage=pathlib.Path(tmp)
    with tarfile.open(archive) as t:t.extractall(stage,filter='data')
    roots=list(stage.iterdir());assert len(roots)==1
    staged=roots[0]
    assert sha(staged/'bin/xfmd')==package['binary_sha256']
    shutil.copy2(staged/'libexec/xfmd/sdptool',staged/'bin/sdptool')
    files=sorted(p for p in staged.rglob('*') if p.is_file())
    assert all(not p.is_symlink() for p in files)
    files.sort(key=lambda p:p.relative_to(staged)==pathlib.Path('bin/xfmd'))
    installed=[]
    def replace(source,target):
        fd,temp=tempfile.mkstemp(prefix='.'+target.name+'-',dir=target.parent);os.close(fd)
        try:shutil.copy2(source,temp);os.replace(temp,target)
        finally:
            if os.path.exists(temp):os.unlink(temp)
    try:
        for source in files:
            rel=source.relative_to(staged);target=prefix/rel;backup=record/'previous'/rel
            existed=target.exists()
            if existed:
                backup.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(target,backup)
            target.parent.mkdir(parents=True,exist_ok=True)
            replace(source,target);installed.append((rel,existed))
        assert all(sha(prefix/p.relative_to(staged))==sha(p) for p in files)
        subprocess.run(['python3',str(root/'tools/run_with_xvfb.py'),'python3',str(root/'SDP/05--Implementation/Standalone-Mermaid/evidence/MM-001/package_smoke.py'),str(prefix/'bin/xfmd'),str(root/'tests/fixtures/mermaid/standalone.mmd'),str(record/'smoke.log')],check=True)
    except Exception:
        for rel,existed in reversed(installed):
            target=prefix/rel
            if existed:replace(record/'previous'/rel,target)
            else:target.unlink()
        raise
    receipt={'installedAt':datetime.datetime.now(datetime.timezone.utc).isoformat(),'package':package,'prefix':str(prefix),'previous_version':previous_version,'installed_version':subprocess.check_output([str(prefix/'bin/xfmd'),'--version'],text=True).strip(),'files_sha256':{str(p.relative_to(staged)):sha(prefix/p.relative_to(staged)) for p in files},'backup':str(record/'previous'),'authority':'Owner SESSION-XFMD-0007 T012: autonomous completion through local XFMD and required SDPTool installation; selected XFMD 0.9','verification':{'package_checksum':True,'all_installed_files_match_package':True,'native_smoke':'passed isolated Xvfb/temp-HOME startup with raw Mermaid fixture'},'standaloneProducer':subprocess.check_output([str(prefix/'bin/sdptool'),'--version'],text=True).strip(),'publication':None}
    (record/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n')
pathlib.Path('/tmp/xfmd-n15b-install-record').write_text(str(record))
print('Installed:',receipt['installed_version'])
print('Files:',len(files))
print('Receipt:',record/'receipt.json')
