
#include <map>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "Einzelstarts.h"
#include "../Zeit.h"
#include "../Debugging.h"

using namespace std;

Einzelstarts::Einzelstarts(const SchwimmerList& schwimmer) :
		SchwoptComputer(schwimmer, +ANZAHL_POSITIONEN), positionDisziplinTable(+ANZAHL_POSITIONEN)
{
	// Einzelstarts (4 x 50 m Lagen + 4 x 100 m Lagen)
	positionDisziplinTable[0] = Disziplin::BRUST_50;
	positionDisziplinTable[1] = Disziplin::RUECK_50;
	positionDisziplinTable[2] = Disziplin::SCHM_50;
	positionDisziplinTable[3] = Disziplin::FREI_50;
	positionDisziplinTable[4] = Disziplin::BRUST_100;
	positionDisziplinTable[5] = Disziplin::RUECK_100;
	positionDisziplinTable[6] = Disziplin::SCHM_100;
	positionDisziplinTable[7] = Disziplin::FREI_100;
}

void Einzelstarts::outputResult(ostream& os) const
{
	os << "Einzelstarts (Lage)" << endl;
	for (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{
		int diszi = positionDisziplinTable[i];
		os << setiosflags(ios::left) << setw(15) << Disziplin::convertToString(diszi, true, false);
		os << setiosflags(ios::right) << setw(8) << Disziplin::convertToString(diszi, false, true, "m") << resetiosflags(ios::right) << " ";
		os << getResult()[i]->kuerzel << "  " << Zeit::convertToString(ergebnis[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
}
