# XFMD local RPM CI

The owner selected automatic publication from `main` on 2026-11-11 (KB-XFMD-032).
This follows minibrowser's local-build/server-signing workflow. GitHub Actions
continues source verification; it does not publish XFMD RPMs.

## Build and verify

Run on the AlmaLinux 10.2 x86_64 build host with rootless Podman, Git, Python 3.11+,
SSH, curl, tar, rpmkeys and the existing Xvfb test tool. The digest-pinned AlmaLinux
image installs distribution/EPEL build dependencies and pinned private FOX 1.6.57.
Xvfb is a read-only test-tool mount, identified by hash, because the target repos
do not ship its server. No host application libraries enter the package.

```sh
python3 packaging/rpm/local-ci.py --ref HEAD --plan
python3 packaging/rpm/local-ci.py --ref HEAD --output /tmp/xfmd-candidate
python3 packaging/rpm/local-ci.py --ref HEAD --publish --host beinerud --output /tmp/xfmd-publication
```

Use a fresh output directory. Only committed inputs are included. The build
copies full Git history, generates truthful clean metadata, runs all configured
CTest cases and structural checks, then builds one RPM. A clean base container
installs a synthetic older RPM, upgrades to the candidate, checks the native
window, CLI and assets, reinstalls, removes and installs again. The fixture is
not a historical release and is never uploaded. Optional real-tool tests need
separately supplied tools; no claim of bundled SDPTool/SDUI or whole-system
acceptance follows from the default CTest suite.

The package installs `/usr/bin/xfmd`, desktop/icons/manpage, math resources,
licenses and JSON provenance. Private shared FOX resides in `/usr/lib64/xfmd`
with an application RPATH and no advertised system FOX provider. Corresponding
bundled native-library sources accompany the package. The project license is
still owner-unspecified (`LicenseRef-Unspecified`); no license grant is invented.
SDPTool and SDUI helpers are optional separate installations. This first RPM
does not overwrite `~/.local/bin/xfmd`; that earlier local executable can shadow
`/usr/bin/xfmd` according to PATH. Use the absolute path when verifying the RPM.

Version is product major.minor plus `.0`; Release is first-parent commit count
plus `.el10_2`. The visible application version stays major.minor. See
[versioning](../../docs/versioning.md). Full Git history and immutable published
history are required. Output retains source, build/test/install logs, package list,
image identity, unsigned hashes and signed-download/public-DNF evidence. Build
cache locking prevents overlapping use; no signing keys leave Beinerud.

## Server publication

Copy `packaging/repository/` into a staging directory on Beinerud and run
`sudo -n bash install-server.sh` there. It installs only the XFMD receiver,
restricted service account and promoter permission. It reuses the existing key,
Apache mapping, stable/development channels and `beinerud-repo-refresh`; it does
not regenerate keys or replace other publishers or repository configuration.

The receiver accepts one bounded flat XFMD RPM batch. Promotion validates actual
RPM identity, target, version/release and absence of scriptlets, rejects conflicting
replacements, signs on the server, updates metadata and records publication.
The local pipeline downloads and verifies the resulting signature using the
checked-in public key, then installs the exact NEVRA through public HTTPS DNF
with package and metadata signature checks. Only then is publication successful.
If upload or post-publication checks fail, inspect the remote state before retrying.
Unchanged already-published package content can be retried idempotently.

## Automatic main watcher

```sh
packaging/rpm/install-builder.sh --host beinerud --enable
systemctl --user status xfmd-builder.timer xfmd-builder.service
journalctl --user -u xfmd-builder.service
```

The installer defaults to `~/.ssh/known_hosts`, copies only that public host-key
file and installs state under `~/.local/state/xfmd-builder`. The user timer checks
main every minute with a dedicated clean checkout; uncommitted developer work is
excluded. Existing Git credentials and SSH authorization are used noninteractively.
Enable user lingering to survive logout/reboot if it is not already enabled.
No private credentials are written to units or configuration.

A successful candidate is not rebuilt. Failed/interrupted candidates retain their
logs and wait for explicit retry or a new main commit. Rewritten main history and
backward/equal package versions are rejected. Locks prevent overlapping watchers.

```sh
python3 ~/.local/state/xfmd-builder/watch-main.py --config ~/.local/state/xfmd-builder/config.json --retry
systemctl --user stop xfmd-builder.timer
```

Stopping the timer does not interrupt an already running build. Reinstall watcher
files after changing the maintained watcher, when no build is active. Preserve
verified artifacts; retention is manual.

## Install from the public repository

```sh
sudo dnf install epel-release dnf-plugins-core
sudo dnf config-manager --set-enabled crb
sudo curl -fsSL https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/RPM-GPG-KEY-beinerud -o /etc/pki/rpm-gpg/RPM-GPG-KEY-beinerud
sudo dnf config-manager --add-repo https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/beinerud-apps.repo
sudo dnf --refresh install xfmd
/usr/bin/xfmd --version
sudo dnf --refresh upgrade xfmd
```

Repository signing fingerprint: `8E0E49A101DDF1C7C15532FCCB7BB881901AF029`.
Install the trusted public key at the location declared by the repository file,
`/etc/pki/rpm-gpg/RPM-GPG-KEY-beinerud`, before first use.
