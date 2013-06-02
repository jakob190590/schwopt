
#include <iomanip>

#include "Lagenstaffel.h"
#include "../Zeit.h"

using namespace std;

const int Lagenstaffel::DISZIPLINEN_IN_STAFFEL[] = { Disziplin::RUECK_50, Disziplin::BRUST_50, Disziplin::SCHM_50, Disziplin::FREI_50 };

Lagenstaffel::Lagenstaffel(const SchwimmerList& schwimmer) :
		SchwoptComputer(schwimmer, +ANZAHL_POSITIONEN)
{
}


void Lagenstaffel::outputResult(ostream& os) const
{
	os << "Lagenstaffel (4 x 50 m Lagen)" << endl;
	for (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{
		int diszi = DISZIPLINEN_IN_STAFFEL[i];
		os << setiosflags(ios::left) << setw(23) << Disziplin::convertToString(diszi) << " " << getResult()[i]->kuerzel << "  " << Zeit::convertToString(ergebnis[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
}
