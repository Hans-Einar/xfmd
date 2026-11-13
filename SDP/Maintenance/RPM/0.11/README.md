# XFMD0.11 shared-runtime release work

Scope: ACTIVITY-SDP-0004 / PLAN-SDP-0029 C3–C5, coordinated by SESSION-SDP-0021.
Local progress: [Session0011](../../../Sessions/session-%230011--Shared-tool-consumer.md).
Prior public release: signed0.10.0-76.el10_2; its immutable receipt remains in0.10.
[Checklist](ReleaseChecklist.md) records current gate disposition.

The RPM no longer owns an SDP engine/helper or producer source pin. Build and
installed qualification take a separately established exact-hash shared engine
bundle. Runtime compatibility follows wire schema/capabilities. Tests establish
registration through actual setup and prove no gh/auth/network dependency.
No successor RPM, independent approval or publication has yet been claimed.
