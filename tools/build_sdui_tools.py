#!/usr/bin/env python3
"""Build SDUI-owned helpers without modifying the user's gh sdp installation."""
import argparse, pathlib, subprocess
p=argparse.ArgumentParser()
p.add_argument('--source', required=True, type=pathlib.Path, help='SDP-vNow/SDUI/go')
p.add_argument('--output', required=True, type=pathlib.Path, help='build-ui/libexec/xfmd')
a=p.parse_args();a.output.mkdir(parents=True,exist_ok=True)
for name,flags in [('sdui-preview',[]),('sdui-fyne',['-tags','desktop'])]:
    subprocess.run(['go','build',*flags,'-o',str((a.output/name).resolve()),'./cmd/'+name],cwd=a.source,check=True)
