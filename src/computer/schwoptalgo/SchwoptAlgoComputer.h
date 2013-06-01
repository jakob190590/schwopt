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
#include "../../Disziplin.h"
#include "../OptComputer.h"

using namespace std;

class SchwoptAlgoComputer: public OptComputer
{
protected:
public:
	class NormAbstandComparer;


	typedef vector<int> DisziplinenAufPositionen;

	typedef pair<int, Schwimmer*> PositionSchwimmerPair;
	typedef list<PositionSchwimmerPair> PositionSchwimmerPairList;
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;
	typedef map<Schwimmer*, int> SchwimmerIntMap;

	// Fuer Abstaende je eines Schwimmern zum Naechstschlechteren in einer Disziplin
	typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap;
	typedef vector<SchwimmerAbstandMap> SchwimmerAbstandMapVector;

	class NormAbstandComparer
	{
		SchwoptAlgoComputer& computer;
	public:
		NormAbstandComparer(SchwoptAlgoComputer&);
		bool operator ()(const PositionSchwimmerPair&, const PositionSchwimmerPair&);
	};

	SchwimmerList schwimmerzeitList;
	DisziplinenAufPositionen disziplinenAufPositionen;
	SchwimmerAbstandMapVector abstaendeInDisziplinen;

	SchwoptAlgoComputer(const SchwimmerVector&);
	void outputResult(ostream& os) const;

	// Debugging
	void outputSchwimmerAbstand(ostream&, const SchwimmerAbstandMap&, int disziplin) const;
	void outputEingesetzteSchwimmer(ostream& os, const SortedPositionSchwimmerSet&) const;

	void gscheideDebugAusgabe(ostream&,
			const SchwoptAlgoComputer::DisziplinenAufPositionen&,
			const SchwimmerListVector&,
			const SchwoptAlgoComputer::PositionSchwimmerPairList&,
			const vector<SchwoptAlgoComputer::SchwimmerAbstandMap>&,
			unsigned anzahlNaechstbester = 2, bool showDisziplin = true) const;
protected:
    virtual SchwimmerAbstandMapVector createAbstandsMap(const SchwimmerListVector) const;
};

#endif /* SCHWOPTALGOCOMPUTER_H_ */
