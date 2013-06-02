
#ifndef SCHWOPTCOMPUTER_H_
#define SCHWOPTCOMPUTER_H_

#include <iostream>

#include "../Schwimmer.h"

// Alle SchwoptComputer sind Einmal-Computer, d.h. es ist pro Objekt nur ein einziger Aufruf von compute moeglich.
// Danach koennen sie weggeworfen werden!
class SchwoptComputer {
	// anzahlPositionen wird zum initialisieren von ergebnis verwendet
	const int anzahlPositionen;
protected:
	// Array mit Comperator (Less)-Funktionen, um Schwimmer nach verschiedenen Disziplinen zu sortieren
	static bool (*schwimmerZeitLessComperators[])
			(Schwimmer const * const, Schwimmer const * const);

	SchwimmerList schwimmer;
	SchwimmerListVector schwimmerSortiert;

	// Ergebnisse, werden gefuellt von compute()
	SchwimmerVector ergebnis;
	unsigned gesamtzeit;
public:
	SchwoptComputer(const SchwimmerList&, const int& numberPositionen);

	int getNumberOfPositions() const { return anzahlPositionen; };

	virtual void compute() = 0;
	virtual unsigned        getTime()   const { return gesamtzeit; };
	virtual SchwimmerVector getResult() const { return ergebnis; };
	virtual void outputResult(ostream&) const = 0;
};

#endif /* SCHWOPTCOMPUTER_H_ */
