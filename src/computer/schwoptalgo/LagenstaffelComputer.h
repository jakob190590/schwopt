/*
 * LagenstaffelComputer.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMPUTER_H_
#define LAGENSTAFFELCOMPUTER_H_

#include "SchwoptAlgoComputer.h"

class LagenstaffelComputer: public SchwoptAlgoComputer
{
	PositionSchwimmerPair* findMostWanted(PositionSchwimmerPairList&);
	void ensureMixedBedingung(Schwimmer&, int neededGeschlecht[2], SchwimmerSet& availableSchwimmer);
	void removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer,
				SchwimmerListVector& schwimmerSortiert,
				SchwimmerAbstandMapVector& abstaendeInDisziplinen);

public:
	static const int ANZAHL_POSITIONEN = 4;

	LagenstaffelComputer(const SchwimmerVector&);

	void compute();
	void outputResult(ostream&) const;
};

#endif /* LAGENSTAFFELCOMPUTER_H_ */
