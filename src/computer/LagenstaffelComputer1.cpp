/*
 * LagenstaffelComputer1.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <cassert>
#include <iomanip>

#include "LagenstaffelComputer1.h"
#include "../Zeit.h"
#include "../Debugging.h"

using namespace std;

LagenstaffelComputer1::NormAbstandComparer::NormAbstandComparer(LagenstaffelComputer1& computer) :
		computer(computer)
{
}

bool LagenstaffelComputer1::NormAbstandComparer::operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
{
	// Abstand in Diziplin, die in der Staffel auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
	return computer.normierteAbstaende[DISZIPLINEN_IN_STAFFEL[p1.first]][p1.second] > computer.normierteAbstaende[DISZIPLINEN_IN_STAFFEL[p2.first]][p2.second];
}

void LagenstaffelComputer1::entfAusSchwimmerSortiertUndNormierteAbstaende(int position, Schwimmer* schw)
{
	// Eigentlich reicht's fuer Disziplinen von Positionen der Staffel
	for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		schwimmerSortiert[i].remove(schw);

		SchwimmerFloatMap& abst = normierteAbstaende[i];
		SchwimmerFloatMap::iterator it2, it = abst.find(schw);
		if (it == abst.end()) // nicht gefunden (??)
			continue;

		if (it == abst.begin())
		{
			// nothing to do (except erase)
			abst.erase(it);
			continue;
		}

		// Standardfall: Abstand neu berechnen
		abst.erase(it--); // decrement it before erase
		it2 = it; // it2 soll auf Naechstschlechteren zeigen
		it2++;

		int diszi = DISZIPLINEN_IN_STAFFEL[position];
		unsigned itZeit   = it->first->zeiten[diszi];
		unsigned nextZeit = Zeit::MAX_UNSIGNED_VALUE; // falls it der letzte Schwimmer ist...
		if (it2 != abst.end())
			nextZeit = it2->first->zeiten[diszi];

		it->second = nextZeit / itZeit;
	}
}

LagenstaffelComputer1::LagenstaffelComputer1(const SchwimmerVector& schwimmer) :
		LagenstaffelComputer(schwimmer)
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
			normierteAbstaende[i][*it] = nextZeit / itZeit; // Naechstschlechterer / Aktueller, z.B. 00:14,0 / 00:13,0
		}
	}
}

/*
 * Algorithm:
 *
 * Solange freie Positionen in Staffel
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
void LagenstaffelComputer1::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = ANZAHL_POSITIONEN_IN_STAFFEL;
	// Positionen, die schon fest vergeben sind
	bool vergebenePositionen[ANZAHL_POSITIONEN_IN_STAFFEL] = { false, false, false, false };
	// Noch verfuegbare Schwimmer
	SchwimmerSet availableSchwimmer;
	availableSchwimmer.insert(schwimmer.begin(), schwimmer.end());
	// Size of result setzen!
	result.resize(ANZAHL_POSITIONEN_IN_STAFFEL);
	gesamtzeit = 0;

	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
		clog << "nichtvergebenePositionen == " << nichtvergebenePositionen << endl;

		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		// Alle nicht-vergebenen Schwimmer nach Abstand absteigend sortiert in set einfuegen
		SortedPositionSchwimmerSet eingesetzteSchwimmerSortiertNachAbstand(NormAbstandComparer(*this));
		for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
			if (!vergebenePositionen[i])
			{
				outputSchwimmerZeiten<SchwimmerList::const_iterator>(clog,
					schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]].begin(),
					schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]].end(),
					DISZIPLINEN_IN_STAFFEL[i]);
				Schwimmer* schw = *schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]].begin();
				eingesetzteSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, schw));
				result[i] = schw;
			}

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		for (SortedPositionSchwimmerSet::const_iterator it = eingesetzteSchwimmerSortiertNachAbstand.begin();
				it != eingesetzteSchwimmerSortiertNachAbstand.end(); ++it)
			if (availableSchwimmer.find(it->second) != availableSchwimmer.end()) // beim 1. mal immer true, danach kann's auch false sein!
			{
				// Diesen Schwimmer festsetzen fuer seine Position
				nichtvergebenePositionen--;
				vergebenePositionen[it->first] = true;
				availableSchwimmer.erase(it->second);
				entfAusSchwimmerSortiertUndNormierteAbstaende(it->first, it->second);
				gesamtzeit += it->second->zeiten[DISZIPLINEN_IN_STAFFEL[it->first]];
			}
			else
				break;

	}
}
