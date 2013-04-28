/*
 * LagenstaffelComputer2.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMUTER2_H_
#define LAGENSTAFFELCOMUTER2_H_

#include <map>

#include "OptComputer.h"

class LagenstaffelComputer2: public OptComputer
{
	static const int ANZAHL_POSITIONEN_IN_STAFFEL = 4;
	static const int DISZIPLINEN_IN_STAFFEL[];

public:
	LagenstaffelComputer2(const SchwimmerVector&);

	void compute();
	ostream& outputResult(ostream&);
};

#endif /* LAGENSTAFFELCOMUTER_H_ */
