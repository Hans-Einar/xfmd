# P35: Etikettpekere i SVG

Status: Implemented P35; videreført i P41s [regresjonsbevis](../evidence/P41.md).

UR-039/041 og FUNC-024: en tynn peker med prikk på eierkanten gjør
etikettilhørighet synlig. Dette er presentasjon etter fullført ruting. Ingen
node, kant, port, etikett eller kildekobling flyttes av presentasjonssteget.

Forkens add_label_leaders tar ferdig SVG, Layout, Theme og config.
Venstre kandidater prioriteres når de er frie; høyre er fallback. Loddrett
målsegment gir vannrett peker fra midten av etikettkanten. Vannrett målsegment
gir 45° ut fra øvre/nedre hjørne, deretter loddrett til kanten. Skarpe hjørner,
tynnere strek og liten prikk ved målet. Faktiske etikettmål og SVG-padding brukes.

Kandidater må holde klaring til noder, etiketter, andre kanter og allerede
valgte pekere; pilspisser og hjørner unngås. Ingen fri kandidat: utelat pekeren
og oppgi antall i diagnostikk, behold etiketten og diagrammet. Ingen tvungen
kryssing eller ny ruting. Begrenset kandidatmengde og geometribudsjett.

Rust-layoutadapteren aktiverer dette etter render_svg/render_svg_with_crossings.
FOX/Cairo bruker vanlig SVG og Light/Dark-palett. Tester skal kontrollere sider,
45°-geometri, kollisjon/fallback, SVG-identitet og uendret logisk rute. Visuell
kontroll av de tre brukerdiagrammene og eksisterende GUI/PDF-tester kreves.
