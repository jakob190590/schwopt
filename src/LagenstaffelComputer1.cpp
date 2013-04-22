/*
 * LagenstaffelComputer1.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <utility>

#include "LagenstaffelComputer1.h"
#include "Zeit.h"

using namespace std;

const int LagenstaffelComputer1::DISZIPLINEN_IN_STAFFEL[] = { Disziplin::RUECK_50, Disziplin::BRUST_50, Disziplin::SCHM_50, Disziplin::FREI_50 };

LagenstaffelComputer1::LagenstaffelComputer1(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{
}

void LagenstaffelComputer1::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Schwimmer, die noch nicht feststehen in der Staffel
	int unlockedPositionen = ANZAHL_DISZIPLINEN_IN_STAFFEL;
	// Positionen und Schwimmer, die schon feststehen
	bool locked[ANZAHL_DISZIPLINEN_IN_STAFFEL] = { false, false, false, false };

	// hier geht's los!
	while (unlockedPositionen > 0)
	{
		// ueberall wo noch unlocked ist, besten einsetzen
		for (int i = 0; i < ANZAHL_DISZIPLINEN_IN_STAFFEL; i++)
			if (!locked[i])
			{
				result[i] = schwimmerSortiert[DISZIPLINEN_IN_STAFFEL[i]][0]; // TODO statt 0 nen index aus nem array nehmen, der auf den schwimmer zeigt, ab dem noch verfuegbare ist
			}

		// Alle UNLOCKED Schwimmer nach Abstand ABSTEIGEND sortiert in set einfuegen
		set<pair<int, Schwimmer*> > staffelSchwimmerSortiertNachAbstand;
		for (int i = 0; i < ANZAHL_DISZIPLINEN_IN_STAFFEL; i++)
			if (!locked[i])
				staffelSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, result[i]));

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer locken, wenn noch nicht locked!
		for (set<pair<int, Schwimmer*> >::const_iterator it = staffelSchwimmerSortiertNachAbstand.begin();
				it != staffelSchwimmerSortiertNachAbstand.end(); ++it)
			if (!locked[it->first])
			{
				// Diesen Schwimmer festlegen fuer seine Position
				locked[it->first] = true;
				unlockedPositionen--;
			}
			else
			{

			}


	}

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
