/*
 * EinzelstartsComputer.cpp
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#include <map>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "EinzelstartsComputer.h"
#include "../Zeit.h"

using namespace std;

const int EinzelstartsComputer::DISZIPLINEN[] =
{
	Disziplin::BRUST_50, Disziplin::BRUST_100, Disziplin::RUECK_50, Disziplin::RUECK_100, // Einzel-
	Disziplin::SCHM_50, Disziplin::SCHM_100, Disziplin::FREI_50, Disziplin::FREI_100      // starts
};

EinzelstartsComputer::NormAbstandComparer::NormAbstandComparer(EinzelstartsComputer& computer) :
		computer(computer)
{
}

bool EinzelstartsComputer::NormAbstandComparer::operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
{
	// Abstand in Diziplin, die in der Staffel auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
	return computer.abstaende[DISZIPLINEN[p1.first]][p1.second] > computer.abstaende[DISZIPLINEN[p2.first]][p2.second];
}

EinzelstartsComputer::EinzelstartsComputer(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{
	// Normierten Abstand zum Naechstschlechteren berechnen
	for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		SchwimmerList& schwSorted = schwimmerSortiert[i];

		// Normierte Abstaende zw. Schwimmern fuer aktuelle Disziplin berechnen
		SchwimmerList::const_iterator it, next;
		it = next = schwSorted.begin();
		next++; // Naechstschlechterer Schwimmer
		for (; it != schwSorted.end(); ++it)
		{
			unsigned itZeit = (*it)->zeiten[i];
			unsigned nextZeit;
			if (next == schwSorted.end())
				nextZeit = UINT_MAX;
			else
			{
				nextZeit = (*next)->zeiten[i];
				++next; // next iterator schon mal erhoehen
			}

			assert(nextZeit >= itZeit); // Fehlerhafte Sortierung oder schwerer Fehler im Algo
			abstaende[i][*it] = nextZeit - itZeit; // Naechstschlechterer - Aktueller
		}
	}
}

void EinzelstartsComputer::entfAusSchwimmerSortiertUndAbstaende(Schwimmer* schw)
{
	// Eigentlich reicht's fuer Disziplinen der Staffel
	for (int disziplin = 0; disziplin < Disziplin::ANZAHL; disziplin++) // (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{	// int disziplin = DISZIPLINEN[i];
		SchwimmerList& schwimmerzeitList = schwimmerSortiert[disziplin]; // list, sorted by zeiten in disziplin, with Schwimmer*
		SchwimmerAbstandMap& abstandsMap = abstaende[disziplin]; // map, sorted by abstand der zeiten in disziplin, Schwimmer* => unsigned

		abstandsMap.erase(schw);

		SchwimmerList::iterator it = find(schwimmerzeitList.begin(), schwimmerzeitList.end(), schw);
		if (it == schwimmerzeitList.end()) // nicht gefunden (??)
			continue;

		if (it == schwimmerzeitList.begin())
		{
			// nothing to do (except remove from list)
			schwimmerzeitList.remove(schw);
			continue;
		}

		// Standardfall: Abstand neu berechnen
		it--; // decrement it before remove
		schwimmerzeitList.remove(schw);

		SchwimmerList::iterator next = it; // next soll auf Naechstschlechteren zeigen
		next++;

		unsigned itZeit   = (*it)->zeiten[disziplin];
		unsigned nextZeit = Zeit::MAX_UNSIGNED_VALUE; // falls it der letzte Schwimmer ist...
		if (next != schwimmerzeitList.end())
			nextZeit = (*next)->zeiten[disziplin];

		abstandsMap[*it] = nextZeit - itZeit;
	}
}

void EinzelstartsComputer::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = ANZAHL_POSITIONEN;
	// Positionen, die schon fest vergeben sind
	bool vergebenePositionen[ANZAHL_POSITIONEN] = { false, false, false, false };
	// Noch verfuegbare Schwimmer
	SchwimmerIntMap availableSchwimmer;
	for (SchwimmerVector::const_iterator it = schwimmer.begin();
			it != schwimmer.end(); ++it)
		availableSchwimmer[*it] = 3; // Jeder Schwimmer maximal 3 x

	// Size of result setzen!
	result.resize(ANZAHL_POSITIONEN);
	gesamtzeit = 0;

	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
		clog << "nichtvergebenePositionen == " << nichtvergebenePositionen << endl;

		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		// Alle nicht-vergebenen Schwimmer nach Abstand absteigend sortiert in set einfuegen
		SortedPositionSchwimmerSet eingesetzteSchwimmerSortiertNachAbstand(NormAbstandComparer(*this));
		for (int i = 0; i < ANZAHL_POSITIONEN; i++)
			if (!vergebenePositionen[i])
			{
				Schwimmer* schw = *schwimmerSortiert[DISZIPLINEN[i]].begin();
				eingesetzteSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, schw));
				result[i] = schw;
			}

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		for (SortedPositionSchwimmerSet::const_iterator it = eingesetzteSchwimmerSortiertNachAbstand.begin();
				it != eingesetzteSchwimmerSortiertNachAbstand.end(); ++it)
			if (availableSchwimmer[it->second] > 0)
			{
				// Diesen Schwimmer festsetzen fuer seine Position
				nichtvergebenePositionen--;
				vergebenePositionen[it->first] = true;
				availableSchwimmer[it->second]--;
				entfAusSchwimmerSortiertUndAbstaende(it->second);
				gesamtzeit += it->second->zeiten[DISZIPLINEN[it->first]];
			}
			else
				break;

	}
	for (SchwimmerIntMap::const_iterator it = availableSchwimmer.begin();
			it != availableSchwimmer.end(); ++it)
		clog << it->first->kuerzel << " " << it->second << endl;
}

ostream& EinzelstartsComputer::outputResult(ostream& os)
{
	os << "Einzelstarts (Lage)" << endl;
	for (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{
		int diszi = DISZIPLINEN[i];
		os << setiosflags(ios::left) << setw(23) << Disziplin::convertToString(diszi) << " " << getResult()[i]->kuerzel << "  " << Zeit::convertToString(result[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
	return os;
}
