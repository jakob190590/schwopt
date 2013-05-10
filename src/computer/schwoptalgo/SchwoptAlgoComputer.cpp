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
	return computer.abstaende[computer.disziplinen[p1.first]][p1.second] > computer.abstaende[computer.disziplinen[p2.first]][p2.second];
}

void SchwoptAlgoComputer::removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer)
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
		schwimmerzeitList.remove(schw); // TODO warum nicht erase(it--); waere doch schneller, weil schw nicht gesucht werden muss

		SchwimmerList::iterator next = it; // next soll auf Naechstschlechteren zeigen
		next++;

		unsigned itZeit   = (*it)->zeiten[disziplin];
		unsigned nextZeit = Zeit::MAX_UNSIGNED_VALUE; // falls it der letzte Schwimmer ist...
		if (next != schwimmerzeitList.end())
			nextZeit = (*next)->zeiten[disziplin];

		abstandsMap[*it] = nextZeit - itZeit;
	}
}

SchwoptAlgoComputer::SchwoptAlgoComputer(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{

}


ostream& SchwoptAlgoComputer::outputSchwimmerAbstand(ostream& os, const SchwimmerAbstandMap& map, int disziplin) const
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

ostream& SchwoptAlgoComputer::outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet& set) const
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
		os << setw(14) << Zeit::convertToString(abstaende[disziplin].find(&schw)->second);
		os << setw(5) << position << ": ";
		os << resetiosflags(ios::right);
		os << Disziplin::convertToString(disziplin) << endl;
	}
	return os;
}

void SchwoptAlgoComputer::gscheideDebugAusgabe(ostream& os,
		const SchwoptAlgoComputer::DisziplinenAufPositionen& disziplinen,
		const SchwimmerVector schwimmerSortiert[Disziplin::ANZAHL],
		const SchwoptAlgoComputer::PositionSchwimmerPairVector& vec,
		const SchwoptAlgoComputer::SchwimmerAbstandMap abstaende[Disziplin::ANZAHL],
		unsigned anzahlNaechstbester, bool showDisziplin) const
{

	// Ueberschriftenzeile
	os << "Pos. ";
	if (showDisziplin)
		os << "Disziplin ";
	os << "Schw. Zeit       ";
//	for (unsigned i = 2; i < anzahlNaechstbester + 2; i++)
//		os << i << ". Schwimmer ";
	if (anzahlNaechstbester > 0)
		os << "Naechstbessere Schwimmer";
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
		os << schwimmer->kuerzel << schwimmer->zeiten[disziplin];
		os << "-" << abstaende[disziplin].find(schwimmer)->second << "-";

		const SchwimmerVector& naechstbesteSchwimmer = schwimmerSortiert[disziplin];
		SchwimmerVector::const_iterator next = naechstbesteSchwimmer.begin();
		next++; // der erste Schwimmer ist ja schon ausgegeben
		for (unsigned i = 0; i < anzahlNaechstbester && next != naechstbesteSchwimmer.end(); i++)
		{
			Schwimmer* const nextSchwimmer = *next;
			os << nextSchwimmer->zeiten[disziplin] << " (" << nextSchwimmer->kuerzel << ") ";
			os << "-" << abstaende[disziplin].find(nextSchwimmer)->second << "-";
		}
		os << endl;
	}
}
