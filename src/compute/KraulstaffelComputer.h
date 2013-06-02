
#ifndef KRAULSTAFFELCOMUTER_H_
#define KRAULSTAFFELCOMUTER_H_

#include "Kraulstaffel.h"

class KraulstaffelComputer: public Kraulstaffel
{
    void schwimmerFestsetzen(Schwimmer*& schw, int fehlt[2], int& position, int& positionenLeft);

public:
	KraulstaffelComputer(const SchwimmerVector&);

	void compute();
};

#endif /* KRAULSTAFFELCOMUTER_H_ */
