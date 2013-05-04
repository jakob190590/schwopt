/*
 * SchwoptAlgo.cpp
 *
 *  Created on: 01.05.2013
 *      Author: jakob190590
 */

#include "SchwoptAlgo.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../Zeit.h"

using namespace std;

SchwoptAlgo::NormAbstandComparer::NormAbstandComparer(SchwoptAlgo& computer) :
		computer(computer)
{
}

bool SchwoptAlgo::NormAbstandComparer::operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
{
	// Abstand in Diziplin, die in der Staffel auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
	return false;//computer.abstaende[DISZIPLINEN_IN_STAFFEL[p1.first]][p1.second] > computer.abstaende[DISZIPLINEN_IN_STAFFEL[p2.first]][p2.second];
}

void SchwoptAlgo::entfAusSchwimmerSortiertUndAbstaende(Schwimmer* schw)
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


ostream& SchwoptAlgo::outputSchwimmerAbstand(ostream& os, const SchwimmerAbstandMap& map, int disziplin)
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

ostream& SchwoptAlgo::outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet& set)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten/Abstand/Position/Disziplin, Sortiert nach Abstand" << endl;
	for (SortedPositionSchwimmerSet::const_iterator it = set.begin(); it != set.end(); ++it)
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

void gscheideDebugAusgabe(ostream& os,
		const SchwoptAlgo::DisziplinenAufPositionen& disziplinen,
		const SchwimmerVector schwimmerSortiert[Disziplin::ANZAHL],
		const SchwoptAlgo::PositionSchwimmerPairVector& vec,
		const SchwoptAlgo::SchwimmerAbstandMap abstaende[Disziplin::ANZAHL],
		unsigned anzahlNaechstbester, bool showDisziplin)
{
	// Ueberschriftenzeile
	os << "Pos. ";
	if (showDisziplin)
		os << "Disziplin ";
	os << "Schw. Zeit ";
	for (unsigned i = 2; i < anzahlNaechstbester + 2; i++)
		os << i << ". Schwimmer ";
	os << endl;

	// Daten
	for (unsigned i = 0; i < vec.size(); i++)
	{
		const int position  = vec[i].first;
		const int disziplin = disziplinen[position];
		Schwimmer* const schwimmer = vec[i].second;

		os << position;
		if (showDisziplin)
			os << Disziplin::convertToString(disziplin, true, true, "m");
		os << schwimmer->kuerzel << schwimmer->zeiten[disziplin] << "-" << abstaende[disziplin].find(schwimmer)->second << "-";

		const SchwimmerVector& naechstbesteSchwimmer = schwimmerSortiert[disziplin];
		SchwimmerVector::const_iterator next = naechstbesteSchwimmer.begin();
		for (unsigned i = 0; i < anzahlNaechstbester && next != naechstbesteSchwimmer.end(); i++)
		{
			Schwimmer* const nextSchwimmer = *next;
			os << nextSchwimmer->zeiten[disziplin] << "-" << abstaende[disziplin].find(nextSchwimmer)->second << "-";
		}
		os << endl;
	}
}
