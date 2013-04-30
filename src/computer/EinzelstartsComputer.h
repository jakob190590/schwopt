/*
 * EinzelstartsComputer.h
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#ifndef EINZELSTARTSCOMPUTER_H_
#define EINZELSTARTSCOMPUTER_H_

#include "OptComputer.h"

class EinzelstartsComputer: public OptComputer
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
public:
	static const int ANZAHL_POSITIONEN = 8;
	static const int DISZIPLINEN[];

	EinzelstartsComputer(const SchwimmerVector&);

	void entfAusSchwimmerSortiertUndAbstaende(Schwimmer*);

	void compute();
	ostream& outputResult(ostream&);
};

#endif /* EINZELSTARTSCOMPUTER_H_ */
