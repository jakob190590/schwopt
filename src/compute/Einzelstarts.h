
#ifndef EINZELSTARTS_H_
#define EINZELSTARTS_H_

#include "SchwoptComputer.h"
#include "SchwoptAlgo.h"

class Einzelstarts: public SchwoptComputer
{
protected:
	PositionDisziplinTable disziplinenAufPositionen;
public:
	static const int ANZAHL_POSITIONEN = 8;

	Einzelstarts(const SchwimmerList&);

	void outputResult(ostream&) const;
};

#endif /* EINZELSTARTS_H_ */
