/*
 * LagenstaffelComputer1.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMPUTER1_H_
#define LAGENSTAFFELCOMPUTER1_H_

#include "OptComputer.h"

class LagenstaffelComputer1: public OptComputer
{
	static const int ANZAHL_POSITIONEN_IN_STAFFEL = 4;
	static const int DISZIPLINEN_IN_STAFFEL[];

	unsigned gesamtzeit;

public:
	LagenstaffelComputer1(const SchwimmerVector&);

	void compute();
	unsigned getTime();
	SchwimmerVector getResult();
	ostream& outputResult(ostream&);
};

#endif /* LAGENSTAFFELCOMPUTER1_H_ */
