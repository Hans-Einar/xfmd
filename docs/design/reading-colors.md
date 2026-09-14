# Lesefarger: forskning og praktisk fargevalg

Dette er en avgrenset litteratursjekk, ikke en systematisk oversikt eller en
medisinsk anbefaling. Formålet er å begrunne justerbare lesefarger i xfmd.

## Hva resultatene støtter

Kromatisk aberrasjon betyr at øyets brytning varierer med bølgelengde. Et forsøk
med smalbåndet lys fant ingen forverring av synsskarphet for de undersøkte
smalbåndsfargene unntatt blått. Forsøkets lyskilder og måleoppsett kan ikke direkte
oversettes til et universelt fargevalg for vanlig tekst på alle skjermer.
[Ocular accommodation and wavelength (2024)](https://pmc.ncbi.nlm.nih.gov/articles/PMC10910436/).

«Monokrom» tekst i et brukergrensesnitt betyr én tekstfarge. Det betyr ikke at
skjermen sender ut én bølgelengde: gul på en vanlig RGB-skjerm lages med røde og
grønne primærer. HSV kan styre kanalverdier, men gjør ikke skjermen til en
monokromatisk lyskilde. Derfor er «gul er alltid skarpere enn hvit» ingen
konklusjon vi kan trekke fra optikk alene.

Buchner og Baumgartner fant bedre korrekturprestasjon med mørk tekst på lys
bakgrunn enn omvendt, både ved forskjellige lysforhold og undersøkte fargepar.
Dette er målt korrekturprestasjon, ikke en regel om individuell komfort ved lang
lesing i mørke. [Studien fra 2007](https://pubmed.ncbi.nlm.nih.gov/17510822/).

Piepenbrock et al. fant mindre pupiller og bedre korrekturprestasjon med positiv
polaritet. Luminans, pupillstørrelse og skriftstørrelse er dermed relevante ved
siden av fargetone. [Studien fra 2014](https://pubmed.ncbi.nlm.nih.gov/25135324/).

## Kontrast og standarder

WCAG angir minst 4,5:1 for normal tekst, og 7:1 for enhanced contrast. Det er
terskler for tilgjengelighet, ikke en modell for preferanse, øyetretthet eller
beste skjermlysstyrke. [W3Cs forklaring](https://www.w3.org/WAI/WCAG22/Understanding/contrast-minimum.html),
[enhanced contrast](https://www.w3.org/WAI/WCAG22/Understanding/contrast-enhanced.html).

Med sRGB/WCAG-formelen gir #FFFFFF på #000000 21:1, mens #FFFF00 på #000000 gir
omtrent 19,56:1. Gul har altså ikke høyere beregnet luminanskontrast enn hvit i
akkurat dette tilfellet. Personlig komfort kan likevel være annerledes.

## Valg i xfmd

- Light starter med nøytral hvit bakgrunn og mørkegrå tekst; Dark med mørkegrå
  bakgrunn og lysegrå tekst. Begge standardpar har kontrast over 7:1.
- Fire sliders: bakgrunn til venstre, tekst til høyre; hue øverst og brightness
  nederst. Hue -1 velger nøytralt; 0–359 bruker full HSV-metning. Value 0–100
  skalerer RGB-kanalene. Value er ikke perseptuelt jevn lyshet eller monitorens
  baklys. F.eks. 60° gir gul og 120° grønn; rav ligger mellom rødt og gult.
- Beregnet kontrast mellom vanlig tekst og bakgrunn vises. Brukeren kan prøve
  også lav kontrast; kontrollene tvinger ikke et bestemt fargepar.
- Light og Dark lagres hver for seg i eksisterende preferanser. Repaint skjer
  under dragging; avsluttet input lagrer. Reset colors nullstiller bare aktivt tema.
- Ingen ny parsing eller layout ved fargejustering. Tekst, lenkeunderstreking,
  kode og tabellflater bruker samme lesepalett. PDF beholder utskriftspaletten.
