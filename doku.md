Dokumentation zu schwopt
========================

Allgemein
---------

Dieses Prgramm optimiert die Mannschaftszeit beim Schwimmwettkampf [Oberbayerischer Mannschaftspokal (OMP)](http://www.bsv-oberbayern.de/omp/start.html).  Es setzt die Schwimmer in die Positionen der Staffeln und Einzelstarts möglichst optimal ein.  Dabei handelt es sich aber nicht um die exakte Lösung sondern eine Näherungslösung.  Diese kann mit der exakten Lösung übereinstimmen.

Aufruf:

    schwopt <datafilename>

Der Name schwopt hat zu tun mit "Schwimmwettkampfoptimierung".

Begriffe
--------

### Staffel/Einzelstarts

Begriff für die beiden einzelnen Staffeln (Lagenstaffel und Kraulstaffel) bzw. die Lagen-Einzelstarts.

### Position

Die Position innerhalb einer Staffel bzw. bei den Einzelstarts.  Zum Beispiel ist "50 m Schmetterling" die dritte Position innerhalb der Lagenstaffel.

### Disziplin

Eine Disziplin ist nicht nur eine der Lagen "Rücken", "Brust", ... sondern die Lage in Kombination mit Streckenlänge (z. B. 100 m).  "Schmetterling" ist also keine Disziplin, aber "100 m Schmetterling" ist eine Disziplin (unabhängig davon, in welcher Staffel/Einzelstarts sie vorkommt).

Bemerkung: Im eigentlichen Sinne sind "? m Freistil" und "? m Kraul" verschiedene Disziplinen.  Man könnte sogar sagen, dass Freistil keine eigene Disziplin ist.  Freistil kann zwar auf einer Position gefordert sein, in diesem Fall kann aber einfach die Disziplin gewählt werden, in der der Schwimmer am besten ist.  Der Einfachheit halber wird hier aber nicht unterschieden, sondern generell die Disziplin "Freistil" verwendet und vorrausgesetzt, dass die Disziplin "Kraul" im Wettkampf nicht vorkommt.

### Schwimmer

Ein Schwimmer mitsamt seinen Bestzeiten in allen Disziplinen.

Architektur
-----------

Design und Architektur sind eher einfach gehalten.  Gründe:

- Mit unsigned zu rechnen ist wahrscheinlich schneller als mit Objekten einer Klasse Zeit.
- Wenn ich anfange Operatoren zu überladen werde ich nicht mehr froh wenn ich konsequent bin.  Und andernfalls wird das Programm inkonsistent.

# Klasse `OptComputer`

Basisklasse aller Optimierungsrechner.  Stellt Memberfunktionen bereit und auch einige protected Membervariablen, die die Kindklassen verwenden können/sollen.

- `void compute();` // Berechnen
- `unsigned getTime();` // Gesamtzeit abfragen
- `SchwimmerVector getResult();` // Ergebnis abfragen
- `void outputResult(ostream&);` // Ergebnis ausgeben

Achtung: Eine OptComputer-Instanz ist ein "Einmal-und-Wegwerf-Computer".  Die Funktion `compute` darf nur ein einziges Mal an einem Objekt aufgerufen werden -- danach ist das Verhalten undefiniert.  Dadurch wird die Implementierung vereinfacht (es müssen nicht alle Membervariablen als lokale Variablen kopiert werden, sondern können direkt verwendet werden).
