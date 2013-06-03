
#include <set>
#include <cassert>
#include <iomanip>

#include "KraulstaffelComputer.h"
#include "../Zeit.h"

using namespace std;

KraulstaffelComputer::KraulstaffelComputer(const SchwimmerList& schwimmer) :
		Kraulstaffel(schwimmer)
{
}

void KraulstaffelComputer::schwimmerFestsetzen(Schwimmer*& schw, int fehlt[2], int& position, int& positionenLeft)
{
    if(fehlt[schw->geschlecht] > 0)
       fehlt[schw->geschlecht]--; // bis 0 runterzaehlen

    ergebnis[position] = schw;
    gesamtzeit += schw->zeiten[DISZIPLIN];
    positionenLeft--;
    position++;
}
/**
 * Algorithmus:
 *
 * Einfach die besten Krauler nehmen
 * mit Rücksicht auf die "Mixed"-Bedingung (Quote).
 *
 */
void KraulstaffelComputer::compute()
{
	// "Mixed"-Bedingungen: 2 Schwimmer, 2 Schwimmerinnen
	// Immer im Hinterkopf behalten: Hier gibt's nur (m/w) -> binaer
	const int QUOTE = 2;
	int fehlt[2] = { QUOTE, QUOTE }; // Anzahl m und w, die fuer die Quote noch fehlen

	SchwimmerList::const_iterator it = schwimmerSortiert[DISZIPLIN].begin();
	for (int position = 0,
			positionenUebrig = ANZAHL_POSITIONEN;
			position < ANZAHL_POSITIONEN &&
			it != schwimmerSortiert[DISZIPLIN].end(); ++it)
	{
		Schwimmer* schw = *it;
		if (positionenUebrig > QUOTE) // noch zwei Positionen uebrig lassen um die Quote sicherzustellen
		    schwimmerFestsetzen(schw, fehlt, position, positionenUebrig);

		else if (fehlt[Schwimmer::MAENNLICH] < positionenUebrig &&
				 fehlt[Schwimmer::WEIBLICH ] < positionenUebrig)
			// noch nicht kritisch -> den Schwimmer nehmen, der kommt
			schwimmerFestsetzen(schw, fehlt, position, positionenUebrig);
		else
			continue; // Schwimmer ueberspringen, wenn wir ihn wegen der Quote nicht nehmen koennen
	}
}
