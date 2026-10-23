from pathlib import Path
import subprocess,shlex
r=Path('/home/warloc/git/xfmd-sdl-navigation');b=r/'build-ui';e=r/'SDP/05--Implementation/SDL-Navigation/evidence/N3a-001'
for target,obj,binary in [('SduiWorkflowGuiTest','/tmp/xfmd-native-probe.o','/tmp/xfmd-native-probe-after'),('DiagramLayoutTest','/tmp/xfmd-diagram-probe.o','/tmp/xfmd-diagram-probe-after')]:
 line=subprocess.check_output(['ninja','-C',str(b),'-t','commands',target],text=True).splitlines()[-1];a=shlex.split(line.split(' && ')[1]);a=[x for x in a if not x.startswith('-Wl,--dependency-file=')];a[a.index('-o')+1]=binary;a=[obj if x.endswith(target+'.cpp.o') else x for x in a];subprocess.run(a,cwd=b,check=True)
source=r/'tests/fixtures/mermaid/mindmap-quoted.mmd'
with (e/'mindmap-quoted-after.log').open('w') as f:subprocess.run(['/tmp/xfmd-diagram-probe-after',str(source),str(e/'mindmap-quoted-after')],stdout=f,stderr=subprocess.STDOUT,check=True)
with (e/'mindmap-quoted-native-after.log').open('w') as f:subprocess.run(['python3',str(r/'tools/run_with_xvfb.py'),'/tmp/xfmd-native-probe-after',str(source),str(e/'mindmap-quoted-native-after')],stdout=f,stderr=subprocess.STDOUT,check=True)
print('After probes passed')
