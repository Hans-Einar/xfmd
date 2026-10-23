# VP10 — Datagram encoding and packets

Revision: `e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007`.

Only closed contracts with validated Encoding and explicit bit positions.

## Scope and notation

Each packet is one validated fixed Encoding of a Variant in a closed Contract. Bit offsets are logical contract positions; byte-order and bit-order are explicit source declarations. The view does not implement serialization or infer padding, variable lengths, optional-field layout or an executable codec. Header fields belong to the Contract; variant fields belong to the Variant. Independent Encodings are never concatenated.

## Encoding register

No Encodings are declared; no packet layout is inferred.

## Variant coverage

A logical Variant does not define a wire layout. Missing Encoding is a modeling gap, not evidence about native codec implementation.

No Variants are declared.

