/*
 * OptComputer.h
 *
 *  Created on: 17.04.2013
 *      Author: jakob190590
 */

#ifndef OPTCOMPUTER_H_
#define OPTCOMPUTER_H_

#include <iostream>

#include "Schwimmer.h"

// Alle OptComputer sind Einmal-Computer, d.h. es ist pro Objekt nur ein einziger Aufruf von compute moeglich.
// Danach koennen sie weggeworfen werden!
class OptComputer {
protected:
	// Array mit Comperator (Greater)-Funktionen, um Schwimmer nach verschiedenen Disziplinen zu sortieren
	static bool (*schwimmerZeitLowerComperators[])
			(Schwimmer const * const, Schwimmer const * const);

	SchwimmerVector schwimmer;
	SchwimmerList schwimmerSortiert[Disziplin::ANZAHL];

	// Ergebnisse, werden gefuellt von compute()
	SchwimmerVector result;
	unsigned gesamtzeit;
public:
	OptComputer(const SchwimmerVector&);

	virtual void compute() = 0;
	virtual unsigned getTime();
	virtual SchwimmerVector getResult();
	virtual ostream& outputResult(ostream&) = 0;
};

#endif /* OPTCOMPUTER_H_ */
