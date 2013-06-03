Dokumentation zu schwopt
========================

Allgemein
---------

Dieses Prgramm optimiert die Mannschaftszeit beim Schwimmwettkampf [Oberbayerischer Mannschaftspokal (OMP)](http://www.bsv-oberbayern.de/omp/start.html).  Es setzt die Schwimmer in die Positionen der Wettkämpfe (Staffeln und Einzelstarts) möglichst optimal ein.  Dabei handelt es sich aber nicht um die exakte Lösung sondern eine Näherungslösung.  Diese kann mit der exakten Lösung übereinstimmen.

Der Name des Programms "schwopt" hat mit Schwimmwettkampf und Optimierung zu tun.

Manual
------

    schwopt [OPTION] ... DATAFILE

    Dieses Prgramm optimiert die Mannschaftszeit beim Schwimmwettkampf Oberbayerischer
    Mannschaftspokal (OMP).  Es setzt die Schwimmer in die Positionen der Wettkämpfe (Staffeln
    und Einzelstarts) möglichst optimal ein.

    DATAFILE muss angegeben werden (außer bei --version und --help).  DATAFILE ist eine Textdatei,
    die Namen, Daten und Zeiten der Schwimmer enthält.  Jede Zeile enthält die Daten zu einem
    Schwimmer.  Leerzeilen und Zeilen die mit `#' beginnen werden ignoriert.  Folgende, durch
    Whitespace getrennte Felder müssen für jeden Schwimmer definiert sein, in dieser Reihenfolge:
    Nachname, Vorname, Geschlecht, Kürzel, Bestzeiten für 50 und 100 m (je aufeinanderfolgend)
    Brust, Rückenkraul, Schmetterling, Kraul.  Der Rest der Zeile wird ignoriert.

    --class=CLASS
        Legt die Klasse fest, in der die Mannschaft antritt, z. B. `Jugend männlich'.  Nicht alle
        beim OMP möglichen Klassen werden unterstützt.  Genauergesagt wird bis jetzt nur die
        sog. offene Klasse `Mixed' (d. h. Damen und Herren gemischt) unterstützt.  Als Wert sind
        die folgenden, im OMP möglichen Klassen, erlaubt: mini-mixed, jugend-w, jugend-m,
        jugend-mixed, damen, herren, mixed (default)

    --block=BLOCK
        Legt den Block fest für den die Positionen optimal besetzt werden sollen, z. B.
        `Lagenstaffel'.  Standardmäßig werden alle Wettkämpfe (Staffeln und Einzelstarts) besetzt.
        Es wird nicht garantiert, dass alle Blöcke unterstützt werden.  Für alle Klassen außer
        `Mini Mixed' können grundsätzlich folgende Blöcke berechnet werden:    lagenstaffel,
        schlussstaffel, einzelstarts, gesamt (default)

    -d, --dry
        `Trockenlauf', keine Berechnung durchführen.  Die Ausgabe ist die gleiche wie normal, nur
        dass alle Positionen unbesetzt sind (`N/A').  Durch diese Option kann man sich Positionen
        und Disziplinen eines Blocks ansehen.

    -i, --input[=FILE]
        Die Besetzung der Positionen wird nicht vom Programm berechnet, sondern vom Benutzer
        vorgegeben.  Die Eingabe sind die Kürzel der Schwimmer, getrennt durch Whitespace in der
        Reihenfolge der Positionen des Blocks.  Ungültige Schwimmerkürzel führen dazu, dass die
        Position nicht besetzt wird.  Wenn keine Datei angegeben ist, wird von der Standardeingabe
        gelesen.  Diese Option kann verwendet werden um Zwischenzeiten und Gesamtzeit berechnen
        und anzeigen zu lassen.

    -p, --plain
        Das Ergebnis wird in der Form ausgegeben, die bei der vorigen Option als Eingabe erwartet
        wird: Eine durch Zeilenumbruch getrennte Liste von Schwimmerkürzeln in der Reihenfolge der
        Positionen des Blocks.  Eine leere Zeile bedeutet dass die Position nicht besetzt ist.

    -v, --verbose
        Es wird am Anfang die Schwimmer-Zeiten-Liste und generell mehr Text ausgegeben.

    --version
        Zeigt eine Versionsinformation an.

    --help
        Zeigt eine Kurzform dieser Hilfe an.



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

Design und Architektur sind eher einfach gehalten.  Gründe:

- Mit unsigned zu rechnen ist wahrscheinlich schneller als mit Objekten einer Klasse Zeit.
- Wenn ich anfange Operatoren zu überladen werde ich nicht mehr froh wenn ich konsequent bin.  Und andernfalls wird das Programm inkonsistent.

# Klasse SchwoptComputer

Basisklasse aller Optimierungsrechner.  Stellt Memberfunktionen bereit und auch einige protected Membervariablen, die die Kindklassen verwenden können/sollen.

- `void compute();` // Berechnen
- `unsigned getTime() const;` // Gesamtzeit abfragen
- `SchwimmerVector getResult() const;` // Ergebnis abfragen
- `void outputResult(ostream&) const;` // Ergebnis ausgeben

Achtung: Eine SchwoptComputer-Instanz ist ein "Einmal-und-Wegwerf-Computer".  Die Funktion `compute` darf nur ein einziges Mal an einem Objekt aufgerufen werden -- danach ist das Verhalten undefiniert.  Dadurch wird die Implementierung vereinfacht (es müssen nicht alle Membervariablen als lokale Variablen kopiert werden, sondern können direkt verwendet und manipuliert werden).

Algorithmus
-----------

Der Algorithmus ist grob wie folgt:

Solange es freie Positionen gibt
 1. Freie Positionen mit besten freien Schwimmern besetzen
 2. Den Schwimmer festsetzen, der am meisten Zeit rausholt

"festsetzen" bei 2. heißt: Der Schwimmer wird für die Position fest eingetragen; die Position ist dann nicht mehr frei.  "am meisten Zeit rausholt" in ist so gemeint: Zu jedem Schwimmer ist in jeder Disziplin bekannt, um wieviel er besser ist als der nächstschlechtere Schwimmer.  Am Ende wird der Schwimmer festgesetzt, wo der Abstand zum Nächstschlechteren am größten ist, der also am meisten Zeit rausholt.
