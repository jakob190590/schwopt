/*
 * GesamtComputer.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <bitset>
#include <cassert>
#include <iomanip>

#include "GesamtComputer.h"
#include "../../Zeit.h"

using namespace std;

const int GesamtComputer::DISZIPLINEN[] =
{
	Disziplin::RUECK_50, Disziplin::BRUST_50, Disziplin::SCHM_50, Disziplin::FREI_50,     // Lagenstaffel
	Disziplin::FREI_50, Disziplin::FREI_50, Disziplin::FREI_50,     // Kraul-
	Disziplin::FREI_50, Disziplin::FREI_50, Disziplin::FREI_50,		// staffel
	Disziplin::BRUST_50, Disziplin::BRUST_100, Disziplin::RUECK_50, Disziplin::RUECK_100, // Einzel-
	Disziplin::SCHM_50, Disziplin::SCHM_100, Disziplin::FREI_50, Disziplin::FREI_100      // starts
};

GesamtComputer::GesamtComputer(const SchwimmerVector& schwimmer) :
		SchwoptAlgoComputer(schwimmer)
{
}

/*
 * Algorithm:
 *
 * Solange freie Positionen
 *   Freie Positionen mit besten freien Schwimmern besetzen
 *   Alle eben eingesetzten Schwimmer nach Abstand absteigend sortieren
 *   Diese sortierte Liste durchgehen:
 *     Bei mehreren gleichwertigen Moeglichkeiten:
 *       Entscheidungssituation nachschlagen
 *       Neue Entscheidungssituation:
 *         Entscheidungssituation speichern
 *         Erste Moeglichkeit und speichern
 *       sonst:
 *         Naechste Moeglichkeit waehlen und speichern
 *
 *
 *     Wenn Schwimmer noch frei:
 *       Diesen Schwimmer festsetzen
 *     sonst:
 *       break;
 *
 * Kleine Optimierung (eigentlich unnoetig):
 * Bei innerer Schleife, ersten Schwimmer immer GLEICH festsetzen, ohne if
 *
 */
void GesamtComputer::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = ANZAHL_POSITIONEN;
	// Positionen, die schon fest vergeben sind
	bitset<ANZAHL_POSITIONEN> vergebenePositionen;
	// Noch verfuegbare Schwimmer
	map<Schwimmer*, int> availableSchwimmer;

	// Size of result setzen!
	result.resize(ANZAHL_POSITIONEN);
	gesamtzeit = 0;
}
