/*
 * GesamtComputer1.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef GESAMTCOMPUTER1_H_
#define GESAMTCOMPUTER1_H_

#include <map>

#include "LagenstaffelComputer.h"

class GesamtComputer1: public OptComputer
{
	static const int ANZAHL_POSITIONEN = 18;
	static const int DISZIPLINEN[];

	map<Schwimmer*, float> normierteAbstaende[Disziplin::ANZAHL];

public:
	GesamtComputer1(const SchwimmerVector&);

	void compute();
};

#endif /* GESAMTCOMPUTER1_H_ */
