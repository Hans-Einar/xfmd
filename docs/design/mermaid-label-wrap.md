# P33: Tekstombryting i kantetiketter

Krav: UR-039, SR-021 og SR-024. Eier: FUNC-024; konsument: FTR-010.

Lange kantetiketter brytes ved mellomrom før nodeplassering og ruting, med
120 pt som ønsket maksimal linjebredde. Ord deles ikke; et lengre enkeltord
kan overskride grensen. Eksplisitte linjeskift bevares. Node-/gruppetitler
beholder sin utforming; når samme tekst også brukes som kantetikett, beholdes
felles uombrutt måling fordi bibliotekets målekontrakt er tekstnøkkelt.

DiagramTextLayout eier tekstforming og faktiske ITextMetrics-mål. Layoutpayload
4 sender både original tekstnøkkel, ferdige linjer, bredde og høyde til Rust.
Rust leverer samme TextBlock til bibliotekets layout og SVG. C ABI forblir v1;
sceneoutput forblir payload 3. Cacheidentitet endres for ny tekstpolicy.
Parser, Mermaid-kilde, bibliotekpin og rutingsalgoritme endres ikke.

Akseptanse: lange/Unicode-etiketter, eksplisitte linjeskift, lange enkeltord,
lik tekst i node og kant, SVG med faktiske linjer og de tre etablerte
regresjonsdiagrammene. Sammenlign før/etter-SVG visuelt. Økt teksthøyde kan
kreve andre ruter eller gi plassmangel; eksisterende validering beholdes.

## Videre arbeid: begrenset omplassering

Et mulig separat forsøk er å låse upåvirkede noder etter første layout og bare
gi utvalgte noder frihet langs x, y eller begge akser. Dette krever en motor
med eksplisitte posisjons-/aksebegrensninger, eller en egen begrenset
kompakteringsfase. Det er ikke en innstilling i libavoid. Bevar rangering,
grupper, nodeavstand og deterministisk arbeidsbudsjett, og vurder forbedring
etter ny ruting med de samme måleverdiene. Ingen slik fase implementeres i P33.
