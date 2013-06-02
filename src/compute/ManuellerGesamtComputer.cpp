
#include "ManuellerGesamtComputer.h"

ManuellerGesamtComputer::ManuellerGesamtComputer(const SchwimmerVector& schwimmer, const SchwimmerList& eingesetzteSchwimmer) :
		GesamtComputerBase(schwimmer), eingesetzteSchwimmer(eingesetzteSchwimmer)
{
}

void ManuellerGesamtComputer::compute()
{
	// ergebnis auffuellen und gesamtzeit berechnen
	SchwimmerList::const_iterator it = eingesetzteSchwimmer.begin();
	int pos;
	for (pos = 0; pos < ANZAHL_POSITIONEN && it != eingesetzteSchwimmer.end(); pos++, it++)
	{
		Schwimmer* schw = *it;
		ergebnis[pos] = schw;
		if (schw)
			gesamtzeit += schw->zeiten[disziplinenAufPositionen[pos]];
	}
}
