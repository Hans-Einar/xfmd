from pathlib import Path
import subprocess, json, shlex
r = next(p for p in Path(__file__).resolve().parents if (p / 'CMakeLists.txt').exists())
e = Path(__file__).resolve().parent
build = r / 'build-ui'
entry = next(x for x in json.loads((build/'compile_commands.json').read_text()) if x['file'].endswith('/tests/gui/SdpOverviewGuiTest.cpp'))
a = shlex.split(entry['command']);a[a.index('-o')+1] = '/tmp/xfmd-n14-native-probe.o';a[-1] = str(e/'native_probe.cpp')
subprocess.run(a,cwd=entry['directory'],check=True)
line = subprocess.check_output(['ninja','-C',str(build),'-t','commands','SdpOverviewGuiTest'],text=True).splitlines()[-1]
a=shlex.split(line.split(' && ')[1]);a=[x for x in a if not x.startswith('-Wl,--dependency-file=')];a[a.index('-o')+1]='/tmp/xfmd-n14-native-probe'
a=[('/tmp/xfmd-n14-native-probe.o' if x.endswith('SdpOverviewGuiTest.cpp.o') else x) for x in a]
subprocess.run(a,cwd=build,check=True)
for name in ['packet-narrow','packet-split']:
    base=e/'native'/name
    with base.with_suffix('.log').open('w') as out:
        subprocess.run(['python3',str(r/'tools/run_with_xvfb.py'),'/tmp/xfmd-n14-native-probe',str(base.with_suffix('.md')),str(base)],stdout=out,stderr=subprocess.STDOUT,check=True)
    subprocess.run(['pdftotext',str(base.with_suffix('.pdf')),str(base.with_suffix('.txt'))],check=True)
    subprocess.run(['pdftoppm','-scale-to','1400','-png','-singlefile',str(base.with_suffix('.pdf')),str(base)+'-pdf'],check=True)
