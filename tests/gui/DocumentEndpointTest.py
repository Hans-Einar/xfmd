"""Real independent FOX processes: captured target, stale delivery and closed window."""
import os, pathlib, socket, subprocess, sys, tempfile, time
binary=sys.argv[1]
with tempfile.TemporaryDirectory(prefix='xfmd-endpoints-') as tmp:
    root=pathlib.Path(tmp); (root/'nav.md').write_text('# Navigator\n'); (root/'first.md').write_text('# First\n'); (root/'second.md').write_text('# Second\n')
    env=dict(os.environ,XDG_RUNTIME_DIR=tmp)
    children=[]
    def packet(window,message):
        with socket.socket(socket.AF_UNIX,socket.SOCK_SEQPACKET) as s:
            s.settimeout(4);s.connect(str(root/'xfmd'/f'{window}.sock'));s.send(message.encode());return s.recv(32769).decode()
    def state(window):return packet(window,f'XFMD1\tINFO\t{window}\n')
    def opened(window,seq,path,pane='main'):
        return packet(window,f'XFMD1\tOPEN\t{window}\ttest\t{seq}\t{pane}\t{path}\tend\n')
    try:
        for window in ['one','two']:
            p=subprocess.Popen([binary,str(root/'first.md'),'--navigator',str(root/'nav.md'),'--window-id',window],env=env,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True);children.append(p)
            for _ in range(100):
                if (root/'xfmd'/f'{window}.sock').exists():break
                if p.poll() is not None:raise RuntimeError(p.communicate())
                time.sleep(.03)
        # The second process is now the newly shown window. Explicit delivery is to one.
        assert opened('one',1,root/'second.md')=='OK\t1\n'
        assert str(root/'second.md') in state('one')
        assert str(root/'first.md') in state('two')
        assert str(root/'nav.md') in state('one')
        assert 'stale' in opened('one',1,root/'first.md')
        assert 'ERROR' in opened('one',2,root/'missing.md')
        assert str(root/'second.md') in state('one')
        assert 'ERROR' in opened('one',3,root/'first.md','invalid')
        assert 'ERROR' in packet('one','x'*32769)
        assert 'ERROR' in packet('one','XFMD1\tINFO\ttwo\n')
        assert opened('two',1,root/'second.md','navigation')=='OK\t1\n'
        assert str(root/'second.md') in state('two')
        children[0].terminate();children[0].wait(timeout=5)
        result=subprocess.run([binary,'--window','one','--pane','main',str(root/'first.md')],env=env,capture_output=True,text=True,timeout=6)
        assert result.returncode!=0 and 'closed or unavailable' in result.stderr
        print('PASS: two native windows, explicit pane, stale/invalid/oversize requests, retained view, closed target')
    finally:
        for p in children:
            if p.poll() is None:p.terminate()
            p.communicate(timeout=5)
