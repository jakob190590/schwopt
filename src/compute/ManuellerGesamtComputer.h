
#ifndef MANUELLERGESAMTCOMPUTER_H_
#define MANUELLERGESAMTCOMPUTER_H_

#include "Gesamt.h"

class ManuellerGesamtComputer: public Gesamt
{
	const SchwimmerList& eingesetzteSchwimmer;
public:
	ManuellerGesamtComputer(const SchwimmerVector&, const SchwimmerList&);
	void compute();
};

#endif /* MANUELLERGESAMTCOMPUTER_H_ */
