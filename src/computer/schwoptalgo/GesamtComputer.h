/*
 * GesamtComputer.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef GESAMTCOMPUTER1_H_
#define GESAMTCOMPUTER1_H_

#include <map>

#include "SchwoptAlgoComputer.h"

class GesamtComputer1: public SchwoptAlgoComputer
{
	static const int ANZAHL_POSITIONEN = 18;
	static const int DISZIPLINEN[];

	map<Schwimmer*, float> normierteAbstaende[Disziplin::ANZAHL];

public:
	GesamtComputer1(const SchwimmerVector&);

	void compute();
};

#endif /* GESAMTCOMPUTER1_H_ */
