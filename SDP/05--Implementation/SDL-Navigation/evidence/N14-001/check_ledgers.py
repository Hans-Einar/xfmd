from pathlib import Path
import json,hashlib
from jsonschema import Draft202012Validator,FormatChecker
r=next(p for p in Path(__file__).resolve().parents if (p/'CMakeLists.txt').exists());e=Path(__file__).resolve().parent
schema=Path('/home/warloc/git/SDP-xfmd-viewpoint-main/Toolkit/schemas/ledger-event.schema.json')
validator=Draft202012Validator(json.loads(schema.read_text()),format_checker=FormatChecker())
before=json.loads((e/'ledger-before.json').read_text());events={}
for name,oldhash in before.items():
 p=r/name;lines=p.read_bytes().splitlines(keepends=True)
 assert hashlib.sha256(b''.join(lines[:-1])).hexdigest()==oldhash,(name,'historical bytes changed')
 for line in lines:
  if not line.strip():continue
  event=json.loads(line);validator.validate(event)
  assert event['eventId'] not in events;events[event['eventId']]=event
for event in events.values():
 payload=event['payload'];prev=payload.get('previousEventId')
 if prev:
  assert prev in events,(event['eventId'],prev)
  old=events[prev];assert old.get('subjectId')==event.get('subjectId')
  if 'from' in payload and 'to' in old['payload']:assert payload['from']==old['payload']['to']
 for eid in payload.get('managementEvents',[]):assert eid in events
print('PASS:',len(events),'events: generic schema, unique IDs, previous-event subject/state lineage and management evidence references')
print('PASS: pre-existing ledger bytes unchanged; exactly one event appended to each ledger')
