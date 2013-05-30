/*
 * EinzelstartsComputer1.h
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#ifndef EINZELSTARTSCOMPUTER1_H_
#define EINZELSTARTSCOMPUTER1_H_

#include "OptComputer.h"

class EinzelstartsComputer1: public OptComputer
{
	class NormAbstandComparer;

	typedef map<Schwimmer*, int> SchwimmerIntMap;
	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;
	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap; // Fuer normierte Abstaende zw. Schwimmern in je einer Disziplin

	class NormAbstandComparer
	{
		EinzelstartsComputer1& computer;
	public:
		NormAbstandComparer(EinzelstartsComputer1&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerAbstandMap abstaende[Disziplin::ANZAHL];

// Debugging
	ostream& outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin);
	ostream& outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet&);
public:
	static const int ANZAHL_POSITIONEN = 8;
	static const int DISZIPLINEN[];

	EinzelstartsComputer1(const SchwimmerVector&);

	void removeFromAvailable(Schwimmer*);

	void compute();
	ostream& outputResult(ostream&) const;
};

#endif /* EINZELSTARTSCOMPUTER1_H_ */
