import importlib.util
import json
from pathlib import Path
import re
import subprocess
import tempfile
import tomllib

repo = Path.cwd()
spec = importlib.util.spec_from_file_location('validator', repo/'tools/validate_blueprints.py')
v = importlib.util.module_from_spec(spec)
spec.loader.exec_module(v)
with tempfile.TemporaryDirectory(prefix='xfmd-kb002-links-') as d:
    root = Path(d)
    def write(name, text):
        p = root/name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)
        return p
    write('xfmd_requirements.md', '| UR-001 | Fixture | AT-001 |\n')
    write('src/blueprint/README.md', '| UR-001 | [FUNC-001](functionality/Functionality-001--Fixture.md) | AT-001 | FirstRelease |\n')
    write('src/application/Fixture.cpp', 'void Fixture::open() {}\n')
    meta = '---\nid: FUNC-001\nkind: Functionality\naudience: System\nrole: Service\nowner: application\nstatus: Implemented\nscope: FirstRelease\nrequirements: UR-001\nuses: none\n---\n'
    body = '\n\n'.join(h+'\n\n'+('| 1 | Caller | Fixture::open | src/application/Fixture.cpp | value | error | Implemented |' if h=='## 5. Plumbing' else 'UR-001; AT-001 fixture.') for h in v.HEADINGS)
    blueprint = write('src/blueprint/functionality/Functionality-001--Fixture.md', meta+body+'\n')
    def expect(label, count, contains=None):
        errors, objects, reqs = v.check(root)
        assert len(errors)==count and objects==1 and reqs==1, (label, errors)
        if contains: assert any(contains in e for e in errors), (label, errors)
        print('PASS:', label)
    expect('valid project fixture', 0)
    for folder in ['.deps/vendor', 'third_party/vendor', 'src/third_party/vendor']:
        write(folder+'/README.md', '[broken](missing.md)\n')
    expect('dependency-owned broken links excluded', 0)
    for name in ['docs/probe.md', 'Agents/KanBan/backlog/#099--Study--Fixture.md']:
        p=write(name, '[broken](missing.md)\n')
        expect('project link rejected: '+name, 1, 'broken/outside local link')
        p.unlink()
    p=write('README.md','[missing dependency](.deps/vendor/missing.md)\n')
    expect('project link into missing dependency still rejected',1,'broken/outside local link')
    p.unlink()
    blueprint.write_text((meta+body).replace('owner: application','owner: invalid'))
    expect('metadata check retained',1,'invalid owner')
    blueprint.write_text(meta+body.replace(v.HEADINGS[-1],'## 8. Wrong'))
    expect('chapter check retained',1,'expected the eight fixed headings')
    blueprint.write_text(meta+body.replace('Fixture.cpp','Missing.cpp'))
    expect('plumbing source check retained',1,'implemented source does not exist')
    blueprint.write_text(meta+body)
    write('src/blueprint/README.md','| UR-001 | [FUNC-001](functionality/Functionality-001--Fixture.md) | FirstRelease |\n')
    expect('acceptance matrix check retained',1,'matrix omits acceptance ID')

workflow=(repo/'.github/workflows/build.yml').read_text()
command=re.search(r'run: (cargo fmt -p .+)',workflow).group(1).split()
packages=[command[i+1] for i,x in enumerate(command) if x=='-p']
workspace=tomllib.loads((repo/'Cargo.toml').read_text())['workspace']['members']
actual=[tomllib.loads((repo/p/'Cargo.toml').read_text())['package']['name'] for p in workspace]
assert set(packages)==set(actual) and len(packages)==4
print('PASS: CI package selection equals all four workspace packages')
with tempfile.TemporaryDirectory(prefix='xfmd-kb002-fmt-') as d:
    root=Path(d)
    (root/'Cargo.toml').write_text('[workspace]\nresolver="2"\nmembers='+json.dumps(packages)+'\nexclude=[".deps/vendor"]\n')
    for name in [*packages,'.deps/vendor']:
        path=root/name
        (path/'src').mkdir(parents=True)
        dep='\n[dependencies]\nvendor={path="../.deps/vendor"}\n' if name==packages[0] else ''
        (path/'Cargo.toml').write_text('[package]\nname='+json.dumps(Path(name).name)+'\nversion="0.1.0"\nedition="2024"\n'+dep)
        (path/'src/lib.rs').write_text('pub fn value() -> u8 {\n    1\n}\n')
    (root/'.deps/vendor/src/lib.rs').write_text('pub fn value()->u8{1}\n')
    def fmt(args, expected, label):
        r=subprocess.run(['cargo','fmt',*args,'--','--check'],cwd=root,capture_output=True,text=True)
        assert r.returncode==expected,(label,r.stdout,r.stderr)
        print('PASS:',label)
    fmt(['--all'],1,'old --all rejects dependency-only format defect')
    args=command[2:command.index('--')]
    fmt(args,0,'selected packages ignore dependency-only format defect')
    for name in packages:
        p=root/name/'src/lib.rs'
        good=p.read_text(); p.write_text('pub fn value()->u8{1}\n')
        fmt(args,1,'selected packages reject XFMD format defect: '+name)
        p.write_text(good)
print('All 16 boundary probes passed.')
