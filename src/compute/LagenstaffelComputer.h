
#ifndef LAGENSTAFFELCOMPUTER_H_
#define LAGENSTAFFELCOMPUTER_H_

#include <map>
#include <vector>

#include "Lagenstaffel.h"

class LagenstaffelComputer: public Lagenstaffel
{
protected:
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
		LagenstaffelComputer& computer;
	public:
		NormAbstandComparer(LagenstaffelComputer& c) : computer(c) { };
		bool operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
		{
			// Abstand in Diziplin, die auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
			return computer.abstaendeInDisziplinen[computer.disziplinenAufPositionen[p1.first]][p1.second] > computer.abstaendeInDisziplinen[computer.disziplinenAufPositionen[p2.first]][p2.second];
		}
	};

	SchwimmerList schwimmerzeitList;
	DisziplinenAufPositionen disziplinenAufPositionen;
	SchwimmerAbstandMapVector abstaendeInDisziplinen;

	virtual SchwimmerAbstandMapVector createAbstandsMap(const SchwimmerListVector) const;

	PositionSchwimmerPair* findMostWanted(PositionSchwimmerPairList&);
	void ensureMixedBedingung(Schwimmer&, int neededGeschlecht[2], SchwimmerSet& availableSchwimmer);
	void removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer,
				SchwimmerListVector& schwimmerSortiert,
				SchwimmerAbstandMapVector& abstaendeInDisziplinen);

public:
	static const int ANZAHL_POSITIONEN = 4;

	LagenstaffelComputer(const SchwimmerVector&);

	void compute();
};

#endif /* LAGENSTAFFELCOMPUTER_H_ */
