
#ifndef LAGENSTAFFELEXAKTCOMUTER_H_
#define LAGENSTAFFELEXAKTCOMUTER_H_

#include <map>

#include "../Lagenstaffel.h"

// Berechnet die exakte Lösung durch Durchprobieren.
// Bei vier Positionen geht das noch...
class LagenstaffelExaktComputer: public Lagenstaffel
{
public:
	LagenstaffelExaktComputer(const SchwimmerList&);

	void compute();
};

#endif /* LAGENSTAFFELEXAKTCOMUTER_H_ */
