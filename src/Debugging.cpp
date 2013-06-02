/*
 * Debugging.cpp
 *
 *  Created on: 29.04.2013
 *      Author: jakob190590
 */

#include <iomanip>

#include "Debugging.h"

void gscheideDebugAusgabe(ostream& os,
		const PositionDisziplinTable& disziplinen,
		const SchwimmerListVector& schwimmerSortiert,
		const PositionSchwimmerPairList& list,
		const vector<SchwimmerAbstandMap>& abstaende,
		unsigned anzahlNaechstbester, bool showDisziplin)
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
