from pathlib import Path
import subprocess,shlex
r=Path('/home/warloc/git/xfmd-sdl-navigation');b=r/'build-ui';e=r/'SDP/05--Implementation/SDL-Navigation/evidence/N3a-001';archive='/tmp/xfmd-diagram-fork-target/debug/libxfmd_mermaid_ffi.a'
for target,obj,binary in [('SduiWorkflowGuiTest','/tmp/xfmd-native-probe.o','/tmp/xfmd-vp07-native-candidate'),('DiagramLayoutTest','/tmp/xfmd-diagram-exact.o','/tmp/xfmd-vp07-geometry-candidate')]:
 line=subprocess.check_output(['ninja','-C',str(b),'-t','commands',target],text=True).splitlines()[-1];a=shlex.split(line.split(' && ')[1]);a=[x for x in a if not x.startswith('-Wl,--dependency-file=')];a[a.index('-o')+1]=binary;a=[obj if x.endswith(target+'.cpp.o') else archive if x.endswith('/libxfmd_mermaid_ffi.a') else x for x in a];subprocess.run(a,cwd=b,check=True)
source=r/'SDP/KanBan/evidence/2026-10-06-session3/VP07-ConfigurableScrolling.mmd'
with (e/'vp07-candidate-after.log').open('w') as f:subprocess.run(['/tmp/xfmd-vp07-geometry-candidate',str(source),str(e/'vp07-candidate-after')],stdout=f,stderr=subprocess.STDOUT,check=True)
with (e/'vp07-native-candidate-after.log').open('w') as f:subprocess.run(['python3',str(r/'tools/run_with_xvfb.py'),'/tmp/xfmd-vp07-native-candidate',str(source),str(e/'vp07-native-candidate-after')],stdout=f,stderr=subprocess.STDOUT,check=True)
print('Candidate native/PDF completed')
