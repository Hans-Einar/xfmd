# Gesture-scrolling og eksakte scrollgrenser

WheelGuiTest gjenskapte feilen med standard FOX 1.6.57-scrollbarer: gjentatte
SEL_MOUSEWHEEL-hendelser med code=1 lot sidetreet stå 5 piksler fra toppen.
code=120 og code=30 nådde kanten i samme oppsett. Testen feilet før rettelsen.
Den installerte FXScrollBar::onMouseWheel-beregningen bruker heltallsdivisjon
med 120 per hendelse; rest fra små bevegelser ble ikke beholdt.

FoxWheelScrollBar samler rest mellom hendelser, clampler animasjonsmålet og
beholder FOXs timer/varsler. Sidetre, editor og preview bruker samme adapter for
begge akser. Ingen endringer gjøres i systembiblioteket eller andre programmer.
Observasjonen i xfw er forenlig med dette, men xfw er ikke testet eller endret.

Testen bruker ekte FOX-widgets under isolert Xvfb, med vanlig og liten wheel-input
(code ±120, ±30 og ±1), både topp og bunn. Både scrollbarposisjon og faktisk
viewportposisjon må treffe kanten, og animasjonstimeren skal stoppe. Horisontal
scrolling testes i sidetreet med lange filnavn. Dette er testing av FOX-hendelser,
ikke en påstand om testing av brukerens fysiske touchpad eller driver.

Krav: UR-011 / AT-025. Delt adapter: FUNC-005; konsumenter FUNC-010 og FUNC-011.

Verifikasjon: 17/17 Release-tester og 16/16 ASan/UBSan-tester bestått. Etter
utvidelse med native X11-wheel, reversering og Alt/Ctrl ble WheelGuiTest kjørt
på nytt i Release og i samlet sanitizer-suite. Vanlig mouse-wheel går også
X11 → FOX → scrollflate → adapter i testen; små deltaer sendes som FOX-events.
Ingen ventende wheel-timer står igjen når testen har nådd en endegrense.
Eksisterende ScrollingTest kontrollerer fortsatt synkronisering og resize.
