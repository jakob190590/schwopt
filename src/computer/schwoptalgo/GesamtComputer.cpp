/*
 * GesamtComputer.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <bitset>
#include <cassert>
#include <iomanip>

#include "GesamtComputer.h"
#include "../../Zeit.h"

using namespace std;

GesamtComputer::GesamtComputer(const SchwimmerVector& schwimmer) :
		SchwoptAlgoComputer(schwimmer)
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

	// Ergebnis initialisieren
	result.resize(ANZAHL_POSITIONEN);
}

int GesamtComputer::convertPositionToBlock(int position)
{
	if (position < LagenstaffelComputer::ANZAHL_POSITIONEN)
		return BLOCK_LAGENSTAFFEL;
	if (position < LagenstaffelComputer::ANZAHL_POSITIONEN + ANZAHL_POSITIONEN_KRAULSTAFFEL)
		return BLOCK_KRAULSTAFFEL;
	if (position < LagenstaffelComputer::ANZAHL_POSITIONEN + ANZAHL_POSITIONEN_KRAULSTAFFEL + EinzelstartsComputer::ANZAHL_POSITIONEN / 2)
		return BLOCK_EINZELSTARTS_50;
	return BLOCK_EINZELSTARTS_100;
}

void GesamtComputer::compute()
{

}

void GesamtComputer::outputResult(ostream& os) const
{
	os << "Gesamt OMP" << endl;
	SchwoptAlgoComputer::outputResult(os);
}
