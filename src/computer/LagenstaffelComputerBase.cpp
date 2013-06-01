/*
 * LagenstaffelComputerBase.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <iomanip>

#include "LagenstaffelComputerBase.h"
#include "../Zeit.h"

using namespace std;

const int LagenstaffelComputerBase::DISZIPLINEN_IN_STAFFEL[] = { Disziplin::RUECK_50, Disziplin::BRUST_50, Disziplin::SCHM_50, Disziplin::FREI_50 };

// Optimale Lösung (durchprobieren!)
LagenstaffelComputerBase::LagenstaffelComputerBase(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{
}


void LagenstaffelComputerBase::outputResult(ostream& os) const
{
	os << "Lagenstaffel (4 x 50 m Lagen)" << endl;
	for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
	{
		int diszi = DISZIPLINEN_IN_STAFFEL[i];
		os << setiosflags(ios::left) << setw(23) << Disziplin::convertToString(diszi) << " " << getResult()[i]->kuerzel << "  " << Zeit::convertToString(ergebnis[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl << endl;
}
