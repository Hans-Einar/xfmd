# Exact Mermaid fork source

The archive selected by `cmake/mermaid-source.json` is an unmodified `git archive`
of the named fork commit, compressed with xz. The SHA-256 validates archive bytes;
the contained prefix names the commit. It includes the fork licenses, vendored
Libavoid source and its verification script. No generated binaries are included.

Session0005 adopts the local VP07 fix and native use-case glyph/layout support
without publishing the fork. The earlier e79d8f9 source archive remains available
for N3b evidence; the manifest selects the current N3c candidate. Bootstrap reads
this repository-relative archive when `archive` is present; otherwise it retains
the existing GitHub download path. Both paths use the mandatory checksum and
Libavoid verification before installation. Existing mismatched sources are never
overwritten. There are no patches applied in `.deps`.

Reproduction: `git archive --format=tar --prefix=mermaid-rs-renderer-COMMIT/ COMMIT`
in the fork, followed by Python `lzma.compress(data, preset=6)`. Commit and checksum
are authoritative in the manifest. A later separately authorized publication can
switch transport to a checksummed GitHub archive of the same tree; it must update
the archive checksum because transport bytes differ.

Session0007 N14 adopts the packet readability correction from local fork commit
49bb0eee49e04310752a63043d7f79d74411e2d9 using the same exact archive policy.
The prior 55eff5d archive remains for historical evidence; the source manifest
selects N14. This is dependency adoption, not publication of the fork.
