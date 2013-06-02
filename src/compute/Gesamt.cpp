
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

Gesamt::Gesamt(const SchwimmerList& schwimmer) :
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
	const int WIDTH_SCHW = 5;
	const int WIDTH_ZEIT = 7;
	const string underline(36, '-');
	const string divider  (55, '-');

	os << "Gesamtaufstellung fuer OMP-Wettkampf" << endl
	   << "====================================" << endl;

	os << "Lagenstaffel (4 x 50 m Lagen)" << endl
	   << underline << endl;

	unsigned sum = 0;
	for (int pos = 0; pos < ANZAHL_POSITIONEN; pos++)
	{
		Schwimmer* schw = ergebnis[pos];
		int diszi = disziplinenAufPositionen[pos];

		os << setiosflags(ios::right) << setw(6) << Disziplin::convertToString(diszi, false, true, " m ") << resetiosflags(ios::right) << " ";
		os << setiosflags(ios::left) << setw(17) << Disziplin::convertToString(diszi, true, false);
		if (schw != NULL)
		{
			os << setw(WIDTH_SCHW) << schw->kuerzel << Zeit::convertToString(schw->zeiten[diszi]);
			sum += schw->zeiten[diszi];
		}
		else
			os << setw(WIDTH_SCHW + WIDTH_ZEIT) << "N/A";

		switch (pos)
		{
		case Lagenstaffel::ANZAHL_POSITIONEN - 1:
			outputZwischenergebnis(os, sum, divider);
			os << "Kraulstaffel (6 x 50 m Freistil)" << endl
			   << underline << endl;
			sum = 0;
			break;
		case Lagenstaffel::ANZAHL_POSITIONEN +
		     Kraulstaffel::ANZAHL_POSITIONEN - 1:
			outputZwischenergebnis(os, sum, divider);
			os << "Einzelstarts (4 x 50 m + 4 x 100 m Lagen)" << endl
			   << underline << endl;
			sum = 0;
			break;
		case Lagenstaffel::ANZAHL_POSITIONEN +
		     Kraulstaffel::ANZAHL_POSITIONEN +
		     Einzelstarts::ANZAHL_POSITIONEN - 1:
			outputZwischenergebnis(os, sum, divider);
			sum = 0;
			break;
		default:
			os << endl;
			break;
		}

	}
	os << "Gesamtzeit: " << Zeit::convertToString(gesamtzeit) << endl;
}

void Gesamt::outputZwischenergebnis(ostream& os, const unsigned& zeit, const string& divider) const
{
	os << "      Zeit: " << Zeit::convertToString(zeit) << endl
	   << divider << endl;
}
