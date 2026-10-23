#!/usr/bin/env python3
"""Deterministic demand/lifetime fixture; real producer is covered separately."""
import json, os, pathlib, sys
root = pathlib.Path(sys.argv[1])
op = sys.argv[2]
log = pathlib.Path(os.environ['XFMD_RUNTIME_CALLS'])
with log.open('a') as stream:
    stream.write(op + '\n')
reply = {'schema': 'sdptool/0.2', 'operation': op}
if op == 'actions':
    reply = {'schema': 'sdptool.actions/1', 'operation': 'actions', 'actions': []}
elif op == 'discover':
    def target(uri):
        return dict(operation='select', project='runtime-test', model='model', revision='r1', uri=uri)
    children = ['diagram-' + str(i) for i in range(35)]
    nodes = [dict(id='sdl', kind='tab', label='SDL', state='available', children=['vp']),
             dict(id='vp', kind='viewpoint', label='VP01', children=children,
                  target=target('sdl-view://runtime-test/VP01?target=main'))]
    nodes += [dict(id=d, kind='diagram', label=d,
                   target=target('sdl-view://runtime-test/VP01?diagram=' + d + '&target=main')) for d in children]
    reply.update(status='valid', root=str(root), area=str(root/'SDP'),
                 navigation=dict(schema='sdptool/0.2', operation='tree', project='runtime-test', roots=['sdl'], nodes=nodes))
elif op == 'select':
    uri = sys.argv[sys.argv.index('--uri') + 1]
    if 'diagram=' not in uri:
        print(json.dumps(dict(schema='sdptool/0.2', error=dict(code='limit',message='Unexpected bulk selection'))), file=sys.stderr)
        sys.exit(1)
    output = pathlib.Path(sys.argv[sys.argv.index('--output') + 1])
    output.mkdir(exist_ok=True)
    entry = output/'entry.md'
    entry.write_text('# Selected diagram\n\n' + uri + '\n')
    reply.update(entry=str(entry), directory=str(output), revision='r1')
else:
    sys.exit(2)
print(json.dumps(reply))
