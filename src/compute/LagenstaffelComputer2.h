
#ifndef LAGENSTAFFELCOMUTER2_H_
#define LAGENSTAFFELCOMUTER2_H_

#include <map>

#include "Lagenstaffel.h"

class LagenstaffelComputer2: public Lagenstaffel
{
public:
	LagenstaffelComputer2(const SchwimmerList&);

	void compute();
};

#endif /* LAGENSTAFFELCOMUTER_H_ */
