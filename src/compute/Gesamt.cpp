
#include <set>
#include <bitset>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "Gesamt.h"
#include "Lagenstaffel.h"
#include "Kraulstaffel.h"
#include "Einzelstarts.h"
#include "../Zeit.h"

using namespace std;

Gesamt::Gesamt(const SchwimmerVector& schwimmer) :
		SchwoptComputer(schwimmer, +ANZAHL_POSITIONEN)
{
	disziplinenAufPositionen.reserve(ANZAHL_POSITIONEN);
	// Lagenstaffel (4 x 50 m Lagen)
	disziplinenAufPositionen.push_back(+Disziplin::RUECK_50); // + workaround (fu c++) http://stackoverflow.com/questions/272900
	disziplinenAufPositionen.push_back(+Disziplin::BRUST_50);
	disziplinenAufPositionen.push_back(+Disziplin::SCHM_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	// Kraulstaffel (8 x 50 m Kraul)
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);
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

int Gesamt::getBlock(int position)
{
	if (position < Lagenstaffel::ANZAHL_POSITIONEN)
		return LAGENSTAFFEL;
	if (position < Lagenstaffel::ANZAHL_POSITIONEN + Kraulstaffel::ANZAHL_POSITIONEN)
		return KRAULSTAFFEL;
	if (position < Lagenstaffel::ANZAHL_POSITIONEN + Kraulstaffel::ANZAHL_POSITIONEN + Einzelstarts::ANZAHL_POSITIONEN / 2)
		return EINZELSTARTS_50;
	return EINZELSTARTS_100;
}

void Gesamt::outputResult(ostream& os) const
{
	const string hr = "------------------------------------------------";
	os << "Gesamtaufstellung fuer OMP-Wettkampf" << endl;
	os << "====================================" << endl;

	for (int pos = 0; pos < ANZAHL_POSITIONEN; pos++)
	{
		switch (pos)
		{
		case 0:
		case 4: // TODO konstanten verwenden
		case 10:
			os << hr << endl;
			break;
		}
		int diszi = disziplinenAufPositionen[pos];
		os << setiosflags(ios::left) << setw(15) << Disziplin::convertToString(diszi, true, false);
		os << setiosflags(ios::right) << setw(8) << Disziplin::convertToString(diszi, false, true, "m") << resetiosflags(ios::right) << " ";
		Schwimmer* schw = ergebnis[pos];
		if (schw != NULL)
			os << schw->kuerzel << "  " << Zeit::convertToString(schw->zeiten[diszi]) << endl;
		else
			os << "N/A" << endl;
	}
	os << hr << endl;
	os << "Gesamtzeit: " << Zeit::convertToString(gesamtzeit) << endl;
}
