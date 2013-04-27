/*
 * OptComputer.cpp
 *
 *  Created on: 17.04.2013
 *      Author: jakob190590
 */

#include <algorithm>
#include <climits>

#include "OptComputer.h"
#include "Schwimmer.h"

bool (*OptComputer::schwimmerZeitLowerComperators[])
		(const Schwimmer*, const Schwimmer*) = {
		zeitLower<0>,
		zeitLower<1>,
		zeitLower<2>,
		zeitLower<3>,
		zeitLower<4>,
		zeitLower<5>,
		zeitLower<6>,
		zeitLower<7> };

OptComputer::OptComputer(const SchwimmerVector& schwimmer) :
		schwimmer(schwimmer)
{
	// Zeiten von 00:00,0 ersetzen durch maximalen Wert!
	for (SchwimmerVector::const_iterator it = schwimmer.begin();
			it != schwimmer.end(); ++it)
		for (int i = 0; i < Disziplin::ANZAHL; i++)
			if ((*it)->zeiten[i] == 0)
				(*it)->zeiten[i] = UINT_MAX;

	// Schwimmer fuer jede Disziplin sortieren und Vergleichswerte berechnen
	for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		SchwimmerList& schwSorted = schwimmerSortiert[i];
		// Schwimmer in leeren Vektor schwimmerSortiert[i] uebertragen
		schwSorted.insert(schwSorted.begin(), schwimmer.begin(), schwimmer.end());
		// Aufsteigend sortieren nach Zeit, d. h. Bester <=> Erster
		schwSorted.sort(schwimmerZeitLowerComperators[i]);

		// Normierte Abstaende zw. Schwimmern fuer aktuelle Disziplin berechnen
		SchwimmerList::const_iterator it, first, previous;
		it = first = previous = schwSorted.begin();
		for (; it != schwSorted.end(); ++it)
		{
			abstandZumBesten[i].push_back((*it)->zeiten[i] / (*first)->zeiten[i]);
			abstandZumBesseren[i].push_back((*it)->zeiten[i] / (*previous)->zeiten[i]);

			// previous erhoehen, aber it einen Schritt Vorsprung lassen!
			if (previous != it) ++previous;
		}
	}
}
