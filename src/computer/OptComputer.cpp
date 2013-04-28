/*
 * OptComputer.cpp
 *
 *  Created on: 17.04.2013
 *      Author: jakob190590
 */

#include <algorithm>

#include "OptComputer.h"
#include "../Zeit.h"

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
				(*it)->zeiten[i] = Zeit::MAX_UNSIGNED_VALUE;

	// Schwimmer fuer jede Disziplin sortieren
	for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		SchwimmerList& schwList = schwimmerSortiert[i];
		// Schwimmer in leere Liste schwimmerSortiert[i] uebertragen
		schwList.insert(schwList.begin(), schwimmer.begin(), schwimmer.end());
		// Aufsteigend sortieren nach Zeit, d. h. Erster = Bester
		schwList.sort(schwimmerZeitLowerComperators[i]);
	}
}

unsigned OptComputer::getTime()
{
	return gesamtzeit;
}

SchwimmerVector OptComputer::getResult()
{
	return result;
}

