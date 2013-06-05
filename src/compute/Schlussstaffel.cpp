
#include <set>
#include <cassert>
#include <iomanip>

#include "Schlussstaffel.h"
#include "../Zeit.h"

using namespace std;

Schlussstaffel::Schlussstaffel(const SchwimmerList& schwimmer) :
		SchwoptComputer(schwimmer, +ANZAHL_POSITIONEN)
{
}

void Schlussstaffel::outputResult(ostream& os) const
{
	string diszi = Disziplin::convertToString(DISZIPLIN);
	os << "Kraulstaffel (" << ANZAHL_POSITIONEN << " x " << diszi << ")" << endl; // TODO freistil oder kraul?
	for (int i = 0; i < ANZAHL_POSITIONEN; i++)
	{
		os << (i + 1) << ". Schwimmer   " << getResult()[i]->kuerzel << "  " << Zeit::convertToString(ergebnis[i]->zeiten[DISZIPLIN]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
}
