#!/usr/bin/env python3
"""Validate the native GUI export artifact; unsent input must not leak into PDF."""
import pathlib, subprocess, sys
pdf=pathlib.Path(sys.argv[1])
text=subprocess.check_output(['pdftotext',str(pdf),'-'],text=True)
for required in ('C2','simulated snapshot','Ordinary Markdown after BoxUI.'):
    assert required in text, (required,text)
assert 'UNSUBMITTED-DRAFT' not in text
assert 'BoxUI:' not in text, text
info=subprocess.check_output(['pdfinfo',str(pdf)],text=True)
assert 'JavaScript:      no' in info
print('PASS: accepted snapshot, simulation marker, surrounding Markdown; no unsent draft or PDF JavaScript')
