
#ifndef LAGENSTAFFEL_H_
#define LAGENSTAFFEL_H_

#include "SchwoptComputer.h"
#include "SchwoptAlgo.h"

class Lagenstaffel: public SchwoptComputer
{
protected:
	PositionDisziplinTable positionDisziplinTable;
public:
	static const int ANZAHL_POSITIONEN = 4;

	Lagenstaffel(const SchwimmerList&);

	void outputResult(ostream&) const;
};

#endif /* LAGENSTAFFEL_H_ */
