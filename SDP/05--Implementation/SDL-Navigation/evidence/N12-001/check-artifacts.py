from pathlib import Path
import json,re,subprocess
p=Path(__file__).resolve().parent
for name in ['vp10-actual','vp10-native']:
 subprocess.run(['pdftotext','-layout',str(p/'native'/f'{name}.pdf'),str(p/'native'/f'{name}.txt')],check=True)
actual=json.loads((p/'actual-view/manifest.json').read_text())
assert len(actual['diagrams'])==0
source=(p/'actual-view/source-inputs/s0.design').read_text()
assert not re.search(r'^(encoding|variant) ',source,re.M)
for filename in [p/'actual-view/entry.md',p/'native/vp10-actual.txt']:
 text=filename.read_text();assert 'No Encodings are declared' in text and 'No Variants are declared' in text
fixture=(p/'native/vp10-generated.md').read_text();data=(p/'native/vp10-source.design.txt').read_bytes()
assert fixture.count('```mermaid\npacket')==2
for name in ['PacketBig','PacketLittle','PacketFlag','PacketNumber','PacketPayload','PacketBody','WireVariant']:
 assert name in fixture and name in (p/'native/vp10-native.txt').read_text(),name
spans=set(re.findall(r'sdl-source://[^/]+/(\d+)/(\d+)',fixture))
for a,b in spans:assert 0<=int(a)<int(b)<=len(data)
for field,offset,width in [('PacketFlag',0,1),('PacketNumber',1,12),('PacketPayload',13,16)]:
 for enc in ['PacketBig','PacketLittle']:
  expected=f'{enc} places {field} at {offset} bits {width}.'
  assert any(data[int(a):int(b)].decode()==expected for a,b in spans),expected
body=re.findall(r'```mermaid\n(packet.*?)```',fixture,re.S)
detail=(p/'native/vp10-detail.md').read_text();single=re.findall(r'```mermaid\n(packet.*?)```',detail,re.S)
assert len(single)==1 and single[0]==body[0]
result={'actualEncodings':0,'actualVariants':0,'actualDiagrams':0,'fixturePackets':2,'fixtureBitsPerPacket':29,'fixtureFieldsPerPacket':3,'fixtureSourceSpansInBounds':len(spans),'sixPlacementSpansExact':True,'legacyDetailPacketUnchanged':True,'pdfTextContainsAllFieldNames':True,'visualLimitation':'KB020: narrow 1-bit PacketFlag label clipped and adjacent 12/13 boundary labels crowded in native/PDF. Full source-linked register remains readable. PDF text extraction is not proof of visible diagram-label completeness.','generalVisualAcceptance':False}
(p/'artifact-checks.json').write_text(json.dumps(result,indent=2)+'\n');print(result)
