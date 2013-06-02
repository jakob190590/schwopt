
#include <map>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "Einzelstarts.h"
#include "../Zeit.h"
#include "../Debugging.h"

using namespace std;

Einzelstarts::Einzelstarts(const SchwimmerList& schwimmer) :
		SchwoptComputer(schwimmer, +ANZAHL_POSITIONEN)
{
	disziplinenAufPositionen.reserve(ANZAHL_POSITIONEN);
	// Einzelstarts (4 x 50 m Lagen + 4 x 100 m Lagen)
	disziplinenAufPositionen.push_back(+Disziplin::BRUST_50);
	disziplinenAufPositionen.push_back(+Disziplin::RUECK_50);
	disziplinenAufPositionen.push_back(+Disziplin::SCHM_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	disziplinenAufPositionen.push_back(+Disziplin::BRUST_100);
	disziplinenAufPositionen.push_back(+Disziplin::RUECK_100);
	disziplinenAufPositionen.push_back(+Disziplin::SCHM_100);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_100);
}

void Einzelstarts::outputResult(ostream& os) const
{
	os << "Einzelstarts (Lage)" << endl;
	for (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{
		int diszi = disziplinenAufPositionen[i];
		os << setiosflags(ios::left) << setw(15) << Disziplin::convertToString(diszi, true, false);
		os << setiosflags(ios::right) << setw(8) << Disziplin::convertToString(diszi, false, true, "m") << resetiosflags(ios::right) << " ";
		os << getResult()[i]->kuerzel << "  " << Zeit::convertToString(ergebnis[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
}
