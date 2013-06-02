
#ifndef GESAMTNOTCOMPUTER_H_
#define GESAMTNOTCOMPUTER_H_

#include "Gesamt.h"

class GesamtNotComputer: public Gesamt
{
	const SchwimmerList& eingesetzteSchwimmer;
public:
	GesamtNotComputer(const SchwimmerList&, const SchwimmerList&);
	void compute();
};

#endif /* GESAMTNOTCOMPUTER_H_ */
