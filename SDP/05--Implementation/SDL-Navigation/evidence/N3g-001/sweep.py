from pathlib import Path
import json,shlex,subprocess
r=next(p for p in Path(__file__).resolve().parents if (p/'CMakeLists.txt').exists());e=Path(__file__).resolve().parent;build=r/'build-ui'
source=r/'SDP/05--Implementation/SDL-Navigation/evidence/N3a-001/probe.cpp'
entry=next(x for x in json.loads((build/'compile_commands.json').read_text()) if x['file'].endswith('/tests/renderer/DiagramLayoutTest.cpp'))
a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/xfmd-n3g-final-diagram-probe.o';a[-1]=str(source);subprocess.run(a,cwd=entry['directory'],check=True)
line=subprocess.check_output(['ninja','-C',str(build),'-t','commands','DiagramLayoutTest'],text=True).splitlines()[-1];a=shlex.split(line.split(' && ')[1]);a=[x for x in a if not x.startswith('-Wl,--dependency-file=')];a[a.index('-o')+1]='/tmp/xfmd-n3g-final-diagram-probe';a=[('/tmp/xfmd-n3g-final-diagram-probe.o' if x.endswith('DiagramLayoutTest.cpp.o') else x) for x in a];subprocess.run(a,cwd=build,check=True)
results=[]
for diagram in sorted((e/'vp07-sweep').glob('*/diagrams/*.mmd')):
 result=subprocess.run(['/tmp/xfmd-n3g-final-diagram-probe',str(diagram),str(diagram.parent/'native')],text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
 (diagram.parent/'native.log').write_text(result.stdout)
 results.append({'diagram':str(diagram.relative_to(e)),'exit':result.returncode,'output':result.stdout})
 print(diagram.stem,result.returncode,flush=True)
(e/'sweep-results.json').write_text(json.dumps(results,indent=2)+'\n')
raise SystemExit(int(any(x['exit'] for x in results)))
