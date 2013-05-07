/*
 * SchwoptAlgoComputer.h
 *
 *  Created on: 01.05.2013
 *      Author: jakob190590
 */

#ifndef SCHWOPTALGOCOMPUTER_H_
#define SCHWOPTALGOCOMPUTER_H_

#include <vector>
#include <set>
#include <map>

#include "../../Schwimmer.h"
#include "../OptComputer.h"

using namespace std;

class SchwoptAlgoComputer: public OptComputer
{
protected:
public:
	class NormAbstandComparer;


	typedef vector<int> DisziplinenAufPositionen;

	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef vector<PositionSchwimmerPair> PositionSchwimmerPairVector;
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;

	// Fuer Abstaende je eines Schwimmern zum Naechstschlechteren in einer Disziplin
	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap;

	class NormAbstandComparer
	{
		SchwoptAlgoComputer& computer;
	public:
		NormAbstandComparer(SchwoptAlgoComputer&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerList schwimmerzeitList;
	DisziplinenAufPositionen disziplinen;
	SchwimmerAbstandMap abstaende[Disziplin::ANZAHL];
	void removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer);

	// Debugging
	ostream& outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin) const;
	ostream& outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet&) const;

	void gscheideDebugAusgabe(ostream&,
			const SchwoptAlgoComputer::DisziplinenAufPositionen&,
			const SchwimmerVector[Disziplin::ANZAHL],
			const SchwoptAlgoComputer::PositionSchwimmerPairVector&,
			const SchwoptAlgoComputer::SchwimmerAbstandMap[Disziplin::ANZAHL],
			unsigned anzahlNaechstbester = 2, bool showDisziplin = true) const;
};

#endif /* SCHWOPTALGOCOMPUTER_H_ */
