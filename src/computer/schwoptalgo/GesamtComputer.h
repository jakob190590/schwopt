/*
 * GesamtComputer.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef GESAMTCOMPUTER_H_
#define GESAMTCOMPUTER_H_

#include "SchwoptAlgoComputer.h"
#include "LagenstaffelComputer.h"
#include "EinzelstartsComputer.h"

class GesamtComputer: public SchwoptAlgoComputer
{
protected:
	int getBlock(int position);
	PositionSchwimmerPair* findMostWanted(PositionSchwimmerPairList& list);
	void removeFromAvailable(Schwimmer*,
			SchwimmerSet& availableSchwimmer,
			SchwimmerListVector& schwimmerSortiert,
			SchwimmerAbstandMapVector& abstaendeInDisziplinen) const;
	void ensureMax3Bedingung(Schwimmer* schw,
			SchwimmerIntMap& nAvailableSchwimmer,
			SchwimmerSet& availableSchwimmer,
			SchwimmerListVector& schwimmerSortiert,
			SchwimmerAbstandMapVector& abstaendeInDisziplinen,
			vector<SchwimmerSet>& availableSchwimmerPerBlock,
			vector<SchwimmerListVector>& schwimmerSortiertPerBlock,
			vector<SchwimmerAbstandMapVector>& abstaendeInDisziplinenPerBlock) const;
	void ensureStaffelBedingung(Schwimmer* schw, int block,
			vector<SchwimmerSet>& availableSchwimmerPerBlock,
			vector<SchwimmerListVector>& schwimmerSortiertPerBlock,
			vector<SchwimmerAbstandMapVector>& abstaendeInDisziplinenPerBlock) const;
	void ensureMixedBedingung(int sexNeeded[2], int vacantPositionen,
			SchwimmerSet& availableSchwimmer,
			SchwimmerListVector& schwimmerSortiert,
			SchwimmerAbstandMapVector& abstaendeInDisziplinen) const;
public:
	static const int ANZAHL_POSITIONEN_KRAULSTAFFEL = 6;
	static const int ANZAHL_POSITIONEN = LagenstaffelComputer::ANZAHL_POSITIONEN + ANZAHL_POSITIONEN_KRAULSTAFFEL + EinzelstartsComputer::ANZAHL_POSITIONEN;

	static const int ANZAHL_BLOCKE = 4; // Lagenstaffel, Kraulstaffel, Einzelstarts (50 m), Einzelstarts (100 m)
	static const int BLOCK_LAGENSTAFFEL = 0;
	static const int BLOCK_KRAULSTAFFEL = 1;
	static const int BLOCK_EINZELSTARTS_50  = 2;
	static const int BLOCK_EINZELSTARTS_100 = 3;

	GesamtComputer(const SchwimmerVector&);

	void compute();
	void outputResult(ostream&) const;
};

#endif /* GESAMTCOMPUTER_H_ */
