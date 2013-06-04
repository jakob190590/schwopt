
Wie verwende ich das Programm schwopt?
Wie lasse ich eine m�glichst optimale Mannschaftszeit beim OMP berechnen?

Beispiele
---------

datafile.txt   ist eine Beispiel-Datendatei, die Schwimmerdaten und -zeiten enth�lt.
inputfile.txt  ist eine Beispiel-Eingabedatei, die Schwimmerk�rzel f�r eine selbst
               definierte Belegung der Positionen enth�lt.

 1. Kommandozeile �ffnen (Eingabeaufforderung, cmd.exe oder eine Shell)
 2. Ins verzeichnis wechseln, in dem schwopt liegt (mit cd)
 3. Oben aufgef�hrte Dateien in dieses Verzeichnis kopieren.
 4. schwopt ausf�hren mit einem der folgenden Befehle:

schwopt --dry   "datafile.txt"          sehen was denn �berhaupt berechnet wird
schwopt -v      "datafile.txt"          berechnen und "wortreich" ausgeben
schwopt --plain "datafile.txt"          berechnen und ausgeben in der Form, die unterhalb als Eingabe erwartet wird
schwopt -vi                     "datafile.txt"             Zeiten berechnen mit eigener Belegung (selbst eingeben)
schwopt --input="inputfile.txt" "datafile.txt"             Zeiten berechnen mit eigener Belegung (aus Datei)
schwopt --block=lagenstaffel    "datafile.txt"             nur f�r die Lagenstaffel berechnen; Achtung: geht nur wenn genuegend Schwimmer eingetragen sind!


Benutztung des Programms schwopt
--------------------------------

usage: schwopt [--help] [or other options] <filename>
 --help             
 --version          
 --class=CLASS      mini-mixed, jugend-w, jugend-m, jugend-mixed, damen, 
                    herren, mixed (default)
 --block=BLOCK      lagenstaffel, schlussstaffel, einzelstarts,
                    gesamt (default)
 -d, --dry          Nicht berechnen, nur leeres Ergebnis anzeigen
 -i, --input[=FILE] Nicht berechnen, Belegung der Positionen selbst eingeben
 -p, --plain        Nur die Schwimmerkuerzel als Ergebnis ausgeben
 -v, --verbose      
