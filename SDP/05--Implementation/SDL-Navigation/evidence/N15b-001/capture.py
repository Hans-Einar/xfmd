from pathlib import Path
import hashlib,json,subprocess,tarfile
root=Path('/home/warloc/git/xfmd-sdl-navigation');ev=root/'SDP/05--Implementation/SDL-Navigation/evidence/N15b-001'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
old=json.loads((root/'SDP/05--Implementation/SDL-Navigation/evidence/N14-001/candidate.json').read_text())
paths={root/p for p in old['files'] if (root/p).is_file()}
for name in ['src','cmake','tests','tools','packaging','LICENSES']:
 paths.update(p for p in (root/name).rglob('*') if p.is_file() and not p.is_symlink() and '__pycache__' not in p.parts and p.suffix!='.pyc')
for name in ['VERSION','CMakeLists.txt','Cargo.toml','Cargo.lock','README.md','softwareArchitecture.md','xfmd_requirements.md','docs/versioning.md']:
 paths.add(root/name)
paths.add(root/old['renderer']['archive'])
with tarfile.open(ev/'source.tar.gz','w:gz') as t:
 for p in sorted(paths):t.add(p,arcname=str(p.relative_to(root)),recursive=False)
producer=Path('/home/warloc/git/SDP-xfmd-viewpoint-main')
assert subprocess.check_output(['git','status','--porcelain'],cwd=producer,text=True)==''
subprocess.run(['git','archive','--format=tar.gz','-o',str(ev/'producer-source.tar.gz'),'HEAD','SDPTool','SDL/go','SDUI/go','SDL/docs','SDUI/docs'],cwd=producer,check=True)
bins=[root/'build-ui'/n for n in ['xfmd','SdpDeferredGuiTest','SdpNavigationGuiTest','SdpOverviewGuiTest','SdlSourceGuiTest']]+list((root/'build-ui/libexec/xfmd').iterdir())
r={'work':'N15b / Session0007 T012 / XFMD 0.9 local installation','consumerHead':subprocess.check_output(['git','rev-parse','HEAD'],cwd=root,text=True).strip(),'consumerState':'Shared dirty tree, exact archive/hashes; not an unchanged HEAD build. Unrelated staged/dirty work preserved.','producerCommit':subprocess.check_output(['git','rev-parse','HEAD'],cwd=producer,text=True).strip(),'renderer':old['renderer'],'files':{str(p.relative_to(root)):sha(p) for p in sorted(paths)},'sourceArchiveSha256':sha(ev/'source.tar.gz'),'producerSourceArchiveSha256':sha(ev/'producer-source.tar.gz'),'binaries':{str(p):sha(p) for p in bins if p.is_file()},'limitations':['Local development installation, not public release/main merge','No independent review/owner visual acceptance claimed','Remaining KB020/025/028 guards and acceptance stay open','No authored SDL/SDUI changes or installed process upgrade']}
(ev/'candidate.json').write_text(json.dumps(r,indent=2)+'\n');print('Captured',len(paths),'inputs and',len(r['binaries']),'binaries')
