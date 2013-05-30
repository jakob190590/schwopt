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

	SchwimmerSet::iterator it;
	Schwimmer::GeschlechtPredicate pred(schw.geschlecht);
	while ((it = find_if(availableSchwimmer.begin(), availableSchwimmer.end(), pred))
			!= availableSchwimmer.end())
		// hier wird it ungueltig, wegen "erase(it)", deshalb "while (find_if"
		removeFromAvailable(*it, availableSchwimmer);
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
//		clog << "nichtvergebenePositionen == " << nichtvergebenePositionen << endl;

		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		PositionSchwimmerPairVector eingesetzteSchwimmer; // eigentlich nur "testweise eingesetzte Schwimmer"!
		for (unsigned pos = 0; pos < disziplinenAufPositionen.size(); pos++)
			if (!vergebenePositionen[pos])
			{
				const int disziplin = disziplinenAufPositionen[pos];
//				outputSchwimmerZeiten<SchwimmerList::const_iterator>(clog,
//					schwimmerSortiert[disziplin].begin(),
//					schwimmerSortiert[disziplin].end(),
//					disziplin);
				Schwimmer* const schw = *schwimmerSortiert[disziplin].begin();
				eingesetzteSchwimmer.push_back(PositionSchwimmerPair(pos, schw));
				result[pos] = schw;
			}

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		PositionSchwimmerPair* mostWanted = findMostWanted(eingesetzteSchwimmer);

		// Diesen Schwimmer festsetzen fuer seine Position
		const int position    = mostWanted->first;
		Schwimmer* const schw = mostWanted->second;
		const int disziplin   = disziplinenAufPositionen[position];

		sort(eingesetzteSchwimmer.begin(), eingesetzteSchwimmer.end(), NormAbstandComparer(*this));
//		outputEingesetzteSchwimmer(clog, eingesetzteSchwimmer);
//		clog << "Groesster Abstand: " << schw->kuerzel << " auf Position " << position << endl;
//		outputSchwimmerAbstand(clog, abstaendeInDisziplinen[disziplin], disziplin);

		gscheideDebugAusgabe(clog, disziplinenAufPositionen, schwimmerSortiert, eingesetzteSchwimmer, abstaendeInDisziplinen);

		nichtvergebenePositionen--;
		vergebenePositionen[position] = true;
		removeFromAvailable(schw, availableSchwimmer);
		ensureMixedBedingung(*schw, neededGeschlecht, availableSchwimmer);
		gesamtzeit += schw->zeiten[disziplin];

//		outputSchwimmerAbstand(clog, abstaendeInDisziplinen[disziplin], disziplin);

	}
}

ostream& LagenstaffelComputer::outputResult(ostream& os)
{
	os << "Lagenstaffel (4 x 50 m Lagen)" << endl;
	SchwoptAlgoComputer::outputResult(os);
	return os;
}
