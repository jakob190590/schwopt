/*
 * EinzelstartsComputer.h
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#ifndef EINZELSTARTSCOMPUTER_H_
#define EINZELSTARTSCOMPUTER_H_

#include "SchwoptAlgoComputer.h"

class EinzelstartsComputer: public SchwoptAlgoComputer
{
	class NormAbstandComparer;

	typedef map<Schwimmer*, int> SchwimmerIntMap;
	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;
	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap; // Fuer normierte Abstaende zw. Schwimmern in je einer Disziplin

	class NormAbstandComparer
	{
		EinzelstartsComputer& computer;
	public:
		NormAbstandComparer(EinzelstartsComputer&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerAbstandMap abstaende[Disziplin::ANZAHL];

// Debugging
	ostream& outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin);
	ostream& outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet&);
public:
	static const int ANZAHL_POSITIONEN = 8;
	static const int DISZIPLINEN[];

	EinzelstartsComputer(const SchwimmerVector&);

	void removeFromAvailable(Schwimmer*);

	void compute();
	ostream& outputResult(ostream&);
};

#endif /* EINZELSTARTSCOMPUTER_H_ */
