
#include "SchwoptAlgo.h"

SchwimmerAbstandMapVector createAbstandsMap(const SchwimmerListVector& schwimmerSortiert)
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
