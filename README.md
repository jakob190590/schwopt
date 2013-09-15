Dokumentation zu schwopt
========================

Allgemein
---------

Dieses Prgramm optimiert die Mannschaftszeit beim Schwimmwettkampf [Oberbayerischer Mannschaftspokal (OMP)](http://www.bsv-oberbayern.de/omp/start.html) (dort zählt nämlich die _Gesamtmannschaftszeit_).  Das Programm setzt die Schwimmer in die Positionen der Wettkämpfe (Staffeln und Einzelstarts) möglichst optimal ein, um die Mannschaftszeit zu minimieren.

Der Name des Programms "schwopt" hat mit Schwimmwettkampf und Optimierung zu tun.

Benutzung
---------

[Hier ist die manual page](https://github.com/jakob190590/schwopt/tree/master/man/manual-page.markdown) von schwopt.

Hier die Synopsis des Programms:

    schwopt [OPTION] ... DATAFILE

Und hier ein Beispielaufruf des Programms:

    schwopt -v datafile.txt

[Weitere Beispiele](https://github.com/jakob190590/schwopt/tree/master/samples) zum Aufruf des Programms.

Begriffe
--------

### Block

Begriff für die beiden einzelnen Staffeln (Lagenstaffel und Kraulstaffel) bzw. die Lagen-Einzelstarts.
Also, 4 x 50 m Lagen sind ein Block; die Kraulstaffel (6 x 50 m Kraul) ist ein Block.  Die Einzelstarts werden in zwei Blöcke unterteilt: 4 x 50 m und 4 x 100 m.

### Position

Eine Position im Wettkampf.  Zum Beispiel ist "50 m Schmetterling" die dritte Position innerhalb der Lagenstaffel.

### Disziplin

Eine Disziplin ist nicht nur eine der Lagen "Rücken", "Brust", ... sondern die Lage in Kombination mit Streckenlänge (z. B. 100 m).  "Schmetterling" ist also keine Disziplin, aber "100 m Schmetterling" ist eine Disziplin (unabhängig davon, in welcher Staffel/Einzelstarts sie vorkommt).

Bemerkung: Im eigentlichen Sinne sind "? m Freistil" und "? m Kraul" verschiedene Disziplinen.  Man könnte sogar sagen, dass Freistil keine eigene Disziplin ist.  Freistil kann zwar auf einer Position gefordert sein, in diesem Fall kann aber einfach die Disziplin gewählt werden, in der der Schwimmer am besten ist.  Der Einfachheit halber wird hier aber nicht unterschieden, sondern generell die Disziplin "Freistil" verwendet und vorrausgesetzt, dass die Disziplin "Kraul" im Wettkampf nicht vorkommt.

Strecke ist die Länge der Strecke in Metern, die bei einer Disziplin geschwommen werden muss.

### Schwimmer

Ein Schwimmer mitsamt seinen Bestzeiten in allen Disziplinen.

Architektur
-----------

### Klasse SchwoptComputer

`SchwoptComputer` ist die Basisklasse aller Optimierungsrechner.  Sie stellt Memberfunktionen bereit und auch einige protected Membervariablen, die die Kindklassen verwenden können/sollen.

 - `void compute();` // Berechnen
 - `unsigned getTime() const;` // Gesamtzeit abfragen
 - `SchwimmerVector getResult() const;` // Ergebnis abfragen
 - `void outputResult(ostream&) const;` // Ergebnis ausgeben

Achtung: Eine SchwoptComputer-Instanz ist ein "Einmal-und-Wegwerf-Computer".  Die Funktion `compute` darf nur ein einziges Mal an einem Objekt aufgerufen werden &ndash; danach ist das Verhalten undefiniert.  Dadurch wird die Implementierung vereinfacht (es müssen nicht alle Membervariablen als lokale Variablen kopiert werden, sondern können direkt verwendet und manipuliert werden).

Für jeden Block bzw. den gesamten Wettkampf gibt es je eine abgeleitete Klasse.  Diese Klassen dienen wiederum als Basisklassen für die Implementierungen.  In der offenen Wertungsklasse "Mixed" gibt es z. B. die abgeleiteten Klassen `Lagenstaffel`, `Kraulstaffel` (auch "Schlussstaffel" genannt), `Einzelstarts` und `Gesamt`.

Algorithmus
-----------

### Exakte Lösung

Die exakte Lösung kann durch Durchprobieren ermittelt werden.  Der Algorithmus ist grob wie folgt:

    Schwimmer durchprobieren (in geschachtelten Schleifen):
    
    Jeden Einzelnen für 1. Position
      Jeden Einzelnen für 2. Position
        Jeden Einzelnen für 3. Position
          ...
            Jeden Einzelnen für n-te Position
    
    Dabei müssen ständig die Bedingungen überprüft werden.
    Es wird die Kombination gewählt, bei der die Gesamtzeit am geringsten ist.

Laufzeit: Sei n die Anzahl der Schwimmer und m die Anzahl der Positionen.  Dann ist die Laufzeit O(n^m), weil im schlimmsten Fall für jede der m Positionen n Schwimmer durchprobiert werden müssen.

Nachteile: Nicht-rekursiv programmiert erfordert diese Lösung schon bei vier Positionen [sehr viel Code] [impl_exakt].  Die Implementierung ist sehr fehlerträchtig.  Die Laufzeit ist sehr schlecht (exponentiell).  Bei der Lagenstaffel mit vier Positionen ist das kein Problem, aber bei 18 Positionen (Staffeln + Einzelstarts) wird man's merken &ndash; es dauert dann nämlich ca. siebenhundertmilliarden mal so lange).

### Näherungslösung

Der "Schwopt"-Algorithmus stellt eine Näherungslösung dar, die vermutlich mit der exakten Lösung übereinstimmen kann.  Die Schritte sind grob wie folgt:

Solange es freie Positionen gibt

 1. Freie Positionen mit besten freien Schwimmern besetzen
 2. Den Schwimmer festsetzen, der am meisten Zeit rausholt

Bemerkungen zu 2.:

 - "festsetzen" heißt: Der Schwimmer wird für die Position fest eingetragen; die Position ist dann nicht mehr frei.
 - "am meisten Zeit rausholt" ist so gemeint: Zu jedem Schwimmer ist in jeder Disziplin bekannt, um wieviel er besser ist als der nächstschlechtere Schwimmer.  Am Ende wird der Schwimmer festgesetzt, bei dem der Abstand zum Nächstschlechteren am größten ist, der also am meisten Zeit rausholt.

Laufzeit: Bei m Positionen ist die Komplexität im Wesentlichen O(m), wenn man die Funktionsaufrufe an die [STL](http://www.cplusplus.com/reference/stl/ "C++ Standard Template Libary") nicht beachtet.

Eine Implementierung für die Wertungsklasse "Mixed" [kann man sich hier ansehen] [impl_schwopt].

[impl_exakt]: https://github.com/jakob190590/schwopt/blob/v1_0_1/src/compute/mixed/LagenstaffelExaktComputer.cpp#L34 "compute/mixed/LagenstaffelExaktComputer.cpp"
[impl_schwopt]: https://github.com/jakob190590/schwopt/blob/v1_0_1/src/compute/mixed/GesamtComputer.cpp#L141 "compute/mixed/GesamtComputer.cpp"
