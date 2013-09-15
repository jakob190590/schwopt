SCHWOPT 1 "MAY 2013" Linux "User Manuals"
=========================================

NAME
----

schwopt - optimiert die Mannschaftszeit beim OMP

SYNOPSIS
--------

`schwopt` [*OPTIONS*] ... *DATAFILE*

DESCRIPTION
-----------

Dieses Prgramm optimiert die Mannschaftszeit beim Schwimmwettkampf [Oberbayerischer Mannschaftspokal (OMP)](http://www.bsv-oberbayern.de/omp/start.html) (dort zählt nämlich die _Gesamtmannschaftszeit_).  Das Programm setzt die Schwimmer in die Positionen der Wettkämpfe (Staffeln und Einzelstarts) möglichst optimal ein, um die Mannschaftszeit zu minimieren.

Der Name des Programms "schwopt" hat mit Schwimmwettkampf und Optimierung zu tun.

OPTIONS
-------

`-b`
  Do not write "busy" to stdout while processing.

`-c` *config-file*
  Use the alternate system wide *config-file* instead of */etc/foo.conf*. This
  overrides any `FOOCONF` environment variable.

`-a`
  In addition to the baz segments, also parse the blurfl headers.

`-r`
  Recursive mode. Operates as fast as lightning at the expense of a megabyte
  of virtual memory.
  


`--class=`*CLASS*
  Legt die Wertungsklasse fest, in der die Mannschaft antritt, z. B. `Jugend männlich'.
  Die Wertungsklasse Mini Mixed wird derzeit nicht unterstützt.  Als Wert sind die
  folgenden, im OMP möglichen Klassen, erlaubt: mini-mixed, jugend-w, jugend-m,
  jugend-mixed, damen, herren, mixed (default)

`--block=`*BLOCK*
  Legt den Block fest für den die Positionen optimal besetzt werden sollen, z. B.
  `Lagenstaffel'.  Standardmäßig werden alle Wettkämpfe (Staffeln und Einzelstarts) besetzt.
  Es wird nicht garantiert, dass alle Blöcke unterstützt werden.  Für alle Klassen außer
  `Mini Mixed' können grundsätzlich folgende Blöcke berechnet werden: lagenstaffel,
  schlussstaffel, einzelstarts, gesamt (default)

`-d`, `--dry`
  `Trockenlauf', keine Berechnung durchführen.  Die Ausgabe ist die gleiche wie normal, nur
  dass alle Positionen unbesetzt sind (`N/A').  Durch diese Option kann man sich Positionen
  und Disziplinen eines Blocks ansehen.

`-i`, `--input`[`=`*FILE*]
  Die Besetzung der Positionen wird nicht vom Programm berechnet, sondern vom Benutzer
  vorgegeben.  Die Eingabe sind die Kürzel der Schwimmer, getrennt durch Whitespace in der
  Reihenfolge der Positionen des Blocks.  Ungültige Schwimmerkürzel führen dazu, dass die
  Position nicht besetzt wird.  Wenn keine Datei angegeben ist, wird von der Standardeingabe
  gelesen.  Diese Option kann verwendet werden um Zwischenzeiten und Gesamtzeit berechnen
  und anzeigen zu lassen.

`-p`, `--plain`
  Das Ergebnis wird in der Form ausgegeben, die bei der Option --input als Eingabe erwartet
  wird: Eine durch Zeilenumbruch getrennte Liste von Schwimmerkürzeln in der Reihenfolge der
  Positionen des Blocks.  Der Wert `N/A' bedeutet dass die Position nicht besetzt ist.
  Dieser Wert sollte also nicht als Kürzel für einen Schwimmer verwendet werden!

`-v`, `--verbose`
  Es wird am Anfang die Schwimmer-Zeiten-Liste und generell mehr Text ausgegeben.

`--version`
  Zeigt eine Versionsinformation an.

`--help`
  Zeigt eine Kurzform dieser Hilfe an.

DATAFILE
--------

DATAFILE -- die Datendatei -- muss angegeben werden (außer bei `--version` und `--help`).  Die
Textdatei enthält persönliche Daten der Schwimmer und deren Bestzeiten in allen Disziplinen.

Die Datendatei enthält persönliche Daten und Bestzeiten der Schwimmer.  Die Datendatei ist eine
Textdatei.  Jede Zeile enthält Daten zu genau einem Schwimmer.  Ein Schwimmerdatensatz besteht
aus folgenden Feldern:

Nachname, Vorname, Geschlecht (m/w), Kuerzel (z. B. Initialien), Brust 50 m, Brust 100 m,
Rücken 50 m, Rücken 100 m, Schmetterling 50 m, Schmetterling 100 m, Kraul 50 m, Kraul 100 m

Die Feldwerte müssen durch Whitespace getrennt sein.  Zeiten für die Disziplinen müssen in
folgendem Format `01:26,8' vorliegen.  Leerzeilen und Zeilen die mit einem Doppelkreuz (`#')
beginnen werden ignoriert.

DIAGNOSTICS
-----------

**0**
  Success

**1**
  Error (see stderr)

BUGS
----

Please see or report at https://github.com/jakob190590/schwopt/issues?state=open

AUTHOR
------

Jakob Schöttl <jschoett@gmail.com>

SEE ALSO
--------

https://github.com/jakob190590/schwopt
