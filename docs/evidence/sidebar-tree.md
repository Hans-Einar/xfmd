# Sidepanel: rotklikk og bevart synlighet

Brukerrapport: klikk på `/` ser ut som krasj; klikk på Markdown-filer kan lukke
sidepanelet. SidebarGuiTest gjenskapte automatisk skjuling ved native klikk på `/`.
Selve prosesskrasjet er ikke gjenskapt.

SidebarWidget var sitt eget FOX-target med selector 1, som også er FXWindow::ID_HIDE.
FXTreeList sender SEL_COMMAND ved klikk. Den arvede kommandohåndteringen skjulte
derfor widgeten. ID_TREE_EVENT starter nå ved FXDirList::ID_LAST. Dobbeltklikk
sjekker dessuten isItemFile slik at en mappe kalt folder.md ikke åpnes som fil.

SidebarGuiTest kjører i isolert Xvfb og sender X11 ButtonPress/ButtonRelease gjennom
FOX-eventloopen. Testen feilet før rettelsen ved første klikk på rot-elementet.
Den kontrollerer nå:

- Enkelt-/dobbeltklikk på `/` bevarer vindu, dokument og synlig sidepanel.
- En mappe med .md-suffiks navigeres uten dokumentåpning eller feil.
- Enkeltklikk på fil velger; dobbeltklikk åpner fil og bevarer panelet og bredden.
- Eksplisitt skjuling via ViewModeController beholdes over dokumentåpning.
- Eksplisitt visning fungerer etterpå.

Krav: UR-001/006/007, AT-001/006/007. Eier: FUNC-010. ViewModeController er fortsatt
synlighetseier; det er ikke lagt til automatisk show() som overstyrer brukerens valg.

Verifikasjon: 16/16 tester bestått i Release og 15/15 med ASan/UBSan på lokalt
FOX 1.6.57-miljø. Ingen sanitizer-feil ved rotklikk, mappeutvidelse, dokumentbytte
eller opprydding. Blueprint-, symbol- og lagkontroll inngår i CTest.
