/*
 * EinzelstartsComputer.cpp
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#include <map>

#include "EinzelstartsComputer.h"

using namespace std;

EinzelstartsComputer::EinzelstartsComputer(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{
}

void EinzelstartsComputer::compute()
{
	map<Schwimmer*, int> anzahlAvailableSchwimmer; // am anfang fuer jeden Schwimmer mit 3 initialisiert!
}
