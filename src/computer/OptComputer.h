/*
 * OptComputer.h
 *
 *  Created on: 17.04.2013
 *      Author: jakob190590
 */

#ifndef OPTCOMPUTER_H_
#define OPTCOMPUTER_H_

#include <iostream>

#include "../Schwimmer.h"

// Alle OptComputer sind Einmal-Computer, d.h. es ist pro Objekt nur ein einziger Aufruf von compute moeglich.
// Danach koennen sie weggeworfen werden!
class OptComputer {
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
	OptComputer(const SchwimmerVector&);

	virtual void compute() = 0;
	virtual unsigned getTime() const;
	virtual SchwimmerVector getResult() const;
	virtual void outputResult(ostream&) const = 0;
};

#endif /* OPTCOMPUTER_H_ */
