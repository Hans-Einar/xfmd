import sys,subprocess,time,pathlib,os,hashlib,json,signal
binary,fixture,output=sys.argv[1:]
log=pathlib.Path(output)
version=subprocess.check_output([binary,'--version'],text=True).strip()
with log.open('w') as f:
    f.write('Version: '+version+'\n')
    p=subprocess.Popen([binary,fixture],stdout=f,stderr=subprocess.STDOUT,start_new_session=True)
    try:
        time.sleep(3)
        if p.poll() is not None:raise RuntimeError('Packaged GUI exited during startup: '+str(p.returncode))
        f.write('PASS: packaged executable stayed running with the supplied .mmd argument for 3 seconds in isolated Xvfb/HOME.\n')
    finally:
        if p.poll() is None:
            os.killpg(p.pid,signal.SIGTERM)
            try:p.wait(timeout=3)
            except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGKILL);p.wait()
print(version)
