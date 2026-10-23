from pathlib import Path
import subprocess,json,shlex
r=Path('/home/warloc/git/xfmd-sdl-navigation');build=r/'build-ui';e=r/'SDP/05--Implementation/SDL-Navigation/evidence/N3a-001'
entry=next(x for x in json.loads((build/'compile_commands.json').read_text()) if x['file'].endswith('/tests/gui/SduiWorkflowGuiTest.cpp'))
a=shlex.split(entry['command']);a[a.index('-o')+1]='/tmp/xfmd-native-probe.o';a[-1]=str(e/'native_probe.cpp');subprocess.run(a,cwd=entry['directory'],check=True)
line=subprocess.check_output(['ninja','-C',str(build),'-t','commands','SduiWorkflowGuiTest'],text=True).splitlines()[-1];a=shlex.split(line.split(' && ')[1]);a=[x for x in a if not x.startswith('-Wl,--dependency-file=')];a[a.index('-o')+1]='/tmp/xfmd-native-probe';a=[('/tmp/xfmd-native-probe.o' if x.endswith('SduiWorkflowGuiTest.cpp.o') else x) for x in a];subprocess.run(a,cwd=build,check=True)
for name,source in [('vp07',r/'SDP/KanBan/evidence/2026-10-06-session3/VP07-ConfigurableScrolling.mmd'),('nested',r/'tests/fixtures/mermaid/sdui-nested-containment.mmd')]:
 with (e/(name+'-native-before.log')).open('w') as f:subprocess.run(['python3',str(r/'tools/run_with_xvfb.py'),'/tmp/xfmd-native-probe',str(source),str(e/(name+'-native-before'))],stdout=f,stderr=subprocess.STDOUT,check=True)
print('Native preview/PDF captured')
