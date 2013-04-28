/*
 * LagenstaffelComputer1.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMPUTER1_H_
#define LAGENSTAFFELCOMPUTER1_H_

#include <map>

#include "LagenstaffelComputer.h"

class LagenstaffelComputer1: public LagenstaffelComputer
{
	class NormAbstandComparer;

	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;

	class NormAbstandComparer
	{
		LagenstaffelComputer1& computer;
	public:
		NormAbstandComparer(LagenstaffelComputer1&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	map<Schwimmer*, float> normierteAbstaende[Disziplin::ANZAHL];

public:
	LagenstaffelComputer1(const SchwimmerVector&);

	void compute();
};

#endif /* LAGENSTAFFELCOMPUTER1_H_ */
