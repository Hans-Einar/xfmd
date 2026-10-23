from pathlib import Path
import json,shlex,subprocess
r=Path('/home/warloc/git/xfmd-sdl-navigation');e=r/'SDP/05--Implementation/SDL-Navigation/evidence/N3a-001';build=r/'build-ui'
entry=next(x for x in json.loads((build/'compile_commands.json').read_text()) if x['file'].endswith('/tests/renderer/DiagramLayoutTest.cpp'))
a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/xfmd-diagram-probe.o';a[-1]=str(e/'probe.cpp');subprocess.run(a,cwd=entry['directory'],check=True)
line=subprocess.check_output(['ninja','-C',str(build),'-t','commands','DiagramLayoutTest'],text=True).splitlines()[-1]
a=shlex.split(line.split(' && ')[1]);a=[x for x in a if not x.startswith('-Wl,--dependency-file=')];a[a.index('-o')+1]='/tmp/xfmd-diagram-probe';a=[('/tmp/xfmd-diagram-probe.o' if x.endswith('DiagramLayoutTest.cpp.o') else x) for x in a];subprocess.run(a,cwd=build,check=True)
for name,source in [('vp07',r/'SDP/KanBan/evidence/2026-10-06-session3/VP07-ConfigurableScrolling.mmd'),('nested',r/'tests/fixtures/mermaid/sdui-nested-containment.mmd')]:
 with (e/(name+'-before.log')).open('w') as f:
  result=subprocess.run(['/tmp/xfmd-diagram-probe',str(source),str(e/name)],stdout=f,stderr=subprocess.STDOUT)
 print(name,'exit',result.returncode,(e/(name+'-before.log')).read_text())
