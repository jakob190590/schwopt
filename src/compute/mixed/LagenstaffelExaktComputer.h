
#ifndef MIXED_LAGENSTAFFELEXAKTCOMUTER_H_
#define MIXED_LAGENSTAFFELEXAKTCOMUTER_H_

#include <map>

#include "../Lagenstaffel.h"

namespace Mixed
{

// Berechnet die exakte Lösung durch Durchprobieren.
// Bei vier Positionen geht das noch...
class LagenstaffelExaktComputer: public Lagenstaffel
{
public:
	LagenstaffelExaktComputer(const SchwimmerList&);

	void compute();
};

}

#endif /* MIXED_LAGENSTAFFELEXAKTCOMUTER_H_ */
