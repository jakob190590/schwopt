
#ifndef MANUELLERGESAMTCOMPUTER_H_
#define MANUELLERGESAMTCOMPUTER_H_

#include "Gesamt.h"

class GesamtNotComputer: public Gesamt
{
	const SchwimmerList& eingesetzteSchwimmer;
public:
	GesamtNotComputer(const SchwimmerList&, const SchwimmerList&);
	void compute();
};

#endif /* MANUELLERGESAMTCOMPUTER_H_ */
