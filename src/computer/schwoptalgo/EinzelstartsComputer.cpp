/*
 * EinzelstartsComputer.cpp
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#include <map>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "EinzelstartsComputer.h"
#include "../../Zeit.h"
#include "../../Debugging.h"

using namespace std;

EinzelstartsComputer::EinzelstartsComputer(const SchwimmerVector& schwimmer) :
		SchwoptAlgoComputer(schwimmer)
{
	disziplinenAufPositionen.reserve(8);
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
	result.resize(disziplinenAufPositionen.size());
}


void EinzelstartsComputer::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = disziplinenAufPositionen.size();
	// Positionen, die schon fest vergeben sind
	vector<bool> vergebenePositionen(disziplinenAufPositionen.size(), false);
	// Noch verfuegbare Schwimmer
	SchwimmerIntMap availableSchwimmer;
	for (SchwimmerVector::const_iterator it = schwimmer.begin();
			it != schwimmer.end(); ++it)
		availableSchwimmer[*it] = 3; // Jeder Schwimmer maximal 3 x

	// "Mixed"-Bedingungen: Je mind. 1 m und mind. 1 w bei 50 m und 100 m
//	map<int, int[2]> quoten;
//	quoten[ 50][0] = 1;
//	quoten[ 50][1] = 1;
//	quoten[100][0] = 1;
//	quoten[100][1] = 1;


	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
		clog << "nichtvergebenePositionen == " << nichtvergebenePositionen << endl;

		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		// Alle nicht-vergebenen Schwimmer nach Abstand absteigend sortiert in set einfuegen
		SortedPositionSchwimmerSet eingesetzteSchwimmerSortiertNachAbstand(NormAbstandComparer(*this));
		for (int i = 0; i < disziplinenAufPositionen.size(); i++)
			if (!vergebenePositionen[i])
			{
				outputSchwimmerZeiten(clog,
						schwimmerSortiert[disziplinenAufPositionen[i]].begin(),
						schwimmerSortiert[disziplinenAufPositionen[i]].end(),
						disziplinenAufPositionen[i]);
				Schwimmer* schw = *schwimmerSortiert[disziplinenAufPositionen[i]].begin();
				eingesetzteSchwimmerSortiertNachAbstand.insert(pair<int, Schwimmer*>(i, schw));
				result[i] = schw;
			}

		outputEingesetzteSchwimmer(clog, eingesetzteSchwimmerSortiertNachAbstand);

		// Nach Abstand absteigend sortierte Liste durchgehen und Schwimmer festsetzen, wenn noch nicht vergeben!
		SortedPositionSchwimmerSet::const_iterator it = eingesetzteSchwimmerSortiertNachAbstand.begin();

		const int position    = it->first;
		Schwimmer* const schw = it->second;
		int& timesAvailable = availableSchwimmer[schw];
		assert(timesAvailable > 0); // musss so sein, jetzt wo die schleife weg ist!

		// Diesen Schwimmer festsetzen fuer seine Position
		clog << "=========================================" << endl;
		clog << "Schwimmer festsetzen: " << schw->kuerzel << " bei " << Disziplin::convertToString(disziplinenAufPositionen[position]) << endl;
		outputSchwimmerAbstand(clog, abstaendeInDisziplinen[disziplinenAufPositionen[position]], disziplinenAufPositionen[position]);
		nichtvergebenePositionen--;
		vergebenePositionen[position] = true;
		timesAvailable--;
		if (timesAvailable == 0) // Schwimmer kann kein weiteres Mal eingesetzt werden
			;//removeFromAvailable(schw, availableSchwimmer);
		gesamtzeit += schw->zeiten[disziplinenAufPositionen[position]];
		outputSchwimmerAbstand(clog, abstaendeInDisziplinen[disziplinenAufPositionen[position]], disziplinenAufPositionen[position]);

	}
//	clog << "availableSchwimmer am Ende des Algos" << endl;
//	for (SchwimmerIntMap::const_iterator it = availableSchwimmer.begin();
//			it != availableSchwimmer.end(); ++it)
//		clog << it->first->kuerzel << " " << it->second << endl;
}

ostream& EinzelstartsComputer::outputResult(ostream& os) const
{
	os << "Einzelstarts (Lage)" << endl;
	SchwoptAlgoComputer::outputResult(os);
	return os;
}
