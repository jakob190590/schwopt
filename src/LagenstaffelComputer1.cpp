/*
 * LagenstaffelComputer1.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>

#include "LagenstaffelComputer1.h"
#include "Zeit.h"

using namespace std;

const int LagenstaffelComputer1::DISZIPLINEN_IN_STAFFEL[] = { Disziplin::RUECK_50, Disziplin::BRUST_50, Disziplin::SCHM_50, Disziplin::FREI_50 };

LagenstaffelComputer1::LagenstaffelComputer1(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
{
}

/*
 * Algorithm:
 *
 * Solange freie Positionen in Staffel
 *   Freie Positionen mit besten freien Schwimmern besetzen
 *   Alle eben eingesetzten Schwimmer nach Abstand absteigend sortieren
 *   Diese sortierte Liste durchgehen:
 *     Wenn Schwimmer noch frei:
 *       Diesen Schwimmer festsetzen
 *     sonst:
 *       break;
 *
 * Kleine Optimierung (eigentlich unnoetig):
 * Bei innerer Schleife, ersten Schwimmer immer GLEICH festsetzen, ohne if
 *
 */
void LagenstaffelComputer1::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht feststehen
	int unlockedPositionen = ANZAHL_POSITIONEN_IN_STAFFEL;
	// Positionen, die schon feststehen
	bool locked[ANZAHL_POSITIONEN_IN_STAFFEL] = { false, false, false, false };
	// Noch verfuegbare Schwimmer
	SchwimmerSet availableSchwimmer;
	availableSchwimmer.insert(schwimmer.begin(), schwimmer.end());
	// Schwimmerlisten sortiert
	SchwimmerList sortedSchwimmer[Disziplin::ANZAHL];
	for (int i = 0; i < Disziplin::ANZAHL; i++)
		sortedSchwimmer[i] = schwimmerSortiert[i];


	// hier geht's los!
	while (unlockedPositionen > 0)
	{
		// ueberall wo noch unlocked ist, besten einsetzen
		for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
			if (!locked[i])
			{
				result[i] = *sortedSchwimmer[DISZIPLINEN_IN_STAFFEL[i]].begin();
			}

		// Alle UNLOCKED Schwimmer nach Abstand ABSTEIGEND sortiert in set einfuegen
		set<pair<int, Schwimmer*> > staffelSchwimmerSortiertNachAbstand;
		for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
			if (!locked[i])
				staffelSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, result[i]));

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer locken, wenn noch nicht locked!
		for (set<pair<int, Schwimmer*> >::const_iterator it = staffelSchwimmerSortiertNachAbstand.begin();
				it != staffelSchwimmerSortiertNachAbstand.end(); ++it)
			if (!locked[it->first]) // beim 1. mal immer true, danach kann's auch false sein!
			{
				// Diesen Schwimmer festlegen fuer seine Position
				unlockedPositionen--;
				locked[it->first] = true;
				availableSchwimmer.erase(it->second);
				for (int i = 0; i < Disziplin::ANZAHL; i++)
					sortedSchwimmer[i].remove(it->second);
			}
			else
				break;


	}

	// Zum Schluss Gesamtzeit berechnen:
	gesamtzeit = 0;
	for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
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
	for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
	{
		int diszi = DISZIPLINEN_IN_STAFFEL[i];
		os << "Disziplin " << diszi << ": " << result[i]->kuerzel << "  " << Zeit::convertToString(result[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl;
	return os;
}
