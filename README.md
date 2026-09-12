# xfmd — X File Markdown Viewer/Editor

En planlagt lettvekts Markdown-viser og enkel editor for Linux, bygget på FOX
som companion til `xfw` og `xfi`. Prosjektet skal tilby native Markdown-visning,
live preview, dokumentnavigasjon med historikk og synkronisert scrolling.

**Status: arkitektur- og designfase. Ingen applikasjon er implementert ennå.**
Dokumentene er forslag til gjennomgang; planlagte funksjoner og tester er ikke
påstander om levert funksjonalitet.

## Les prosjektet

1. [Krav og use cases](xfmd_requirements.md)
2. [Arkitektur, kontrakter og kildefilkart](softwareArchitecture.md)
3. [Arbeidsmåte for features og functionality](docs/working-method.md)
4. [Blueprint-register og sporbarhetsmatrise](src/blueprint/README.md)
5. [Implementeringsplan](implementationPlan.md)
6. [Agent- og bidragsregler](AGENTS.md)

## Struktur

```text
src/
  application/   FOX-applikasjon, roller og adaptere
  interpreter/   Markdown → semantisk modell
  renderer/      Semantisk modell → layout og tegnekommandoer
  contracts/     Små, delte grensesnitt og datatyper
  blueprint/
    feature/       Fire sammenhengende evner
    functionality/ Elleve tjenester og mekanismer
    templates/     Faste maler; kapittel 5 er alltid Plumbing
```

Kodekatalogene inneholder foreløpig ansvarsdokumentasjon. Ingen byggkommandoer er
operative før implementeringsplanens P1. Dokumentkontrollen kan kjøres nå:

```sh
python3 tools/validate_blueprints.py
```

FOX er fast applikasjonsteknologi; interpreter og renderer er uavhengige bak
kontrakter. Ingen browser engine, skripteksekvering eller eksterne bildenedlastinger.
Framtidig `xfw`-IPC er utsatt og krever egen spesifikasjon.

Prosjektlisens er foreløpig ikke valgt. Repositoryet inneholder prosjektets egne
designtekster og kontrollverktøy; tredjeparts kildekode er ikke inkludert.
