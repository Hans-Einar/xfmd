from pathlib import Path
import json,re,subprocess
p=Path(__file__).resolve().parent
result={}
for name in ['vp11-actual','vp11-native']:
 subprocess.run(['pdftotext','-layout',str(p/'native'/f'{name}.pdf'),str(p/'native'/f'{name}.txt')],check=True)
 pdf=(p/'native'/f'{name}.txt').read_text();compact=re.sub(r'\s','',pdf)
 if name=='vp11-actual':
  text=(p/'actual-view/entry.md').read_text();source=(p/'actual-view/source-inputs/s0.design').read_bytes();m=json.loads((p/'actual-view/manifest.json').read_text());facts=[f['text'] for f in m['facts']];gaps=m['model_gaps'];assert len(m['diagrams'])==0
 else:
  text=(p/'native/vp11-generated.md').read_text();source=(p/'native/vp11-source.design.txt').read_bytes();facts=[l for l in source.decode().splitlines() if l[:1].isupper()];gaps=None
 declarations=re.findall(r'^([a-z]+) ([A-Z][A-Za-z0-9]*)\.$',source.decode(),re.M)
 missing=[f for f in facts if re.sub(r'\s','',f) not in compact];assert not missing,(name,missing[:10])
 assert all(n in compact for kind,n in declarations)
 spans=set(re.findall(r'sdl-source://s0/(\d+)/(\d+)',text));covered=set()
 for a,b in spans:
  assert 0<=int(a)<int(b)<=len(source);covered.add(source[int(a):int(b)].decode())
 assert all(f in covered for f in facts)
 assert all(f'{kind} {n}.' in covered for kind,n in declarations)
 assert len(spans)==len(facts)+len(declarations)
 assert f'| Declarations | {len(declarations)} |' in text and f'| Facts | {len(facts)} |' in text
 assert 'No diagrams match' not in text and '```mermaid' not in text
 if gaps is not None:
  for g in gaps:assert g['code'].replace('_','') in compact.replace('_','') and re.sub(r'\s','',g['message']) in compact
 result[name]={'declarations':len(declarations),'facts':len(facts),'explicitProperties':sum(' has ' in f for f in facts),'exactSourceSpans':len(spans),'allFactStatementsAndDeclarationNamesInPdf':True,'pdfPages':pdf.count('\f'),'reportedGaps':len(gaps) if gaps is not None else None}
result['limits']='Counts describe checked sources only; reported gap rules are not exhaustive. Same-context verification; no independent or general large-print acceptance. Existing KB020/025 residuals unchanged.'
(p/'artifact-checks.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
