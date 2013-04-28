/*
 * LagenstaffelComputer2.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <set>
#include <climits>
#include <cassert>
#include <iomanip>

#include "LagenstaffelComputer2.h"
#include "../Zeit.h"

using namespace std;

const int LagenstaffelComputer2::DISZIPLINEN_IN_STAFFEL[] = { Disziplin::RUECK_50, Disziplin::BRUST_50, Disziplin::SCHM_50, Disziplin::FREI_50 };

// Optimale Lösung (durchprobieren!)
LagenstaffelComputer2::LagenstaffelComputer2(const SchwimmerVector& schwimmer) :
		OptComputer(schwimmer)
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
void LagenstaffelComputer2::compute()
{
	result.resize(ANZAHL_POSITIONEN_IN_STAFFEL);

    typedef SchwimmerVector::const_iterator It;
    const It begin = schwimmer.begin();
    const It end   = schwimmer.end();

	unsigned best = UINT_MAX;

    for(It it_s0 = begin; it_s0 != end; ++it_s0)
	{
		unsigned summe0 = (*it_s0)->zeiten[Disziplin::RUECK_50];
		for (It it_s1 = begin; it_s1 != end; ++it_s1)
		{
			if (it_s1 == it_s0)
				continue;

			// Summe der Zeiten der Schwimmer auf den Positionen 0 und 1
			unsigned summe01 = summe0 + (*it_s1)->zeiten[Disziplin::BRUST_50];
			for (It it_s2 = begin; it_s2 != end; ++it_s2)
			{
				if (it_s2 == it_s1 || it_s2 == it_s0)
					continue;

				// Summe der Zeiten der Schwimmer auf den Positionen 0, 1 und 2
				unsigned summe012 = summe01 + (*it_s2)->zeiten[Disziplin::SCHM_50];
				for (It it_s3 = begin; it_s3 != end; ++it_s3)
				{
					if (it_s3 == it_s2 || it_s3 == it_s1 || it_s3 == it_s0)
						continue;

					// Summe der Zeiten der Schwimmer auf den allen vier Positionen (0 bis 3)
					unsigned summe0123 = summe012 + (*it_s3)->zeiten[Disziplin::FREI_50];

					if (summe0123 < best)
					{
						best = summe0123;
						result[0] = *it_s0;
						result[1] = *it_s1;
						result[2] = *it_s2;
						result[3] = *it_s3;
					}
				}
			}
		}
	}

	gesamtzeit = best;
}

ostream& LagenstaffelComputer2::outputResult(ostream& os)
{
	for (int i = 0; i < ANZAHL_POSITIONEN_IN_STAFFEL; i++)
	{
		int diszi = DISZIPLINEN_IN_STAFFEL[i];
		os << setiosflags(ios::left) << setw(23) << Disziplin::convertToString(diszi) << " " << getResult()[i]->kuerzel << "  " << Zeit::convertToString(result[i]->zeiten[diszi]) << endl;
	}
	os << "Gesamtzeit: " << Zeit::convertToString(getTime()) << endl;
	return os;
}
