
#ifndef SCHWOPTCOMPUTER_H_
#define SCHWOPTCOMPUTER_H_

#include <iostream>

#include "../Schwimmer.h"

// Alle SchwoptComputer sind Einmal-Computer, d.h. es ist pro Objekt nur ein einziger Aufruf von compute moeglich.
// Danach koennen sie weggeworfen werden!
class SchwoptComputer {
	const int anzahlPositionen;
public:
	typedef vector<SchwimmerList> SchwimmerListVector;
protected:
	// Array mit Comperator (Less)-Funktionen, um Schwimmer nach verschiedenen Disziplinen zu sortieren
	static bool (*schwimmerZeitLessComperators[])
			(Schwimmer const * const, Schwimmer const * const);

	SchwimmerVector schwimmer;
	SchwimmerListVector schwimmerSortiert;

	// Ergebnisse, werden gefuellt von compute()
	SchwimmerVector ergebnis;
	unsigned gesamtzeit;
public:
	SchwoptComputer(const SchwimmerVector&, const int& numberPositionen);

	virtual void compute() = 0;
	virtual unsigned        getTime()   const { return gesamtzeit; };
	virtual SchwimmerVector getResult() const { return ergebnis; };
	virtual void outputResult(ostream&) const = 0;

//	int getNumberPositionen() const { return anzahlPositionen; };
};

#endif /* SCHWOPTCOMPUTER_H_ */
