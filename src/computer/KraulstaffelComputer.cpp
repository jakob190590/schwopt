/*
 * KraulstaffelComputer.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <cassert>
#include <iomanip>

#include "KraulstaffelComputer.h"
#include "../Zeit.h"

using namespace std;

// Optimale Lösung
KraulstaffelComputer::KraulstaffelComputer(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{
}

/**
 * Algorithmus:
 *
 * Einfach die 4 besten Krauler nehmen.
 *
 */
void KraulstaffelComputer::compute()
{
	result.resize(ANZAHL_POSITIONEN_IN_STAFFEL);
	gesamtzeit = 0;

	SchwimmerList::const_iterator it = schwimmerSortiert[DISZIPLIN].begin();
	for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; ++i, ++it)
	{
		result[i] = (*it);
		gesamtzeit += (*it)->zeiten[DISZIPLIN];
	}
}

ostream& KraulstaffelComputer::outputResult(ostream& os)
{
	string diszi = Disziplin::convertToString(DISZIPLIN);
	for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
	{
		os << setiosflags(ios::left) << setw(23) << diszi << " " << getResult()[i]->kuerzel << "  " << Zeit::convertToString(result[i]->zeiten[DISZIPLIN]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl;
	return os;
}
