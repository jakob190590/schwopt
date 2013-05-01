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
#include "../Debugging.h"

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

void EinzelstartsComputer::removeFromAvailable(Schwimmer* schw)
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

/*
 * Algorithm:
 *
 * Solange freie Positionen
 *   Freie Positionen mit besten freien Schwimmern besetzen
 *   Alle eben eingesetzten Schwimmer nach Abstand absteigend sortieren
 *   Diese sortierte Liste durchgehen:
 *     Wenn Schwimmer noch frei:
 *       Diesen Schwimmer festsetzen
 *     sonst:
 *       break;
 *
 * Kleine Optimierung (eigentlich unnoetig):
 * Bei innerer Schleife, ersten Schwimmer immer GLEICH festsetzen, ohne if
 *
 */
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
				outputSchwimmerZeiten(clog,
						schwimmerSortiert[DISZIPLINEN[i]].begin(),
						schwimmerSortiert[DISZIPLINEN[i]].end(),
						DISZIPLINEN[i]);
				Schwimmer* schw = *schwimmerSortiert[DISZIPLINEN[i]].begin();
				eingesetzteSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, schw));
				result[i] = schw;
			}

		outputAbstaendeSortiert(clog, eingesetzteSchwimmerSortiertNachAbstand);

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		for (SortedPositionSchwimmerSet::const_iterator it = eingesetzteSchwimmerSortiertNachAbstand.begin();
				it != eingesetzteSchwimmerSortiertNachAbstand.end(); ++it)
		{
			const int position    = it->first;
			Schwimmer* const schw = it->second;
			const int timesAvailable = availableSchwimmer[schw];
			if (timesAvailable > 0)
			{
				// Diesen Schwimmer festsetzen fuer seine Position
				clog << "=========================================" << endl;
				clog << "Schwimmer festsetzen: " << schw->kuerzel << " bei " << Disziplin::convertToString(DISZIPLINEN[position]) << endl;
				outputSchwimmerAbstand(clog, abstaende[DISZIPLINEN[position]], DISZIPLINEN[position]);
				nichtvergebenePositionen--;
				vergebenePositionen[position] = true;
				availableSchwimmer[schw]--;
				if (timesAvailable == 1) // Schwimmer kann kein weiteres Mal eingesetzt werden
					removeFromAvailable(schw);
				gesamtzeit += schw->zeiten[DISZIPLINEN[position]];
				outputSchwimmerAbstand(clog, abstaende[DISZIPLINEN[position]], DISZIPLINEN[position]);
			}
			else
				break;
		}

	}
//	clog << "availableSchwimmer am Ende des Algos" << endl;
//	for (SchwimmerIntMap::const_iterator it = availableSchwimmer.begin();
//			it != availableSchwimmer.end(); ++it)
//		clog << it->first->kuerzel << " " << it->second << endl;
}

ostream& EinzelstartsComputer::outputResult(ostream& os)
{
	os << "Einzelstarts (Lage)" << endl;
	for (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{
		int diszi = DISZIPLINEN[i];
		os << setiosflags(ios::left) << setw(15) << Disziplin::convertToString(diszi, true, false);
		os << setiosflags(ios::right) << setw(8) << Disziplin::convertToString(diszi, false, true, "m") << resetiosflags(ios::right) << " ";
		os << getResult()[i]->kuerzel << "  " << Zeit::convertToString(result[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
	return os;
}

ostream& EinzelstartsComputer::outputSchwimmerAbstand(ostream& os, const SchwimmerAbstandMap& map, int disziplin)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten/Abstand, Disziplin: " << Disziplin::convertToString(disziplin) << endl;
	for (SchwimmerAbstandMap::const_iterator it = map.begin(); it != map.end(); ++it)
	{
		Schwimmer& schw = *it->first;
		os << setiosflags(ios::left);
		os << setw(16) << schw.nachname << setw(10) << schw.vorname << setw(3) << schw.kuerzel;
		os << setiosflags(ios::right);
		os << setw(14) << Zeit::convertToString(schw.zeiten[disziplin]);
		os << setw(14) << Zeit::convertToString(it->second) << endl;
		os << resetiosflags(ios::right);
	}
	return os;
}

ostream& EinzelstartsComputer::outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet& set)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten/Abstand/Position/Disziplin, Sortiert nach Abstand" << endl;
	for (SortedPositionSchwimmerSet::const_iterator it = set.begin(); it != set.end(); ++it)
	{
		Schwimmer& schw = *it->second;
		int position  = it->first;
		int disziplin = DISZIPLINEN[position];

		os << setiosflags(ios::left);
		os << setw(16) << schw.nachname << setw(10) << schw.vorname << setw(3) << schw.kuerzel;
		os << setiosflags(ios::right);
		os << setw(14) << Zeit::convertToString(schw.zeiten[disziplin]);
		os << setw(14) << Zeit::convertToString(abstaende[disziplin][&schw]);
		os << setw(5) << position << ": ";
		os << resetiosflags(ios::right);
		os << Disziplin::convertToString(disziplin) << endl;
	}
	return os;
}
