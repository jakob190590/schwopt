
#ifndef LAGENSTAFFEL_H_
#define LAGENSTAFFEL_H_

#include "SchwoptComputer.h"

class Lagenstaffel: public SchwoptComputer
{
public:
	static const int ANZAHL_POSITIONEN = 4;
	static const int DISZIPLINEN_IN_STAFFEL[];

	Lagenstaffel(const SchwimmerList&);

	void outputResult(ostream&) const;
};

#endif /* LAGENSTAFFEL_H_ */
