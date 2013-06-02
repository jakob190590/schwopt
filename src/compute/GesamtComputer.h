
#ifndef GESAMTCOMPUTER_H_
#define GESAMTCOMPUTER_H_

#include <vector>
#include <map>

#include "Gesamt.h"

class GesamtComputer: public Gesamt
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
		GesamtComputer& computer;
	public:
		NormAbstandComparer(GesamtComputer& c) : computer(c) { };
		bool operator ()(const PositionSchwimmerPair& p1, const PositionSchwimmerPair& p2)
		{
			// Abstand in Diziplin, die auf der angegebenen Position gilt, und fuer den Schwimmer, der fuer diese Position vorgesehen ist
			return computer.abstaendeInDisziplinen[computer.disziplinenAufPositionen[p1.first]][p1.second] > computer.abstaendeInDisziplinen[computer.disziplinenAufPositionen[p2.first]][p2.second];
		}
	};

	SchwimmerList schwimmerzeitList;
	DisziplinenAufPositionen disziplinenAufPositionen;
	SchwimmerAbstandMapVector abstaendeInDisziplinen;


	vector<SchwimmerAbstandMap> createAbstandsMap(const SchwimmerListVector) const;

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
	void ensureMixedBedingung(int block,
			int sexNeeded[ANZAHL_BLOCKE][2],
			int vacantPositionenPerBlock[ANZAHL_BLOCKE],
			vector<SchwimmerSet>& availableSchwimmerPerBlock,
			vector<SchwimmerListVector>& schwimmerSortiertPerBlock,
			vector<SchwimmerAbstandMapVector>& abstaendeInDisziplinenPerBlock) const;
public:
	GesamtComputer(const SchwimmerList&);

	void compute();
};

#endif /* GESAMTCOMPUTER_H_ */
