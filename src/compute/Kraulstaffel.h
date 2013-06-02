
#ifndef KRAULSTAFFEL_H_
#define KRAULSTAFFEL_H_

#include "SchwoptComputer.h"

class Kraulstaffel: public SchwoptComputer
{
public:
	static const int ANZAHL_POSITIONEN = 6;
	static const int DISZIPLIN = Disziplin::FREI_50;

	Kraulstaffel(const SchwimmerList&);

	void outputResult(ostream&) const;
};

#endif /* KRAULSTAFFEL_H_ */
