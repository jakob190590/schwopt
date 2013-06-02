
#ifndef GESAMTCOMPUTERBASE_H_
#define GESAMTCOMPUTERBASE_H_

#include "SchwoptComputer.h"

class GesamtComputerBase: public SchwoptComputer
{
protected:
	typedef vector<int> DisziplinenAufPositionen;

	DisziplinenAufPositionen disziplinenAufPositionen;
public:
	static const int ANZAHL_POSITIONEN = 18;//TODO LagenstaffelComputerBase::ANZAHL_POSITIONEN + KraulstaffelComputerBase::ANZAHL_POSITIONEN + EinzelstartsComputerBase::ANZAHL_POSITIONEN;

	static const int ANZAHL_BLOCKE = 4; // Lagenstaffel, Kraulstaffel, Einzelstarts (50 m), Einzelstarts (100 m)
	static const int BLOCK_LAGENSTAFFEL = 0;
	static const int BLOCK_KRAULSTAFFEL = 1;
	static const int BLOCK_EINZELSTARTS_50  = 2;
	static const int BLOCK_EINZELSTARTS_100 = 3;

	GesamtComputerBase(const SchwimmerVector& schwimmer);

	void outputResult(ostream&) const;

	int getBlock(int position);
};

#endif /* GESAMTCOMPUTERBASE_H_ */
