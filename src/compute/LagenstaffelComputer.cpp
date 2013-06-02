
#include <set>
#include <vector>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "LagenstaffelComputer.h"
#include "../Zeit.h"
#include "../Debugging.h"

using namespace std;

vector<LagenstaffelComputer::SchwimmerAbstandMap> LagenstaffelComputer::createAbstandsMap(const SchwimmerListVector schwimmerSortiert) const
{
	SchwimmerAbstandMapVector result(Disziplin::ANZAHL);

    // Abstand zum Naechstschlechteren berechnen
    for (int i = 0; i < Disziplin::ANZAHL; i++)
	{
		const SchwimmerList& schwSorted = schwimmerSortiert[i];

		// Abstaende zw. Schwimmern fuer aktuelle Disziplin berechnen
		SchwimmerList::const_iterator it, next;
		it = next = schwSorted.begin();
		next++; // Naechstschlechterer Schwimmer
		for (; it != schwSorted.end(); ++it)
		{
			unsigned itZeit = (*it)->zeiten[i];
			unsigned nextZeit;
			if (next == schwSorted.end())
				nextZeit = Zeit::MAX_UNSIGNED_VALUE;
			else
			{
				nextZeit = (*next)->zeiten[i];
				++next; // next iterator schon mal erhoehen
			}

			assert(nextZeit >= itZeit); // Fehlerhafte Sortierung oder schwerer Fehler im Algo
			result[i][*it] = nextZeit - itZeit; // Naechstschlechterer - Aktueller
		}
	}

    return result;
}

LagenstaffelComputer::PositionSchwimmerPair* LagenstaffelComputer::findMostWanted(PositionSchwimmerPairList& list)
{
	PositionSchwimmerPair* result = NULL;
	// Abstand in Diziplin auf der angegebenen Position, fuer den Schwimmer, der fuer diese Position vorgesehen ist
	unsigned greatestAbstand = 0;
	for (PositionSchwimmerPairList::iterator it = list.begin();
			it != list.end(); ++it)
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

void LagenstaffelComputer::removeFromAvailable(Schwimmer* schw,
		SchwimmerSet& availableSchwimmer,
		SchwimmerListVector& schwimmerSortiert,
		SchwimmerAbstandMapVector& abstaendeInDisziplinen)
{
	availableSchwimmer.erase(schw);

	// Eigentlich reicht's fuer Disziplinen der Staffel (Vorsicht: Eine Disziplin kann in Staffel ja mehrfach vorkommen)
	for (int disziplin = 0; disziplin < Disziplin::ANZAHL; disziplin++)
	{
		SchwimmerList& schwimmerzeitList = schwimmerSortiert[disziplin]; // list, sorted by zeiten in disziplin, with Schwimmer*
		SchwimmerAbstandMap& abstandsMap = abstaendeInDisziplinen[disziplin]; // map, sorted by abstand der zeiten in disziplin, Schwimmer* => unsigned

		abstandsMap.erase(schw);

		// Abstaende in abstandsMap evtl. korrigieren!
		SchwimmerList::iterator it = find(schwimmerzeitList.begin(), schwimmerzeitList.end(), schw);
		assert(it != schwimmerzeitList.end()); // schw muss in der list sein

		if (it == schwimmerzeitList.begin())
		{
			// nothing to do (except remove from list)
			schwimmerzeitList.remove(schw);
			continue;
		}

		// Standardfall: Abstand neu berechnen
		schwimmerzeitList.erase(it--);

		SchwimmerList::iterator next = it;
		next++; // next soll auf Naechstschlechteren zeigen

		unsigned itZeit   = (*it)->zeiten[disziplin];
		unsigned nextZeit = Zeit::MAX_UNSIGNED_VALUE; // falls it der letzte Schwimmer ist...
		if (next != schwimmerzeitList.end())
			nextZeit = (*next)->zeiten[disziplin];

		abstandsMap[*it] = nextZeit - itZeit;
	}
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

LagenstaffelComputer::LagenstaffelComputer(const SchwimmerList& schwimmer) :
		Lagenstaffel(schwimmer)
{
	disziplinenAufPositionen.reserve(ANZAHL_POSITIONEN);
	// Lagenstaffel (4 x 50 m Lagen)
	disziplinenAufPositionen.push_back(+Disziplin::RUECK_50);
	disziplinenAufPositionen.push_back(+Disziplin::BRUST_50);
	disziplinenAufPositionen.push_back(+Disziplin::SCHM_50);
	disziplinenAufPositionen.push_back(+Disziplin::FREI_50);

	abstaendeInDisziplinen = createAbstandsMap(schwimmerSortiert);
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
		PositionSchwimmerPairList eingesetzteSchwimmer; // eigentlich nur "testweise eingesetzte Schwimmer"!
		for (unsigned pos = 0; pos < disziplinenAufPositionen.size(); pos++)
			if (!vergebenePositionen[pos])
			{
				const int disziplin = disziplinenAufPositionen[pos];
				Schwimmer* const schw = *schwimmerSortiert[disziplin].begin();
				eingesetzteSchwimmer.push_back(PositionSchwimmerPair(pos, schw));
				ergebnis[pos] = schw;
			}

		PositionSchwimmerPair* mostWanted = findMostWanted(eingesetzteSchwimmer);

		// Diesen Schwimmer festsetzen fuer seine Position
		const int position    = mostWanted->first;
		Schwimmer* const schw = mostWanted->second;
		const int disziplin   = disziplinenAufPositionen[position];

		eingesetzteSchwimmer.sort(NormAbstandComparer(*this)); // Sortierung nur fuer die Debug-Ausgabe
//		gscheideDebugAusgabe(clog, disziplinenAufPositionen, schwimmerSortiert, eingesetzteSchwimmer, abstaendeInDisziplinen);

		nichtvergebenePositionen--;
		vergebenePositionen[position] = true;
		removeFromAvailable(schw, availableSchwimmer, schwimmerSortiert, abstaendeInDisziplinen);
		ensureMixedBedingung(*schw, neededGeschlecht, availableSchwimmer);
		gesamtzeit += schw->zeiten[disziplin];
	}
}
