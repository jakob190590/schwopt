
#include <iomanip>

#include "Lagenstaffel.h"
#include "../Zeit.h"

using namespace std;

Lagenstaffel::Lagenstaffel(const SchwimmerList& schwimmer) :
		SchwoptComputer(schwimmer, +ANZAHL_POSITIONEN), positionDisziplinTable(+ANZAHL_POSITIONEN)
{
	// Lagenstaffel (4 x 50 m Lagen)
	positionDisziplinTable[0] = Disziplin::RUECK_50;
	positionDisziplinTable[1] = Disziplin::BRUST_50;
	positionDisziplinTable[2] = Disziplin::SCHM_50;
	positionDisziplinTable[3] = Disziplin::FREI_50;
}


void Lagenstaffel::outputResult(ostream& os) const
{
	os << "Lagenstaffel (4 x 50 m Lagen)" << endl;
	for (int pos = 0; pos < ANZAHL_POSITIONEN; pos++)
	{
		int diszi = positionDisziplinTable[pos];
		os << setiosflags(ios::left) << setw(23) << Disziplin::convertToString(diszi) << " " << getResult()[pos]->kuerzel << "  " << Zeit::convertToString(ergebnis[pos]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
}
