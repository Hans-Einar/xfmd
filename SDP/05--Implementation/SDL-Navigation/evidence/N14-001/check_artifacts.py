from pathlib import Path
import subprocess, xml.etree.ElementTree as ET
root=Path(__file__).resolve().parent/'native'
for name in ['packet-narrow','packet-split']:
    pdf=root/(name+'.pdf')
    xml=subprocess.check_output(['pdftotext','-bbox',str(pdf),'-'])
    doc=ET.fromstring(xml)
    pages=doc.findall('.//{*}page'); assert len(pages)==1
    page=pages[0]; width=float(page.attrib['width']);height=float(page.attrib['height'])
    words=page.findall('.//{*}word')
    for word in words:
        a=word.attrib
        assert 0<=float(a['xMin'])<=float(a['xMax'])<=width
        assert 0<=float(a['yMin'])<=float(a['yMax'])<=height
    text=' '.join(w.text or '' for w in words)
    if name=='packet-narrow':
        for label in ['PacketFlag','PacketNumber','PacketPayload']: assert label in text
        twelve=next(w for w in words if w.text=='12'); thirteen=next(w for w in words if w.text=='13')
        assert abs(float(twelve.attrib['yMin'])-float(thirteen.attrib['yMin']))<0.1
        gap=float(thirteen.attrib['xMin'])-float(twelve.attrib['xMax']);assert gap>2,gap
        print('PASS: adjacent 12/13 PDF glyph boxes separated by',round(gap,2),'points')
    else:
        for label in ['FirstLongFlag','SecondLongFlag','CrossingLongName','LastFlag']: assert label in text
        assert text.count('CrossingLongName')==2,text
    images=subprocess.check_output(['pdfimages','-list',str(pdf)],text=True).splitlines();assert len(images)==2
    print('PASS:',name,'one A4 vector page, all labels within page. Visual inspection remains required.')
