import pathlib,subprocess,tempfile,json,hashlib,os
helper=pathlib.Path('/home/warloc/git/xfmd-sdl-navigation/build-ui/libexec/xfmd/sdui-preview')
with tempfile.TemporaryDirectory() as d:
 d=pathlib.Path(d);s=d/'source with spaces.sdui';s.write_text('sdui 0.2; page=["Å source"];')
 revision=hashlib.sha256(s.read_bytes()).hexdigest();out=d/'output';out.mkdir()
 def call(*args,ok=True):
  r=subprocess.run([str(helper),'-source',str(s),'-entry','page',*args],capture_output=True,text=True)
  assert (r.returncode==0)==ok,(r.returncode,r.stderr)
  return r
 report=json.loads(call('-check','-revision',revision).stdout);assert report['status']=='prototype'
 call('-check','-revision','stale',ok=False)
 reply=json.loads(call('-revision',revision,'-output',str(out)).stdout)
 assert reply['revision']==revision
 assert (out/'source.sdui').read_bytes()==s.read_bytes()
 md=(out/'entry.md').read_text();assert '```mermaid' in md and 'Static GUI dump' in md and 'sdui-source://' in md
 call('-output',str(out),ok=False)
 link=d/'link';link.symlink_to(out,target_is_directory=True);call('-output',str(link),ok=False)
 call('-entry','missing','-check',ok=False)
print('PASS: preview/check, revision, snapshot, combined output, existing/symlink output, missing entry')
