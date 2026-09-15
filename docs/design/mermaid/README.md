# Mermaid designfixtures

Disse er brukerens to eksempler, bevart som input til AT-059.
De brukes i Rust-profiler, DiagramLayoutTest, native preview og MermaidPdfTest.

- [Servicekart](service-map.mmd): 9 noder, 9 toveis kanter med etiketter.
- [Lag og levering](layer-delivery.mmd): 9 noder, 11 enveis kanter og sykler.

P26 teller noder/kanter etter parsing, kontrollere endepunkter og etiketter,
og viser at mapping gjennom den rene modellen ikke mister kantretning eller
parallelle forbindelser. P27–P29 bruker samme input for native preview og PDF.
