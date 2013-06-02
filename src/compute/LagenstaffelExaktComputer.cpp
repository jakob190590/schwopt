
#include <set>
#include <cassert>
#include <iomanip>

#include "LagenstaffelExaktComputer.h"
#include "../Zeit.h"

using namespace std;

// Optimale Lösung (durchprobieren!)
LagenstaffelExaktComputer::LagenstaffelExaktComputer(const SchwimmerList& schwimmer) :
		Lagenstaffel(schwimmer)
{
}

/**
 * Algorithmus:
 *
 * Schwimmer durchprobieren:
 * 1. Jeden einzelnen fuer 1. Position
 *    2. Jeden einzelnen der Uebrigen fuer 2. Position
 *       3. Jeden einzelnen der Uebrigen fuer 3. Position
 *          4. Jeden einzelnen der Uebrigen fuer 4. Position
 *
 * Ergibt Laufzeit:
 * n: Gesamtanzahl der Schwimmer
 * bei 4 Positionen in der Staffel
 * n * (n - 1) * (n - 2) * (n - 3) = (n! - (n - 4)!)
 * => O(n!) -- bisschen besser eigentlich...
 * In Ordnung, weil n nicht beliebig steigt.
 *
 */
void LagenstaffelExaktComputer::compute()
{
	typedef vector<int> IntVec;

	typedef SchwimmerList::const_iterator It;
	const It begin = schwimmer.begin();
	const It end   = schwimmer.end();

	unsigned best = UINT_MAX;

	// "Mixed"-Bedingungen: 2 Schwimmer, 2 Schwimmerinnen
	IntVec neededGeschlecht(2, 2); // 2 elemente, beide werte sind 2

	for(It it_s0 = begin; it_s0 != end; ++it_s0)
	{
		IntVec needed0(neededGeschlecht);
		needed0[(*it_s0)->geschlecht]--;

		unsigned zeit0 = (*it_s0)->zeiten[Disziplin::RUECK_50];
		if (zeit0 == Zeit::MAX_UNSIGNED_VALUE)
			continue;

		unsigned summe0 = zeit0;
		for (It it_s1 = begin; it_s1 != end; ++it_s1)
		{
			if (it_s1 == it_s0)
				continue;

			IntVec needed1(needed0);
			needed1[(*it_s1)->geschlecht]--;
			// Beim 2. Schwimmer noch keine Pruefung noetig

			unsigned zeit1 = (*it_s1)->zeiten[Disziplin::BRUST_50];
			if (zeit1 == Zeit::MAX_UNSIGNED_VALUE)
				continue;

			// Summe der Zeiten der Schwimmer auf den Positionen 0 und 1
			unsigned summe01 = summe0 + zeit1;
			for (It it_s2 = begin; it_s2 != end; ++it_s2)
			{
				if (it_s2 == it_s1 || it_s2 == it_s0)
					continue;

				IntVec needed2(needed1);
				if (needed2[(*it_s2)->geschlecht] > 0)
					needed2[(*it_s2)->geschlecht]--;
				else
					continue;

				unsigned zeit2 = (*it_s2)->zeiten[Disziplin::SCHM_50];
				if (zeit2 == Zeit::MAX_UNSIGNED_VALUE)
					continue;

				// Summe der Zeiten der Schwimmer auf den Positionen 0, 1 und 2
				unsigned summe012 = summe01 + zeit2;
				for (It it_s3 = begin; it_s3 != end; ++it_s3)
				{
					if (it_s3 == it_s2 || it_s3 == it_s1 || it_s3 == it_s0)
						continue;

					IntVec needed3(needed2);
					if (needed3[(*it_s3)->geschlecht] > 0)
						needed3[(*it_s3)->geschlecht]--;
					else
						continue;

					unsigned zeit3 = (*it_s3)->zeiten[Disziplin::FREI_50];
					if (zeit3 == Zeit::MAX_UNSIGNED_VALUE)
						continue;

					// Summe der Zeiten der Schwimmer auf den allen vier Positionen (0 bis 3)
					unsigned summe0123 = summe012 + zeit3;

					if (summe0123 < best)
					{
						best = summe0123;
						ergebnis[0] = *it_s0;
						ergebnis[1] = *it_s1;
						ergebnis[2] = *it_s2;
						ergebnis[3] = *it_s3;
					}
				}
			}
		}
	}

	gesamtzeit = best;
}
