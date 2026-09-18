# Packet, Timeline, Gantt og Journey 1

Status: Implemented P40; UR-039/040, SR-021/022, FUNC-023/024/025.
Packet beskriver konkret encoding, ikke en abstrakt SDL Datagram-familie.
Tidsplaner og brukerreiser supplerer systemmodellen uten å vedta språkregler.

- Packet/packet-beta: eksplisitt bit eller start-end og +count med sitert navn.
  32 bits per rad, maksimalt 4096 bits/128 felt; ingen overlapp. Bibliotekets
  gamle generiske flowchart-dispatch erstattes i forken av bitgeometri.
- Timeline: title, section, period : event og videre : event. Ordnet kategorisk
  tid bevares; intervaller er ikke nødvendigvis proporsjonale kalenderdager.
- Gantt: title, dateFormat YYYY-MM-DD, section; eksplisitt task-ID, startdato
  eller after tidligere-ID, og varighet i hele dager (1–3650d). Valgfri done,
  active eller crit. Andre kalenderformater, excludes, flertallsavhengighet og
  milestones avvises i denne profilen. Ingen skjult kalenderantagelse.
- Journey: title, section, oppgave : score 1–5 : aktører separert med komma.
  Ordnet score/aktørliste bevares; ikke en modell av scheduler eller tråder.

Kontrakten har separate felt for bitposisjoner, periodehendelser, start/varighet/
avhengighet og score/aktører. Renderer mapper til native domeneverdier; ingen
Mermaid-kilde parses i layout. SVG/palett/PDF-porter og arbeidsbudsjetter gjenbrukes.
Kapitlene i galleriet er praktiske akseptanseeksempler, ikke kompatibilitetsløfter.

Kilder: https://mermaid.js.org/syntax/packet.html,
https://mermaid.js.org/syntax/timeline.html,
https://mermaid.js.org/syntax/gantt.html,
https://mermaid.js.org/syntax/userJourney.html.

Datoer begrenses til 1970–2100 og faktisk gyldige kalenderdager.
