/*
 * SchwoptAlgo.h
 *
 *  Created on: 01.05.2013
 *      Author: jakob190590
 */

#ifndef SCHWOPTALGO_H_
#define SCHWOPTALGO_H_

#include <vector>
#include <set>
#include <map>

#include "../Schwimmer.h"

using namespace std;

class SchwoptAlgo
{
protected:
public:
	class NormAbstandComparer;

	typedef vector<int> DisziplinenAufPositionen;

	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef vector<PositionSchwimmerPair> PositionSchwimmerPairVector;
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;
	typedef vector<SchwimmerList> SchwimmerListVector;

	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap; // Fuer Abstaende zw. Schwimmern in je einer Disziplin

	class NormAbstandComparer
	{
		SchwoptAlgo& computer;
	public:
		NormAbstandComparer(SchwoptAlgo&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerList schwimmerzeitList;
	SchwimmerListVector schwimmerSortiert;
	DisziplinenAufPositionen disziplinen;
	SchwimmerAbstandMap abstaende[Disziplin::ANZAHL];
	void entfAusSchwimmerSortiertUndAbstaende(Schwimmer*);

	// Debugging
	ostream& outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin);
	ostream& outputAbstaendeSortiert(ostream& os, const SortedPositionSchwimmerSet&);

	void gscheideDebugAusgabe(ostream&,
			const SchwoptAlgo::DisziplinenAufPositionen&,
			const SchwimmerVector[Disziplin::ANZAHL],
			const SchwoptAlgo::PositionSchwimmerPairVector&,
			const SchwoptAlgo::SchwimmerAbstandMap[Disziplin::ANZAHL],
			unsigned anzahlNaechstbester = 2, bool showDisziplin = true);
};

#endif /* SCHWOPTALGO_H_ */
