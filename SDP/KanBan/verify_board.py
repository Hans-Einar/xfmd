#!/usr/bin/env python3
"""Validate the local SDP KanBan board. Requires Python 3 and jsonschema.

No network or Git checkout of SDP is required; this does not verify product code.
"""
import json
import re
import runpy
from jsonschema import Draft202012Validator
from pathlib import Path
from datetime import datetime

BOARD = Path(__file__).resolve().parent
STATES = {
    'backlog': {'backlog', 'queued'},
    'active': {'ready', 'in-progress', 'gate-review'},
    **{name: {name} for name in ('onHold', 'completed', 'canceled', 'superseded', 'irrelevant')},
}


def main():
    descriptor = json.loads((BOARD / 'board.json').read_text())
    legacy = {'schemaVersion': '0.1', 'projectId': 'XFMD', 'ledger': 'Ledger.ndjson'}
    shared = {'schemaVersion': '0.2', 'projectId': 'XFMD',
              'ledger': '../ProjectManagement/Ledger.ndjson',
              'namespaces': ['XFMD'], 'profile': 'sdp-project-management/0.2'}
    assert descriptor in (legacy, shared), 'Unsupported board descriptor'
    contract = runpy.run_path(str(BOARD / 'examples/verify_lineage.py'))
    validate = contract['validate']
    all_events = [json.loads(line) for line in
                  (BOARD / descriptor['ledger']).read_text().splitlines()]
    assert len({e['eventId'] for e in all_events}) == len(all_events), 'Duplicate event ID'
    management = {}
    for event in all_events:
        contract['ENVELOPE'].validate(event)
        if event['eventType'].startswith('x-kanban:'):
            continue
        assert descriptor == shared and event['eventType'].startswith('x-management:'), 'Unknown event'
        payload = event['payload']
        filename = {'0.1': 'management-payload.schema.json',
                    '0.2': 'management-payload-0.2.schema.json'}[payload['schemaVersion']]
        schema = json.loads((BOARD.parent / 'ProjectManagement' / filename).read_text())
        Draft202012Validator(schema).validate(payload)
        previous = management.get(event['subjectId'])
        if previous is None:
            assert event['eventType'] == 'x-management:created'
            assert payload['previousEventId'] is payload['from'] is payload['fromPath'] is None
            assert payload['to'] in {'planned', 'active'}
        else:
            old = previous['payload']
            assert datetime.fromisoformat(event['occurredAt']) >= datetime.fromisoformat(previous['occurredAt'])
            assert payload['previousEventId'] == previous['eventId']
            assert (payload['from'], payload['fromPath']) == (old['to'], old['toPath'])
            assert payload['kind'] == old['kind']
            transitions = {'x-management:started': ('planned', 'active'),
                           'x-management:completed': ('active', 'completed')}
            if event['eventType'] in transitions:
                assert (payload['from'], payload['to']) == transitions[event['eventType']]
            elif event['eventType'] == 'x-management:updated':
                assert payload['from'] == payload['to']
            else:
                assert event['eventType'] == 'x-management:canceled'
                assert payload['from'] in {'planned', 'active'} and payload['to'] == 'canceled'
        management[event['subjectId']] = event
    for ident, event in management.items():
        target = (BOARD.parent / event['payload']['toPath']).resolve()
        assert target.is_relative_to(BOARD.parent.resolve()), 'Outside process area'
        text = target.read_text()
        assert f'| id | {ident} |' in text and f"| state | {event['payload']['to']} |" in text
    events = [e for e in all_events if e['eventType'].startswith('x-kanban:')]
    latest, _ = validate(events)
    numbers = [int(event['eventId'].rsplit('-', 1)[1]) for event in events]
    assert numbers == sorted(set(numbers)), 'Event numbers are not unique/increasing'
    actual = {}
    for folder, allowed in STATES.items():
        for path in (BOARD / folder).glob('#*.md'):
            text = path.read_text()
            ids = re.findall(r'^\| id \| (KB-XFMD-\d+) \|$', text, re.M)
            states = re.findall(r'^\| CardState \| ([^|]+) \|$', text, re.M)
            assert len(ids) == len(states) == 1, path
            assert states[0] in allowed, (path, states)
            ident = ids[0]
            assert ident not in actual, ident
            actual[ident] = str(path.relative_to(BOARD))
    assert actual == {ident: event['payload']['toPath'] for ident, event in latest.items()}
    index = dict(re.findall(r'^\| (KB-XFMD-\d+) \| [^|]+ \| (\w+) \|', (BOARD / 'README.md').read_text(), re.M))
    assert index == {ident: path.split('/')[0] for ident, path in actual.items()}, 'Index mismatch'
    print(f'PASS: {len(actual)} cards, {len(events)} card events, {len(management)} management records; schemas, replay, CardState, placement and index')


if __name__ == '__main__':
    main()
