from pathlib import Path
import subprocess, json, shlex
r = next(p for p in Path(__file__).resolve().parents if (p / 'CMakeLists.txt').exists())
e = Path(__file__).resolve().parent
build = r / 'build-ui'
entry = next(x for x in json.loads((build/'compile_commands.json').read_text()) if x['file'].endswith('/tests/gui/SduiWorkflowGuiTest.cpp'))
a = shlex.split(entry['command'])
a[a.index('-o')+1] = '/tmp/xfmd-n3f-native-probe.o'
a[-1] = str(e/'native_probe.cpp')
subprocess.run(a, cwd=entry['directory'], check=True)
line = subprocess.check_output(['ninja','-C',str(build),'-t','commands','SduiWorkflowGuiTest'],text=True).splitlines()[-1]
a = shlex.split(line.split(' && ')[1])
a = [x for x in a if not x.startswith('-Wl,--dependency-file=')]
a[a.index('-o')+1] = '/tmp/xfmd-n3f-native-probe'
a = [('/tmp/xfmd-n3f-native-probe.o' if x.endswith('SduiWorkflowGuiTest.cpp.o') else x) for x in a]
subprocess.run(a, cwd=build, check=True)
with (e/'native.log').open('w') as out:
    subprocess.run(['python3',str(r/'tools/run_with_xvfb.py'),'/tmp/xfmd-n3f-native-probe',str(e/'../N3f-001/generated/entry.md'),str(e/'document-references')],stdout=out,stderr=subprocess.STDOUT,check=True)
subprocess.run(['pdftotext',str(e/'document-references.pdf'),str(e/'document-references.txt')],check=True)
subprocess.run(['pdftoppm','-scale-to','1400','-png','-singlefile',str(e/'document-references.pdf'),str(e/'document-references-pdf')],check=True)
