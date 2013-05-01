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
	typedef vector<PositionSchwimmerPair> PositionSchwimmerPairVector;
	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap; // Fuer normierte Abstaende zw. Schwimmern in je einer Disziplin

	class NormAbstandComparer
	{
		LagenstaffelComputer1& computer;
	public:
		NormAbstandComparer(LagenstaffelComputer1&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerAbstandMap abstaende[Disziplin::ANZAHL];
	PositionSchwimmerPair* findMostWanted(PositionSchwimmerPairVector&);
	void removeFromAvailable(Schwimmer*);
	void ensureMixedBedingung();

	// Debugging
	ostream& outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin);
	ostream& outputEingesetzteSchwimmer(ostream&, const PositionSchwimmerPairVector&);

public:
	LagenstaffelComputer1(const SchwimmerVector&);

	void compute();
};

#endif /* LAGENSTAFFELCOMPUTER1_H_ */
