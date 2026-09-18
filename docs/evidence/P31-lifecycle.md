# P31: opprydding av SVG-fontkart

GitHub Ubuntu 24.04 / ASan fant Fontconfig-allokeringer fra librsvgs Pango-
tekstrenderer ved avslutning av MermaidGuiTest, DiagramReadingTest og PDF-testen.
RsvgHandle-eierskapet var korrekt, men Pango beholdt sitt per-tråd standardkart.

SvgDiagramCache frigjør nå RsvgHandle-oppføringene først og kaller deretter
pango_cairo_font_map_set_default(nullptr) ved avslutning av trådens cache.
Dette gjenbruker samme oppryddingsmekanisme som MathTypesetter, og innfører
ingen nye LeakSanitizer-suppresjoner.

En isolert Ubuntu 24.04-container med librsvg/Pango og g++ -fsanitize=address
kompilerte den faktiske SvgDiagramCache.cpp og rendret SVG-tekst til Cairo.
Etter rettelsen: ingen lekkasjefeil. Bare den eksisterende, snevre FcInit-
suppresjonen (320 bytes / 3 allokeringer) ble brukt. Full CI kjøres på nytt.
