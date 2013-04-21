/*
 * LagenstaffelComputer1.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include "LagenstaffelComputer1.h"
#include "Zeit.h"

const int LagenstaffelComputer1::ANZAHL_DISZIPLINEN_IN_STAFFEL = 4;
const int LagenstaffelComputer1::DISZIPLINEN_IN_STAFFEL[] = { RUECK_50, BRUST_50, SCHM_50, FREI_50 };

LagenstaffelComputer1::LagenstaffelComputer1(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{

}

void LagenstaffelComputer1::compute()
{
	// erst mal ueberall den besten einsetzen
	for (int i = 0; i < ANZAHL_DISZIPLINEN_IN_STAFFEL; i++)
	{
		result.push_back(schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]][0]);
	}

	// hier geht's los!


	// Zum Schluss Gesamtzeit berechnen:
	gesamtzeit = 0;
	for (int i = 0; i < ANZAHL_DISZIPLINEN_IN_STAFFEL; i++)
	{
		gesamtzeit += result[i]->zeiten[DISZIPLINEN_IN_STAFFEL[i]];
	}
}

unsigned LagenstaffelComputer1::getTime()
{
	return gesamtzeit;
}

SchwimmerVector LagenstaffelComputer1::getResult()
{
	return result;
}

ostream& LagenstaffelComputer1::outputResult(ostream& os)
{
	for (int i = 0; i < ANZAHL_DISZIPLINEN_IN_STAFFEL; i++)
	{
		int diszi = DISZIPLINEN_IN_STAFFEL[i];
		os << "Disziplin " << diszi << ": " << result[i]->kuerzel << "  " << Zeit::convertToString(result[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl;
	return os;
}
