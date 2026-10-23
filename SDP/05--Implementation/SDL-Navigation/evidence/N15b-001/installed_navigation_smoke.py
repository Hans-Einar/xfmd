"""Observe the installed GUI launching its private lazy-discovery producer."""
import os,pathlib,signal,subprocess,sys,time,json
prefix=pathlib.Path('/home/warloc/.local')
env=dict(os.environ);env.pop('XFMD_SDP_TOOL',None)
p=subprocess.Popen([str(prefix/'bin/xfmd'),sys.argv[1]],env=env,start_new_session=True,stdout=subprocess.DEVNULL,stderr=subprocess.PIPE)
seen=[]
def descendants(pid):
    try:
        children=set()
        for task in pathlib.Path(f'/proc/{pid}/task').iterdir():
            try:children.update((task/'children').read_text().split())
            except OSError:pass
    except OSError:return []
    result=[]
    for child in children:
        result.append(child);result.extend(descendants(child))
    return result
try:
    deadline=time.monotonic()+25
    while time.monotonic()<deadline:
        assert p.poll() is None,'Installed GUI exited early'
        for pid in descendants(p.pid):
            try:
                argv=pathlib.Path(f'/proc/{pid}/cmdline').read_bytes().decode().split('\0')[:-1]
            except OSError:continue
            if argv and argv[0]==str(prefix/'libexec/xfmd/sdptool') and argv not in seen:seen.append(argv)
        if any('discover' in a and 'lazy-sdl' in a for a in seen): break
        time.sleep(.01)
    assert any('discover' in a and 'lazy-sdl' in a for a in seen),seen
    print(json.dumps({'result':'pass','observedPrivateProducerCommands':seen,'version':subprocess.check_output([str(prefix/'bin/xfmd'),'--version'],text=True).strip()},indent=2))
finally:
    children=descendants(p.pid)
    for child in children:
        try:os.kill(int(child),signal.SIGTERM)
        except ProcessLookupError:pass
    if p.poll() is None:
        os.killpg(p.pid,signal.SIGTERM)
        try:p.wait(timeout=3)
        except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGKILL);p.wait()
