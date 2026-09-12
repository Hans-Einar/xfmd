# Blueprint-maler

Bruk [feature-malen](Feature-TEMPLATE.md) for en sammenhengende evne og
[functionality-malen](Functionality-TEMPLATE.md) for en avgrenset tjeneste.
Begge har samme åtte kapitteloverskrifter. Kapittel 5 heter alltid **Plumbing**.

1. Reserver neste ID i [registeret](../README.md); behold ID ved omdøping.
2. Kopier til `feature/Feature-NNN--English-Slug.md` eller
   `functionality/Functionality-NNN--English-Slug.md`.
3. Fyll alle felt. Metadata har én linje per felt og komma-separerte ID-er.
   `uses` peker til konsumerte functionality-ID-er; `none` betyr ingen.
4. Oppgi krav-ID-er og konkrete offentlige kall med kildefiler. Marker alle
   uimplementerte kall `Planned`; ikke bruk oppdiktede eksisterende symboler.
5. Beskriv både happy path og avbrudd/feil, eierskap og faktisk/plausibel konsument.
6. Oppdater kravmatrisen og kjør `python3 tools/validate_blueprints.py` fra roten.

Gyldig metadata: kind `Feature/Functionality`, audience `User/System/Integration`,
role `Workflow/Service/Adapter/Mechanism`, owner `application/interpreter/renderer`,
status `Proposed/Ready/Implemented/Verified/Retired`, scope `FirstRelease/Future`.
Feature-role er vanligvis Workflow, men er ikke en kildekodemappe eller et krav
om egen klasse. Se [arbeidsmåten](../../../docs/working-method.md) for statusporter.

Kontrakt-/kildebaner i backticks kan være planlagte. Markdown-fillenker skal alltid
peke til noe som finnes. Testplaner skal aldri omtales som utførte testbevis.
