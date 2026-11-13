# XFMD local RPM CI

The owner selected automatic publication from `main` on 2026-11-11 (KB-XFMD-032).
This follows minibrowser's local-build/server-signing workflow. GitHub Actions
verifies pull requests and manual runs; main commits use the local pipeline.
GitHub does not publish XFMD RPMs.

## Build and verify

Run on the AlmaLinux 10.2 x86_64 build host with rootless Podman, Git, Python 3.11+,
SSH, curl, tar, rpmkeys and the existing Xvfb test tool. The digest-pinned AlmaLinux
image installs distribution/EPEL build dependencies and pinned private FOX 1.6.57.
The application contains no private SDP engine or SDUI helper payload.
The RPM declares the OS X11/Wayland/OpenGL libraries needed by the managed
native SDUI host; the same libraries are provisioned in qualification images. Qualification
requires a separately established canonical one-file shared distribution, supplied
as a directory containing `gh-sdp`, its exact source commit and binary SHA256.
The pipeline checks embedded source/version identity and copies that test input
into the evidence output. This does not create an XFMD engine/update owner.
Xvfb is a read-only test-tool mount, identified by hash, because the target repos
do not ship its server. No host application libraries enter the package. Test temporary files use tmpfs
to avoid container-overlay SELinux attributes that cannot be copied there. Image
format plugins are an explicit runtime dependency, not just a build dependency.

```sh
python3 packaging/rpm/local-ci.py --ref HEAD --shared-engine /absolute/shared-distribution --shared-engine-commit <engine-sha> --shared-engine-sha256 <binary-sha256> --plan
python3 packaging/rpm/local-ci.py --ref HEAD --shared-engine /absolute/shared-distribution --shared-engine-commit <engine-sha> --shared-engine-sha256 <binary-sha256> --output /tmp/xfmd-candidate
python3 packaging/rpm/local-ci.py --ref <reviewed-sha> --output /tmp/xfmd-candidate --publish-existing --host beinerud
```

Use a fresh output directory. Only committed inputs are included. The disposable checkout is detached at the
exact candidate; source_ref and source commit remain in provenance. The build
copies full Git history, generates truthful clean metadata, runs all configured
CTest cases and structural checks, then builds one RPM. A clean base container
installs the exact signed 0.10.0-76.el10_2 predecessor pinned in predecessor.json,
upgrades to the candidate, checks the native window as an unprivileged user,
CLI and assets, reinstalls, removes and installs again. The predecessor is
downloaded from the existing public repository and checked against its committed
signed digest and repository key. The selected reading workflow runs against the
separately established shared engine. The installed probes invoke actual `setup`,
then exercise registered default selection with gh absent, authentication removed
and networking disabled via an isolated network namespace. The successor upgrade
must remove the old package-owned private engine.

The package installs `/usr/bin/xfmd`, desktop/icons/manpage, math resources,
licenses and JSON provenance. Private shared FOX resides in `/usr/lib64/xfmd`
with an application RPATH and no advertised system FOX provider. Corresponding
bundled native-library sources accompany the package. The project license is
still owner-unspecified (`LicenseRef-Unspecified`); no license grant is invented.
XFMD0.11 requires registered shared SDPTool for SDP workflows; setup and updates
are owned by its gh extension distribution. The RPM
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


## Activity0004 successor feature-branch publication

Owner selects product0.11 and Activity4 successor delivery without merging main.
Publish only after the coordinator's integrated qualification and independent
review gate and canonical/shared extension publication. The main watcher retains
its independent configuration. Configure its `shared_engine`,
`shared_engine_commit` and `shared_engine_sha256` values (or installer options with
hyphens) before using the0.11 pipeline; an absent exact input fails qualification.
No running watcher installation is changed by this source work.


To publish a previously verified output after coordinating review, use
`python3 packaging/rpm/local-ci.py --ref <exact-sha> --output <verified-directory> --publish-existing --host beinerud`.
This verifies source/provenance/shared-engine hash and package digest before uploading the same
bytes. It never rebuilds. Public validation performs an actual signed0.10-to0.11
DNF upgrade before checking a fresh signed public installation.
