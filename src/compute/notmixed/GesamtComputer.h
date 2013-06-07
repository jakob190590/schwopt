
#ifndef NOTMIXED_GESAMTCOMPUTER_H_
#define NOTMIXED_GESAMTCOMPUTER_H_

#include <vector>
#include <map>

#include "../Gesamt.h"
#include "../SchwoptAlgo.h"

namespace NotMixed
{

class GesamtComputer: public Gesamt
{
protected:

	class NormAbstandComparer
	{
		GesamtComputer& computer;
	public:
		NormAbstandComparer(GesamtComputer& c) : computer(c) { };
		bool operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
		{
			// Abstand in Diziplin, die auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
			return computer.abstaendeInDisziplinen[computer.positionDisziplinTable[p1.first]][p1.second] > computer.abstaendeInDisziplinen[computer.positionDisziplinTable[p2.first]][p2.second];
		}
	};
	typedef set<PositionSchwimmerPair, NormAbstandComparer> SortedPositionSchwimmerSet;

	SchwimmerAbstandMapVector abstaendeInDisziplinen;

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
public:
	GesamtComputer(const SchwimmerList&);

	void compute();
};

}

#endif /* NOTMIXED_GESAMTCOMPUTER_H_ */
