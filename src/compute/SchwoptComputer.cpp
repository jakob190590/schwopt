
#include <cstdlib>
#include <algorithm>

#include "SchwoptComputer.h"
#include "../Zeit.h"

bool (*SchwoptComputer::schwimmerZeitLessComperators[])
		(const Schwimmer*, const Schwimmer*) = {
		zeitLess<0>,
		zeitLess<1>,
		zeitLess<2>,
		zeitLess<3>,
		zeitLess<4>,
		zeitLess<5>,
		zeitLess<6>,
		zeitLess<7> };

SchwoptComputer::SchwoptComputer(const SchwimmerVector& schwimmer, const int& numberPositionen) :
		anzahlPositionen(numberPositionen), schwimmer(schwimmer), schwimmerSortiert(Disziplin::ANZAHL), // TODO wenn schwimmer list is, kann man schwimmerSortiert gleich richtig initialisieren
		ergebnis(numberPositionen, (Schwimmer*) NULL), gesamtzeit(0) // SCHEISS DRECKS CPP/GCC!! zeigt mir iwelche unsagbaren fehler an, nur weil NULL nicht gecastet war
{
	// Zeiten von 00:00,0 ersetzen durch maximalen Wert!
	for (SchwimmerVector::const_iterator it = schwimmer.begin();
			it != schwimmer.end(); ++it)
		for (int i = 0; i < Disziplin::ANZAHL; i++)
			if ((*it)->zeiten[i] == 0)
				(*it)->zeiten[i] = Zeit::MAX_UNSIGNED_VALUE;

	// Schwimmer fuer jede Disziplin sortieren
	for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		SchwimmerList& schwList = schwimmerSortiert[i];
		// Schwimmer in leere Liste schwimmerSortiert[i] uebertragen
		schwList.insert(schwList.begin(), schwimmer.begin(), schwimmer.end());
		// Aufsteigend sortieren nach Zeit, d. h. Erster = Bester
		schwList.sort(schwimmerZeitLessComperators[i]);
	}
}
