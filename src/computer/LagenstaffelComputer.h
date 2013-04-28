/*
 * LagenstaffelComputer.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMUTER_H_
#define LAGENSTAFFELCOMUTER_H_

#include "OptComputer.h"

class LagenstaffelComputer: public OptComputer
{
public:
	static const int ANZAHL_POSITIONEN_IN_STAFFEL = 4;
	static const int DISZIPLINEN_IN_STAFFEL[];

	LagenstaffelComputer(const SchwimmerVector&);

	ostream& outputResult(ostream&);
};

#endif /* LAGENSTAFFELCOMUTER_H_ */
