/*
 * LagenstaffelComputer1.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>

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
	return computer.abstaende[DISZIPLINEN_IN_STAFFEL[p1.first]][p1.second] > computer.abstaende[DISZIPLINEN_IN_STAFFEL[p2.first]][p2.second];
}

LagenstaffelComputer1::GeschlechtPredicate::GeschlechtPredicate(const Schwimmer::Geschlecht& g) :
		geschlecht(g)
{
}

bool LagenstaffelComputer1::GeschlechtPredicate::operator ()(const Schwimmer* s)
{
	return s->geschlecht == geschlecht;
}

void LagenstaffelComputer1::entfAusSchwimmerSortiertUndAbstaende(Schwimmer* schw)
{
	// Eigentlich reicht's fuer Disziplinen der Staffel
	for (int disziplin = 0; disziplin < Disziplin::ANZAHL; disziplin++) // (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
	{	// int disziplin = DISZIPLINEN_IN_STAFFEL[i];
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

void LagenstaffelComputer1::ensureMixedBedingung()
{
//	// Fuer "Mixed"-Bedingung
//	neededGeschlecht[it->second->geschlecht]--;
//	Schwimmer::Geschlecht geschlecht; // welche Schwimmer sollen aus den Verfuegbaren geloescht werden
//	if (neededGeschlecht[Schwimmer::MAENNLICH] == 0)
//		geschlecht = Schwimmer::MAENNLICH; // keine weiteren Schwimmer mehr erlaubt (der wahrscheinlichere Fall)
//	else if (neededGeschlecht[Schwimmer::WEIBLICH] == 0)
//		geschlecht = Schwimmer::WEIBLICH;  // keine weiteren Schwimmerinnen mehr erlaubt
//	else
//		return;
//
//	SchwimmerSet::iterator it;
//	GeschlechtPredicate pred(geschlecht);
//	while ((it = find_if(availableSchwimmer.begin(), availableSchwimmer.end(), pred))
//			!= availableSchwimmer.end())
//	{
//		entfAusSchwimmerSortiertUndAbstaende(*it);
//		availableSchwimmer.erase(it);
//	}
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
			abstaende[i][*it] = nextZeit - itZeit; // Naechstschlechterer - Aktueller
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
	SchwimmerSet availableSchwimmer(schwimmer.begin(), schwimmer.end());
	// "Mixed"-Bedingungen: 2 Schwimmer, 2 Schwimmerinnen
	int neededGeschlecht[2] = { 2, 2 };
	// Size of result setzen!
	result.resize(ANZAHL_POSITIONEN_IN_STAFFEL);
	gesamtzeit = 0;

	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
//		clog << "nichtvergebenePositionen == " << nichtvergebenePositionen << endl;

		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		// Alle nicht-vergebenen Schwimmer nach Abstand absteigend sortiert in set einfuegen
		SortedPositionSchwimmerSet eingesetzteSchwimmerSortiertNachAbstand(NormAbstandComparer(*this));
		for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
			if (!vergebenePositionen[i])
			{
//				outputSchwimmerZeiten<SchwimmerList::const_iterator>(clog,
//					schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]].begin(),
//					schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]].end(),
//					DISZIPLINEN_IN_STAFFEL[i]);
				Schwimmer* schw = *schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]].begin();
				eingesetzteSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, schw));
				result[i] = schw;
			}

//		outputAbstaendeSortiert(clog, eingesetzteSchwimmerSortiertNachAbstand);
		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		for (SortedPositionSchwimmerSet::const_iterator it = eingesetzteSchwimmerSortiertNachAbstand.begin();
				it != eingesetzteSchwimmerSortiertNachAbstand.end(); ++it)
			if (availableSchwimmer.find(it->second) != availableSchwimmer.end()) // beim 1. mal immer true, danach kann's auch false sein!
			{
				// Diesen Schwimmer festsetzen fuer seine Position
//				outputSchwimmerAbstand(clog, abstaende[DISZIPLINEN_IN_STAFFEL[it->first]], DISZIPLINEN_IN_STAFFEL[it->first]);
				nichtvergebenePositionen--;
				vergebenePositionen[it->first] = true;
				availableSchwimmer.erase(it->second);
				entfAusSchwimmerSortiertUndAbstaende(it->second);
				ensureMixedBedingung();
				// TODO eigntlich muss ich doch abbrechen, schwimmer von den Verfuegbaren geloescht werden! weil abstand sich evtl geaendert hat und auch wegen ensureMixedBedingung
//				outputSchwimmerAbstand(clog, abstaende[DISZIPLINEN_IN_STAFFEL[it->first]], DISZIPLINEN_IN_STAFFEL[it->first]);
				gesamtzeit += it->second->zeiten[DISZIPLINEN_IN_STAFFEL[it->first]];
			}
			else
				break;

	}
}


ostream& LagenstaffelComputer1::outputSchwimmerAbstand(ostream& os, const SchwimmerAbstandMap& map, int disziplin)
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

ostream& LagenstaffelComputer1::outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet& set)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten/Abstand/Position/Disziplin, Sortiert nach Abstand" << endl;
	for (SortedPositionSchwimmerSet::const_iterator it = set.begin(); it != set.end(); ++it)
	{
		Schwimmer& schw = *it->second;
		int position  = it->first;
		int disziplin = DISZIPLINEN_IN_STAFFEL[position];

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
