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

class OptComputer {
protected:
	// Array mit Comperator (Greater)-Funktionen, um Schwimmer nach verschiedenen Disziplinen zu sortieren
	static bool (*schwimmerZeitLowerComperators[])
			(Schwimmer const * const, Schwimmer const * const);

	SchwimmerVector schwimmer;
	SchwimmerVector schwimmerSortiert[Disziplin::ANZAHL];
	list<float> abstandZumBesseren[Disziplin::ANZAHL];
	list<float> abstandZumBesten[Disziplin::ANZAHL];
	SchwimmerVector result;
public:
	OptComputer(const SchwimmerVector&);

	virtual void compute() = 0;
	virtual unsigned getTime() = 0;
	virtual SchwimmerVector getResult() = 0;
	virtual ostream& outputResult(ostream&) = 0;
};

#endif /* OPTCOMPUTER_H_ */
