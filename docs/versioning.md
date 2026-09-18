# Versjon og byggidentitet

XFMD viser `major.minor branch:commitnummer`, eksempelvis
`0.1 sprint/001/phase/042-build-identity:181`. På `main` brukes nummeret til
siste integrerte PR når dette finnes: `0.1 PR#34:179`.

`VERSION` er eneste kilde til major/minor. Major endres ved bevisst større
produkt-/kompatibilitetsbrudd; minor ved en besluttet funksjonsleveranse.
Fase, milestone eller bygg øker ikke automatisk major/minor. Nåværende
produktlinje beholder `0.1`. CPack bruker teknisk pakkeversjon `0.1.0`.

Commitnummer er `git rev-list --count HEAD`: antall forskjellige commits som
kan nås fra byggets commit, inkludert merge-commits. Vanlig commit øker tallet
med én; merge kan øke med flere fordi hele fasens historie kommer inn.
Fasebrancher arver historikken og teller videre. Parallelle brancher kan ha
samme tall; branch/PR og produktversjon inngår derfor i identiteten. Dette er
ikke en global nummerserver som fordeler unike tall mellom parallelle brancher.
Ingen versjonsfil må endres for hver commit, og en ren clone gir samme nummer.

Publisert historie skal ikke rebases, squash-merges eller amendes: det ville
endre tellingen. Integrer sprint-PR med merge-commit. På main leses siste
`Merge pull request #N` langs first-parent-historien; dokumentasjonscommits etter
mergen beholder PR-navnet og får nytt nummer. Uten kjent PR vises `main`.
Git-SHA lagres bare som teknisk sporbarhet i byggmetadata, ikke som commitnummer.

## Bygg og særtilfeller

Versjonen genereres ved eksplisitt CMake-bygg, ikke ved commit og ikke ved oppstart.
Den vises av `xfmd --version` og i vindustittelen. Det tilføyes ingen ekstra
verktøylinje. Bygget skriver også `build/generated/xfmd-build.json` med full
kildeidentitet. Et allerede bygget program endrer ikke versjon når Git flyttes.

- Lokal branch bruker sitt faktiske navn. Detached checkout vises som `detached`;
  PR-testcheckout i GitHub Actions som `ci/<head-branch>`.
- Ucommittede endringer, inkludert uignorerte nye filer, merkes `+dirty`.
  Slike bygg er lokale prøver, ikke publiserte fasebygg.
- Shallow Git-historikk avvises med beskjed om `git fetch --unshallow`.
  CI må hente full historikk. Vi viser aldri et misvisende lavt commitnummer.
- Kildearkiv uten Git vises som `major.minor source:unknown`. Det får ikke et
  oppdiktet commitnummer. Distribuerte binærer beholder innbakt identitet.
- Genererte filer skrives bare når innholdet endres; nytt bygg av samme rene
  commit skal ikke utløse en ny relink bare på grunn av dato/klokkeslett.

Byggfrekvens og dokumentasjon følger [arbeidsmåten](working-method.md).
