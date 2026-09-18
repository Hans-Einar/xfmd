# Sprinter i XFMD

En sprint er en avgrenset integrasjonsleveranse, ikke en plikt til agile
seremonier eller en innføring av den parallelle SDP-prosessen.

- `Sprint-NNN--Tema/README.md`: mål, krav, faser, konsolideringsbranch og én PR.
- `Sprint-NNN--Tema/Phase-NNN--Tema.md`: branch, milestones, akseptanse og byggbevis.
- Én branch per fase; én commit per milestone. Neste fase starter fra forrige.
- Én PR for hele sprinten, fra siste fasebranch til main; behold alle commits.
- Autonom flerfaseøkt: ett planlagt bygg per avsluttet fase. Ikke bygg ved hver
  dokumentasjons-/milepælcommit. Samarbeidsmodus kan bygge etter hver feilretting.

Numrene er stabile. Fasene fortsetter fra eksisterende P41; første sprintmappe
starter med fase 042. Eldre arbeid blir ikke omskrevet til nye sprintmapper.
[Arbeidsmåte](../docs/working-method.md), [versjonering](../docs/versioning.md),
[fase-mal](templates/Phase.md).
