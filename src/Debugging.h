
#ifndef DEBUGGING_H_
#define DEBUGGING_H_

#include <iostream>
#include <iterator>

#include "Zeit.h"
#include "Schwimmer.h"
#include "Disziplin.h"
#include "compute/SchwoptAlgo.h"

using namespace std;

template <typename Iterator> void outputSchwimmerZeiten(ostream& os,
		const Iterator& begin, const Iterator& end, int disziplin)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten, Sortiert nach Zeit, Disziplin: " << Disziplin::convertToString(disziplin) << endl;
	for (Iterator it = begin; it != end; ++it)
	{
		Schwimmer& schw = **it;
		os << setiosflags(ios::left);
		os << setw(16) << schw.nachname << setw(10) << schw.vorname << setw(3) << schw.kuerzel;
		os << setiosflags(ios::right);
		os << setw(14) << Zeit::convertToString(schw.zeiten[disziplin]) << endl;
		os << resetiosflags(ios::right);
	}
}

void gscheideDebugAusgabe(ostream& os,
		const PositionDisziplinTable& disziplinen,
		const SchwimmerListVector& schwimmerSortiert,
		const PositionSchwimmerPairList& list,
		const vector<SchwimmerAbstandMap>& abstaende,
		unsigned anzahlNaechstbester = 2, bool showDisziplin = true);

#endif /* DEBUGGING_H_ */
