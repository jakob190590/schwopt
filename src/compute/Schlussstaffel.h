
#ifndef SCHLUSSSTAFFEL_H_
#define SCHLUSSSTAFFEL_H_

#include "SchwoptComputer.h"

class Schlussstaffel: public SchwoptComputer
{
public:
	static const int ANZAHL_POSITIONEN = 6;
	static const int DISZIPLIN = Disziplin::FREI_50;

	Schlussstaffel(const SchwimmerList&);

	void outputResult(ostream&) const;
};

#endif /* SCHLUSSSTAFFEL_H_ */
