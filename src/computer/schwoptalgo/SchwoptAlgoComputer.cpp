/*
 * SchwoptAlgoComputer.cpp
 *
 *  Created on: 01.05.2013
 *      Author: jakob190590
 */

#include "SchwoptAlgoComputer.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>

#include "../../Zeit.h"

using namespace std;

SchwoptAlgoComputer::NormAbstandComparer::NormAbstandComparer(SchwoptAlgoComputer& computer) :
		computer(computer)
{
}

bool SchwoptAlgoComputer::NormAbstandComparer::operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
{
	// Abstand in Diziplin, die auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
	return computer.abstaendeInDisziplinen[computer.disziplinenAufPositionen[p1.first]][p1.second] > computer.abstaendeInDisziplinen[computer.disziplinenAufPositionen[p2.first]][p2.second];
}

void SchwoptAlgoComputer::removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer,
		SchwimmerListVector& schwimmerSortiert,
		vector<SchwoptAlgoComputer::SchwimmerAbstandMap>& abstaendeInDisziplinen)
{
	availableSchwimmer.erase(schw);

	// Eigentlich reicht's fuer Disziplinen der Staffel (Vorsicht: Eine Disziplin kann in Staffel ja mehrfach vorkommen)
	for (int disziplin = 0; disziplin < Disziplin::ANZAHL; disziplin++)
	{
		SchwimmerList& schwimmerzeitList = schwimmerSortiert[disziplin]; // list, sorted by zeiten in disziplin, with Schwimmer*
		SchwimmerAbstandMap& abstandsMap = abstaendeInDisziplinen[disziplin]; // map, sorted by abstand der zeiten in disziplin, Schwimmer* => unsigned

		abstandsMap.erase(schw);

		// Abstaende in abstandsMap evtl. korrigieren!
		SchwimmerList::iterator it = find(schwimmerzeitList.begin(), schwimmerzeitList.end(), schw);
		assert(it != schwimmerzeitList.end()); // schw muss in der list sein

		if (it == schwimmerzeitList.begin())
		{
			// nothing to do (except remove from list)
			schwimmerzeitList.remove(schw);
			continue;
		}

		// Standardfall: Abstand neu berechnen
		schwimmerzeitList.erase(it--);

		SchwimmerList::iterator next = it;
		next++; // next soll auf Naechstschlechteren zeigen

		unsigned itZeit   = (*it)->zeiten[disziplin];
		unsigned nextZeit = Zeit::MAX_UNSIGNED_VALUE; // falls it der letzte Schwimmer ist...
		if (next != schwimmerzeitList.end())
			nextZeit = (*next)->zeiten[disziplin];

		abstandsMap[*it] = nextZeit - itZeit;
	}
}

vector<SchwoptAlgoComputer::SchwimmerAbstandMap> SchwoptAlgoComputer::createAbstandsMap(const SchwimmerListVector schwimmerSortiert) const
{
	SchwimmerAbstandMapVector result(Disziplin::ANZAHL);

    // Abstand zum Naechstschlechteren berechnen
    for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		const SchwimmerList& schwSorted = schwimmerSortiert[i];

		// Abstaende zw. Schwimmern fuer aktuelle Disziplin berechnen
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
			result[i][*it] = nextZeit - itZeit; // Naechstschlechterer - Aktueller
		}
	}

    return result;
}

SchwoptAlgoComputer::SchwoptAlgoComputer(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer), abstaendeInDisziplinen(Disziplin::ANZAHL)
{
    abstaendeInDisziplinen = createAbstandsMap(schwimmerSortiert);
    // Ergebnis initialisieren
	gesamtzeit = 0;
}

void SchwoptAlgoComputer::outputResult(ostream& os) const
{
	for (unsigned pos = 0; pos < disziplinenAufPositionen.size(); pos++)
	{
		int diszi = disziplinenAufPositionen[pos];
		os << setiosflags(ios::left) << setw(15) << Disziplin::convertToString(diszi, true, false);
		os << setiosflags(ios::right) << setw(8) << Disziplin::convertToString(diszi, false, true, "m") << resetiosflags(ios::right) << " ";
		os << getResult()[pos]->kuerzel << "  " << Zeit::convertToString(ergebnis[pos]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(gesamtzeit) << endl << endl;
}


void SchwoptAlgoComputer::outputSchwimmerAbstand(ostream& os, const SchwimmerAbstandMap& map, int disziplin) const
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
}

void SchwoptAlgoComputer::outputEingesetzteSchwimmer(ostream& os, const SortedPositionSchwimmerSet& set) const
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten/Abstand/Position/Disziplin, Sortiert nach Abstand" << endl;
	for (SortedPositionSchwimmerSet::const_iterator it = set.begin(); it != set.end(); ++it)
	{
		Schwimmer& schw = *it->second;
		int position  = it->first;
		int disziplin = disziplinenAufPositionen[position];

		os << setiosflags(ios::left);
		os << setw(16) << schw.nachname << setw(10) << schw.vorname << setw(3) << schw.kuerzel;
		os << setiosflags(ios::right);
		os << setw(14) << Zeit::convertToString(schw.zeiten[disziplin]);
		os << setw(14) << Zeit::convertToString(abstaendeInDisziplinen[disziplin].find(&schw)->second);
		os << setw(5) << position << ": ";
		os << resetiosflags(ios::right);
		os << Disziplin::convertToString(disziplin) << endl;
	}
}

void SchwoptAlgoComputer::gscheideDebugAusgabe(ostream& os,
		const SchwoptAlgoComputer::DisziplinenAufPositionen& disziplinen,
		const SchwimmerListVector& schwimmerSortiert,
		const SchwoptAlgoComputer::PositionSchwimmerPairList& list,
		const vector<SchwoptAlgoComputer::SchwimmerAbstandMap>& abstaende,
		unsigned anzahlNaechstbester, bool showDisziplin) const
{
	const int COL_WIDTH_POSITION = 5;
	const int COL_WIDTH_DISZIPLIN = 10;
	const int COL_WIDTH_SCHWIMMER = 6;
	const int COL_WIDTH_ZEIT = 10;

	os << setiosflags(ios::left); // default

	// Ueberschriftenzeile
	os     << setw(COL_WIDTH_POSITION)  << "Pos.";
	if (showDisziplin)
		os << setw(COL_WIDTH_DISZIPLIN) << "Disziplin";
	os     << setw(COL_WIDTH_SCHWIMMER) << "Schw.";
	os     << setw(COL_WIDTH_ZEIT)      << "Zeit";

//	for (unsigned i = 2; i < anzahlNaechstbester + 2; i++) // so detailiert muessen die header nicht sein
//		os << i << ". Schwimmer ";
	if (anzahlNaechstbester > 0)
		os << "Naechstbeste Schwimmer";
	os << endl;

	// Daten
	for (PositionSchwimmerPairList::const_iterator it = list.begin();
			it != list.end(); ++it)
	{
		const int position  = it->first;
		const int disziplin = disziplinen[position];
		Schwimmer* const schwimmer = it->second;

		os     << setw(COL_WIDTH_POSITION)  << position;
		if (showDisziplin)
			os << setw(COL_WIDTH_DISZIPLIN) << Disziplin::convertToString(disziplin, true, true, "m").substr(0, COL_WIDTH_DISZIPLIN - 2);
		os     << setw(COL_WIDTH_SCHWIMMER) << schwimmer->kuerzel;
		os     << setw(COL_WIDTH_ZEIT / 2 - 1) << schwimmer->zeiten[disziplin] / 100;
		os     << "-" << setw(2) << setiosflags(ios::right) << abstaende[disziplin].find(schwimmer)->second / 100 << resetiosflags(ios::right) << "-  ";

		const SchwimmerList& naechstbesteSchwimmer = schwimmerSortiert[disziplin];
		SchwimmerList::const_iterator next = naechstbesteSchwimmer.begin();
		next++; // der erste Schwimmer ist ja schon ausgegeben
		for (unsigned i = 0; i < anzahlNaechstbester && next != naechstbesteSchwimmer.end(); i++, next++)
		{
			Schwimmer* const nextSchwimmer = *next;
			os << nextSchwimmer->kuerzel << ' ' << nextSchwimmer->zeiten[disziplin] / 100;
			os << " -" << setw(2) << setiosflags(ios::right) << abstaende[disziplin].find(nextSchwimmer)->second / 100 << resetiosflags(ios::right) << "-  ";
		}
		os << endl;
	}
}
