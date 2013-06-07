
#ifndef MIXED_LAGENSTAFFELCOMPUTER_H_
#define MIXED_LAGENSTAFFELCOMPUTER_H_

#include <map>
#include <vector>

#include "../Lagenstaffel.h"
#include "../SchwoptAlgo.h"

namespace Mixed
{

class LagenstaffelComputer: public Lagenstaffel
{
protected:
	class NormAbstandComparer
	{
		LagenstaffelComputer& computer;
	public:
		NormAbstandComparer(LagenstaffelComputer& c) : computer(c) { };
		bool operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
		{
			// Abstand in Diziplin, die auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
			return computer.abstaendeInDisziplinen[computer.positionDisziplinTable[p1.first]][p1.second] > computer.abstaendeInDisziplinen[computer.positionDisziplinTable[p2.first]][p2.second];
		}
	};
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;

	SchwimmerAbstandMapVector abstaendeInDisziplinen;

	PositionSchwimmerPair* findMostWanted(PositionSchwimmerPairList&);
	void ensureMixedBedingung(Schwimmer&, int neededGeschlecht[2], SchwimmerSet& availableSchwimmer);
	void removeFromAvailable(Schwimmer* schw, SchwimmerSet& availableSchwimmer,
				SchwimmerListVector& schwimmerSortiert,
				SchwimmerAbstandMapVector& abstaendeInDisziplinen);

public:
	static const int ANZAHL_POSITIONEN = 4;

	LagenstaffelComputer(const SchwimmerList&);

	void compute();
};

}

#endif /* MIXED_LAGENSTAFFELCOMPUTER_H_ */
