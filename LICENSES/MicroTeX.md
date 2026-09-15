# MicroTeX

Kilde: https://github.com/NanoMichael/MicroTeX

Commit: `0e3707f6dafebb121d98b53c64364d16fefe481d`.
Arkiv-SHA256: `47476269d29c41df322bce6bdd2daa7017cc50b9eda841b2bec1767dba28daa6`.

Bibliotekkoden er MIT-lisensiert (Copyright 2020 Nano Michael). Installasjonen
inkluderer original LICENSE som `MicroTeX-LICENSE` i dokumentasjonsmappen.
Ressursene har egne vilkår: opprinnelige fontlisenser, README-filer og
Greek/Cyrillic LICENSE følger uendret med hele `share/xfmd/math/`.

XFMD bygger bare kjernen og Cairo-adapteren. Lokale rettelser gjøres eksplisitt
i CMake: korrekt `free` for `asprintf`-minne og inkludering av fontconfig sin
FreeType-deklarasjon, samt opprydding av statisk allokerte makroer også når
matematikk aldri initialiseres. Ingen demoprogrammer eller GTK-widgets bygges.
