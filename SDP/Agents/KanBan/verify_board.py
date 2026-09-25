#!/usr/bin/env python3
"""Validate the local SDP KanBan board. Requires Python 3 and jsonschema.

No network or Git checkout of SDP is required; this does not verify product code.
"""
import json
import re
import runpy
from pathlib import Path

BOARD = Path(__file__).resolve().parent
STATES = {
    'backlog': {'backlog', 'queued'},
    'active': {'ready', 'in-progress', 'gate-review'},
    **{name: {name} for name in ('onHold', 'completed', 'canceled', 'superseded', 'irrelevant')},
}


def main():
    descriptor = json.loads((BOARD / 'board.json').read_text())
    assert descriptor == {'schemaVersion': '0.1', 'projectId': 'XFMD', 'ledger': 'Ledger.ndjson'}
    validate = runpy.run_path(str(BOARD / 'examples/verify_lineage.py'))['validate']
    events = [json.loads(line) for line in (BOARD / 'Ledger.ndjson').read_text().splitlines()]
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
    print(f'PASS: {len(actual)} cards, {len(events)} events; schemas, replay, CardState, placement and index')


if __name__ == '__main__':
    main()
