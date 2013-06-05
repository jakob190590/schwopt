
#ifndef KRAULSTAFFELCOMUTER_H_
#define KRAULSTAFFELCOMUTER_H_

#include "../Kraulstaffel.h"

// Berechnet die exakte Lösung (weil's einfach ist bei Kraulstaffel)
class KraulstaffelComputer: public Kraulstaffel
{
    void schwimmerFestsetzen(Schwimmer*& schw, int fehlt[2], int& position, int& positionenLeft);

public:
	KraulstaffelComputer(const SchwimmerList&);

	void compute();
};

#endif /* KRAULSTAFFELCOMUTER_H_ */
