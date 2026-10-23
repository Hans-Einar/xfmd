# MM-002 — Install the verified local candidate

Owner Session 0002 T004, 2026-10-05, explicitly requests installation of the
prepared MM-001 candidate. Applied SDP Worker within this bounded deployment.
No source change or rebuild: installed the exact checksum-verified package.

- Destination: /home/warloc/.local
- Version: xfmd 0.4 sprint/009/phase/057-favorites:206+dirty
- Binary SHA256: e361fdf1ae5972eedf1af8ffd6e20fade6e15387a79828bea9ca130771aeb371
- Receipt: /home/warloc/.local/state/xfmd/installations/20261004T234147Z/receipt.json
- Previous files: /home/warloc/.local/state/xfmd/installations/20261004T234147Z/previous
- All 168 installed files match the package. Per-file atomic replacement and
  rollback protection preserve the previous install if verification fails.
- Installed executable passed isolated Xvfb/temp-HOME startup with the raw Mermaid
  fixture. Prior 85/85 candidate regression evidence remains applicable; no full
  rerun or new independent-review claim.
- Existing user windows were not restarted; new launches use this installation.

[Receipt](receipt.json), [startup log](smoke.log), [installation script](install.py).
Independent review and owner feature acceptance remain pending. No publication,
tag, Git integration or SDL/SDUI source change occurred.
