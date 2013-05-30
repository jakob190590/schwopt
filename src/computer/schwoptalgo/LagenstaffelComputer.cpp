/*
 * LagenstaffelComputer.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "LagenstaffelComputer.h"
#include "../../Zeit.h"
#include "../../Debugging.h"

using namespace std;

LagenstaffelComputer::PositionSchwimmerPair* LagenstaffelComputer::findMostWanted(PositionSchwimmerPairVector& vec)
{
	PositionSchwimmerPair* result = NULL;
	// Abstand in Diziplin auf der angegebenen Position, fuer den Schwimmer, der fuer diese Position vorgesehen ist
	unsigned greatestAbstand = 0;

	for (PositionSchwimmerPairVector::iterator it = vec.begin();
			it != vec.end(); ++it)
	{
		unsigned abstand = abstaendeInDisziplinen[disziplinenAufPositionen[it->first]][it->second];
		if (abstand > greatestAbstand)
		{
			greatestAbstand = abstand;
			result = &*it;
		}
	}
	return result;
}

void LagenstaffelComputer::ensureMixedBedingung(Schwimmer& schw, int neededGeschlecht[2], SchwimmerSet& availableSchwimmer)
{
	// Zum sicherstellen der "Mixed"-Bedingung
	if (--neededGeschlecht[schw.geschlecht] > 0) // zaehler nach dekrement noch groesser 0
		return; // nichts weiter zu tun

	// Alle Schwimmer des gegebenen Geschlechts rausloeschen
	SchwimmerSet::iterator it, toBeErased; // nicht const_iterator, weil element geloescht wird -- wenn auch nicht direkt ueber diese iterator
	for (it = availableSchwimmer.begin(); it != availableSchwimmer.end(); ++it)
		if ((*it)->geschlecht == schw.geschlecht)
		{
			toBeErased = it;
			--it; // it eins zurueckfahren; hier ist er sicher waehrend vorige position geloescht wird
			removeFromAvailable(*toBeErased, availableSchwimmer, schwimmerSortiert, abstaendeInDisziplinen);
		}
}

LagenstaffelComputer::LagenstaffelComputer(const SchwimmerVector& schwimmer) :
		SchwoptAlgoComputer(schwimmer)
{
	disziplinenAufPositionen.reserve(4);
	// Lagenstaffel (4 x 50 m Lagen)
	disziplinenAufPositionen.push_back(+Disziplin::RUECK_50);
	disziplinenAufPositionen.push_back(+Disziplin::BRUST_50);
	disziplinenAufPositionen.push_back(+Disziplin::SCHM_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);

	// Ergebnis initialisieren
	result.resize(disziplinenAufPositionen.size());
}

void LagenstaffelComputer::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = disziplinenAufPositionen.size();
	// Positionen, die schon fest vergeben sind
	vector<bool> vergebenePositionen(disziplinenAufPositionen.size(), false);
	// Noch verfuegbare Schwimmer
	SchwimmerSet availableSchwimmer(schwimmer.begin(), schwimmer.end());
	// "Mixed"-Bedingungen: 2 Schwimmer, 2 Schwimmerinnen
	int neededGeschlecht[2] = { 2, 2 };

	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		PositionSchwimmerPairVector eingesetzteSchwimmer; // eigentlich nur "testweise eingesetzte Schwimmer"!
		for (unsigned pos = 0; pos < disziplinenAufPositionen.size(); pos++)
			if (!vergebenePositionen[pos])
			{
				const int disziplin = disziplinenAufPositionen[pos];
				Schwimmer* const schw = *schwimmerSortiert[disziplin].begin();
				eingesetzteSchwimmer.push_back(PositionSchwimmerPair(pos, schw));
				result[pos] = schw;
			}

		PositionSchwimmerPair* mostWanted = findMostWanted(eingesetzteSchwimmer);

		// Diesen Schwimmer festsetzen fuer seine Position
		const int position    = mostWanted->first;
		Schwimmer* const schw = mostWanted->second;
		const int disziplin   = disziplinenAufPositionen[position];

		sort(eingesetzteSchwimmer.begin(), eingesetzteSchwimmer.end(), NormAbstandComparer(*this)); // Sortierung nur fuer die Debug-Ausgabe
		gscheideDebugAusgabe(clog, disziplinenAufPositionen, schwimmerSortiert, eingesetzteSchwimmer, abstaendeInDisziplinen);

		nichtvergebenePositionen--;
		vergebenePositionen[position] = true;
		removeFromAvailable(schw, availableSchwimmer, schwimmerSortiert, abstaendeInDisziplinen);
		ensureMixedBedingung(*schw, neededGeschlecht, availableSchwimmer);
		gesamtzeit += schw->zeiten[disziplin];
	}
}

ostream& LagenstaffelComputer::outputResult(ostream& os) const
{
	os << "Lagenstaffel (4 x 50 m Lagen)" << endl;
	SchwoptAlgoComputer::outputResult(os);
	return os;
}
