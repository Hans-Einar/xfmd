import pathlib,subprocess,tempfile,hashlib,json,time,os,signal
(pathlib.Path.home()/'.cache').mkdir(exist_ok=True)
prefix=pathlib.Path('/home/warloc/.local');helper=prefix/'libexec/xfmd/sdui-preview';host=prefix/'libexec/xfmd/sdui-fyne'
with tempfile.TemporaryDirectory(prefix='installed-sdui-') as tmp:
 d=pathlib.Path(tmp);s=d/'fixture.sdui';s.write_text('sdui 0.2; page=["Installed SDUI preview", input("Name",value="Åse"), button("Save")];')
 revision=hashlib.sha256(s.read_bytes()).hexdigest();out=d/'preview';out.mkdir()
 args=['-source',str(s),'-entry','page','-revision',revision]
 check=json.loads(subprocess.check_output([str(helper),*args,'-check'],text=True));assert check['status']=='prototype'
 result=json.loads(subprocess.check_output([str(helper),*args,'-output',str(out)],text=True));assert result['revision']==revision
 assert '```mermaid' in (out/'entry.md').read_text()
 processes=[]
 try:
  for cmd in [[str(prefix/'bin/xfmd'),str(out/'entry.md')],[str(host),'-entry','page','-revision',revision,str(s)]]:
   p=subprocess.Popen(cmd,start_new_session=True);processes.append(p)
  time.sleep(4)
  assert all(p.poll() is None for p in processes),'Installed application/host exited early'
  print('PASS: installed helper readiness and combined preview; installed XFMD and separate Fyne host stayed running in isolated Xvfb/temp HOME')
 finally:
  for p in processes:
   if p.poll() is None:
    os.killpg(p.pid,signal.SIGTERM)
    try:p.wait(timeout=3)
    except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGKILL);p.wait()
