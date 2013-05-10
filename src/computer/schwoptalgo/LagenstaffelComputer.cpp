/*
 * LagenstaffelComputer.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "LagenstaffelComputer.h"
#include "../../Zeit.h"
#include "../../Debugging.h"

using namespace std;

LagenstaffelComputer::NormAbstandComparer::NormAbstandComparer(SchwoptAlgoComputer& computer) :
		computer(computer)
{
}

bool LagenstaffelComputer::NormAbstandComparer::operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
{
	// Abstand in Diziplin, die in der Staffel auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
	return computer.abstaende[computer.disziplinen[p1.first]][p1.second] > computer.abstaende[computer.disziplinen[p2.first]][p2.second];
}

void LagenstaffelComputer::removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer)
{
	availableSchwimmer.erase(schw);

	// Eigentlich reicht's fuer Disziplinen der Staffel
	for (int disziplin = 0; disziplin < Disziplin::ANZAHL; disziplin++) // (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
	{	// int disziplin = DISZIPLINEN_IN_STAFFEL[i];
		SchwimmerList& schwimmerzeitList = schwimmerSortiert[disziplin]; // list, sorted by zeiten in disziplin, with Schwimmer*
		SchwimmerAbstandMap& abstandsMap = abstaende[disziplin]; // map, sorted by abstand der zeiten in disziplin, Schwimmer* => unsigned

		abstandsMap.erase(schw);

		SchwimmerList::iterator it = find(schwimmerzeitList.begin(), schwimmerzeitList.end(), schw);
		assert(it != schwimmerzeitList.end()); // schw muss in der list sein

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

LagenstaffelComputer::PositionSchwimmerPair* LagenstaffelComputer::findMostWanted(PositionSchwimmerPairVector& vec)
{
	PositionSchwimmerPair* result = NULL;
	// Abstand in Diziplin auf der angegebenen Position, fuer den Schwimmer, der fuer diese Position vorgesehen ist
	unsigned greatestAbstand = 0;

	for (PositionSchwimmerPairVector::iterator it = vec.begin();
			it != vec.end(); ++it)
	{
		unsigned abstand = abstaende[disziplinen[it->first]][it->second];
		if (abstand > greatestAbstand)
		{
			greatestAbstand = abstand;
			result = &*it;
		}
	}
	return result;
}

void LagenstaffelComputer::ensureMixedBedingung(Schwimmer& schw, int neededGeschlecht[2], SchwimmerSet& availableSchwimmer)
{
	// Zum sicherstellen der "Mixed"-Bedingung
	if (--neededGeschlecht[schw.geschlecht] > 0) // zaehler nach dekrement noch groesser 0
		return; // nichts weiter zu tun

	SchwimmerSet::iterator it;
	Schwimmer::GeschlechtPredicate pred(schw.geschlecht);
	while ((it = find_if(availableSchwimmer.begin(), availableSchwimmer.end(), pred))
			!= availableSchwimmer.end())
		// hier wird it ungueltig, wegen "erase(it)", deshalb "while (find_if"
		removeFromAvailable(*it, availableSchwimmer);
}

LagenstaffelComputer::LagenstaffelComputer(const SchwimmerVector& schwimmer) :
		SchwoptAlgoComputer(schwimmer)
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
				nextZeit = Zeit::MAX_UNSIGNED_VALUE;
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
 *   Ersten Schwimmer festsetzen
 *
 * Kleine Optimierung (eigentlich unnoetig):
 * Bei innerer Schleife, ersten Schwimmer immer GLEICH festsetzen, ohne if
 *
 */
void LagenstaffelComputer::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = disziplinen.size();
	// Positionen, die schon fest vergeben sind
	vector<bool> vergebenePositionen(disziplinen.size(), false);
	// Noch verfuegbare Schwimmer
	SchwimmerSet availableSchwimmer(schwimmer.begin(), schwimmer.end());
	// "Mixed"-Bedingungen: 2 Schwimmer, 2 Schwimmerinnen
	int neededGeschlecht[2] = { 2, 2 };
	// Size of result setzen!
	result.resize(disziplinen.size());
	gesamtzeit = 0;

	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
//		clog << "nichtvergebenePositionen == " << nichtvergebenePositionen << endl;

		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		PositionSchwimmerPairVector eingesetzteSchwimmer; // eigentlich nur "testweise eingesetzte Schwimmer"!
		for (unsigned pos = 0; pos < disziplinen.size(); pos++)
			if (!vergebenePositionen[pos])
			{
				const int disziplin = disziplinen[pos];
				outputSchwimmerZeiten<SchwimmerList::const_iterator>(clog,
					schwimmerSortiert[disziplin].begin(),
					schwimmerSortiert[disziplin].end(),
					disziplin);
				Schwimmer* const schw = *schwimmerSortiert[disziplin].begin();
				eingesetzteSchwimmer.push_back(PositionSchwimmerPair(pos, schw));
				result[pos] = schw;
			}

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		PositionSchwimmerPair* mostWanted = findMostWanted(eingesetzteSchwimmer);

		// Diesen Schwimmer festsetzen fuer seine Position
		const int position    = mostWanted->first;
		Schwimmer* const schw = mostWanted->second;
		const int disziplin   = disziplinen[position];

		sort(eingesetzteSchwimmer.begin(), eingesetzteSchwimmer.end(), NormAbstandComparer(*this));
		outputEingesetzteSchwimmer(clog, eingesetzteSchwimmer);
		clog << "Groesster Abstand: " << schw->kuerzel << " auf Position " << position << endl;
//		outputSchwimmerAbstand(clog, abstaende[disziplin], disziplin);

		nichtvergebenePositionen--;
		vergebenePositionen[position] = true;
		removeFromAvailable(schw, availableSchwimmer);
		ensureMixedBedingung(*schw, neededGeschlecht, availableSchwimmer);
		gesamtzeit += schw->zeiten[disziplin];

//		outputSchwimmerAbstand(clog, abstaende[disziplin], disziplin);

	}
}


ostream& LagenstaffelComputer::outputSchwimmerAbstand(ostream& os, const SchwimmerAbstandMap& map, int disziplin)
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

ostream& LagenstaffelComputer::outputEingesetzteSchwimmer(ostream& os, const PositionSchwimmerPairVector& vec)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten/Abstand/Position/Disziplin, Sortiert nach Abstand" << endl;
	for (PositionSchwimmerPairVector::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		Schwimmer& schw = *it->second;
		int position  = it->first;
		int disziplin = disziplinen[position];

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
