from pathlib import Path
import json,re,subprocess,xml.etree.ElementTree as E
p=Path(__file__).resolve().parent
for name in ['vp09-actual','vp09-native']:
 subprocess.run(['pdftotext','-layout',str(p/'native'/f'{name}.pdf'),str(p/'native'/f'{name}.txt')],check=True)
subprocess.run(['pdftotext','-bbox',str(p/'native/vp09-actual.pdf'),str(p/'native/vp09-actual-bbox.html')],check=True)
m=json.loads((p/'actual-view/manifest.json').read_text());s=(p/'actual-view/source-inputs/s0.design').read_text();txt=(p/'actual-view/entry.md').read_text();pdf=re.sub(r'\s','',(p/'native/vp09-actual.txt').read_text());d=m['diagrams'];nodes={v['model_id'] for x in d for v in x['nodes'].values()};assert all(n in pdf for n in nodes)
ns={'x':'http://www.w3.org/1999/xhtml'};root=E.parse(p/'native/vp09-actual-bbox.html').getroot();col=''
for page in root.findall('.//x:page',ns):
 words=sorted(page.findall('.//x:word',ns),key=lambda w:(float(w.attrib['yMin']),float(w.attrib['xMin'])))
 col+=''.join(w.text or '' for w in words if abs(float(w.attrib['xMin'])-61.6914)<0.05)
counts={}
for kind in ['dataset','datagram','database','contract','variant','field']:
 names=re.findall(r'^'+kind+r' ([A-Za-z0-9]+)\.',s,re.M);counts[kind]=len(names)
 missing=[n for n in names if n not in pdf and n not in col];assert not missing,(kind,missing)
spans=set(re.findall(r'sdl-source://(s\d+)/(\d+)/(\d+)',txt))
for sid,a,b in spans:
 data=(p/'actual-view/source-inputs'/f'{sid}.design').read_bytes();assert 0<=int(a)<int(b)<=len(data)
counts.update({'diagrams':len(d),'nodes':len(nodes),'edges':sum(len(x['edges']) for x in d),'sourceSpansInBounds':len(spans),'allInventoryNamesInPdf':True,'pdfMethod':'layout text plus bbox first-column reconstruction of wrapped contract/field identifiers'})
fixture=' '.join((p/'native/vp09-native.txt').read_text().split());assert 'Database-backed persistence' in fixture and 'Unit-held; persistence unspecified' in fixture
(p/'artifact-checks.json').write_text(json.dumps(counts,indent=2)+'\n');print(counts)
