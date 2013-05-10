/*
 * LagenstaffelComputer.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMPUTER_H_
#define LAGENSTAFFELCOMPUTER_H_

#include <map>

#include "SchwoptAlgoComputer.h"

class LagenstaffelComputer: public SchwoptAlgoComputer
{
	class NormAbstandComparer;

	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef vector<PositionSchwimmerPair> PositionSchwimmerPairVector;
	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap; // Fuer normierte Abstaende zw. Schwimmern in je einer Disziplin

	class NormAbstandComparer
	{
		SchwoptAlgoComputer& computer;
	public:
		NormAbstandComparer(SchwoptAlgoComputer&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerAbstandMap abstaende[Disziplin::ANZAHL];
	PositionSchwimmerPair* findMostWanted(PositionSchwimmerPairVector&);
	void removeFromAvailable(Schwimmer*, SchwimmerSet&);
	void ensureMixedBedingung(Schwimmer&, int neededGeschlecht[2], SchwimmerSet& availableSchwimmer);

	// Debugging
	ostream& outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin);
	ostream& outputEingesetzteSchwimmer(ostream&, const PositionSchwimmerPairVector&);

public:
	LagenstaffelComputer(const SchwimmerVector&);

	void compute();
};

#endif /* LAGENSTAFFELCOMPUTER_H_ */
