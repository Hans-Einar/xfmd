import subprocess,time,json
from pathlib import Path
root='/home/warloc/git/xfmd-sdl-navigation';tool='/tmp/xfmd-n15-sdptool';out=Path('/tmp/xfmd-n15-probe');out.mkdir(exist_ok=True)
results={}
for name,args in [('eager',['discover']),('lazy',['discover','--navigation','lazy-sdl'])]:
 start=time.monotonic();r=subprocess.run([tool,root,*args,'--json'],capture_output=True,timeout=150)
 (out/(name+'.json')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
 results[name]={'code':r.returncode,'seconds':time.monotonic()-start,'bytes':len(r.stdout)}
 if name=='lazy':
  assert r.returncode==0,r.stderr.decode();p=json.loads(r.stdout)
  results[name].update(sources=len(p['sources']),sdlModels=len(p['inventory']['models']),nodes=len(p['navigation']['nodes']),deferred=sum(n.get('childrenDeferred',False) for n in p['navigation']['nodes']))
  m=next(s for s in p['sources'] if s['source']=='SDP/SDL/XFMD/XfmdDesktop.design') if any(s['source']=='SDP/SDL/XFMD/XfmdDesktop.design' for s in p['sources']) else next(s for s in p['sources'] if 'XfmdDesktop' in s['source'] and s['state']=='validated')
  start=time.monotonic();r=subprocess.run([tool,root,'tree','--model',m['id'],'--revision',m['revision'],'--model-only','--json'],capture_output=True,timeout=150)
  (out/'expanded.json').write_bytes(r.stdout);(out/'expanded.stderr').write_bytes(r.stderr);assert r.returncode==0,r.stderr.decode()
  tree=json.loads(r.stdout);assert tree['model']==m['id'] and tree['revision']==m['revision'];assert tree['roots']==['sdl']
  results['expansion']={'code':r.returncode,'seconds':time.monotonic()-start,'bytes':len(r.stdout),'model':m,'nodes':len(tree['nodes']),'overviews':sum('overviewTarget' in n for n in tree['nodes'])}
(out/'summary.json').write_text(json.dumps(results,indent=2)+'\n');print(json.dumps(results,indent=2))
