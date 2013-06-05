
#include <set>
#include <vector>
#include <iostream>
#include <cassert>
#include <iomanip>
#include <algorithm>

#include "LagenstaffelComputer.h"
#include "../../Zeit.h"
#include "../../Debugging.h"

using namespace std;

PositionSchwimmerPair* LagenstaffelComputer::findMostWanted(PositionSchwimmerPairList& list)
{
	PositionSchwimmerPair* result = NULL;
	// Abstand in Diziplin auf der angegebenen Position, fuer den Schwimmer, der fuer diese Position vorgesehen ist
	unsigned greatestAbstand = 0;
	for (PositionSchwimmerPairList::iterator it = list.begin();
			it != list.end(); ++it)
	{
		unsigned abstand = abstaendeInDisziplinen[positionDisziplinTable[it->first]][it->second];
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
		Lagenstaffel(schwimmer), abstaendeInDisziplinen(createAbstandsMap(schwimmerSortiert))
{
}

void LagenstaffelComputer::compute()
{
	// Variablen fuer die Berechnung:
	// Anzahl Positionen, die noch nicht vergeben sind
	int nichtvergebenePositionen = positionDisziplinTable.size();
	// Positionen, die schon fest vergeben sind
	vector<bool> vergebenePositionen(positionDisziplinTable.size(), false);
	// Noch verfuegbare Schwimmer
	SchwimmerSet availableSchwimmer(schwimmer.begin(), schwimmer.end());
	// "Mixed"-Bedingungen: 2 Schwimmer, 2 Schwimmerinnen
	int neededGeschlecht[2] = { 2, 2 };

	// hier geht's los!
	while (nichtvergebenePositionen > 0)
	{
		// Ueberall wo noch nicht vergeben ist, Besten einsetzen
		PositionSchwimmerPairList eingesetzteSchwimmer; // eigentlich nur "testweise eingesetzte Schwimmer"!
		for (unsigned pos = 0; pos < positionDisziplinTable.size(); pos++)
			if (!vergebenePositionen[pos])
			{
				const int disziplin = positionDisziplinTable[pos];
				Schwimmer* const schw = *schwimmerSortiert[disziplin].begin();
				eingesetzteSchwimmer.push_back(PositionSchwimmerPair(pos, schw));
				ergebnis[pos] = schw;
			}

		PositionSchwimmerPair* mostWanted = findMostWanted(eingesetzteSchwimmer);

		// Diesen Schwimmer festsetzen fuer seine Position
		const int position    = mostWanted->first;
		Schwimmer* const schw = mostWanted->second;
		const int disziplin   = positionDisziplinTable[position];

#ifdef DEBUG
		eingesetzteSchwimmer.sort(NormAbstandComparer(*this)); // Sortierung nur fuer die Debug-Ausgabe
		gscheideDebugAusgabe(clog, positionDisziplinTable, schwimmerSortiert, eingesetzteSchwimmer, abstaendeInDisziplinen);
#endif

		nichtvergebenePositionen--;
		vergebenePositionen[position] = true;
		removeFromAvailable(schw, availableSchwimmer, schwimmerSortiert, abstaendeInDisziplinen);
		ensureMixedBedingung(*schw, neededGeschlecht, availableSchwimmer);
		gesamtzeit += schw->zeiten[disziplin];
	}
}
