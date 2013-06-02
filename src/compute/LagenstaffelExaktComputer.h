
#ifndef LAGENSTAFFELCOMUTER2_H_
#define LAGENSTAFFELCOMUTER2_H_

#include <map>

#include "Lagenstaffel.h"

class LagenstaffelExaktComputer: public Lagenstaffel
{
public:
	LagenstaffelExaktComputer(const SchwimmerList&);

	void compute();
};

#endif /* LAGENSTAFFELCOMUTER_H_ */
