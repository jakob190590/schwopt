
#ifndef MIXED_SCHLUSSSTAFFELCOMUTER_H_
#define MIXED_SCHLUSSSTAFFELCOMUTER_H_

#include "../Schlussstaffel.h"

namespace Mixed
{

// Berechnet die exakte Lösung (weil's einfach ist bei Schlussstaffel)
class SchlussstaffelComputer: public Schlussstaffel
{
    void schwimmerFestsetzen(Schwimmer*& schw, int fehlt[2], int& position, int& positionenLeft);

public:
	SchlussstaffelComputer(const SchwimmerList&);

	void compute();
};

}

#endif /* MIXED_SCHLUSSSTAFFELCOMUTER_H_ */
