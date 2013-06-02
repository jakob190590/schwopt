
#ifndef EINZELSTARTS_H_
#define EINZELSTARTS_H_

#include "SchwoptComputer.h"

class Einzelstarts: public SchwoptComputer
{
protected:
	typedef vector<int> DisziplinenAufPositionen;
	DisziplinenAufPositionen disziplinenAufPositionen;
public:
	static const int ANZAHL_POSITIONEN = 8;

	Einzelstarts(const SchwimmerVector&);

	void outputResult(ostream&) const;
};

#endif /* EINZELSTARTS_H_ */
