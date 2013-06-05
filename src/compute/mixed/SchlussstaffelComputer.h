
#ifndef SCHLUSSSTAFFELCOMUTER_H_
#define SCHLUSSSTAFFELCOMUTER_H_

#include "../Schlussstaffel.h"

// Berechnet die exakte Lösung (weil's einfach ist bei Schlussstaffel)
class SchlussstaffelComputer: public Schlussstaffel
{
    void schwimmerFestsetzen(Schwimmer*& schw, int fehlt[2], int& position, int& positionenLeft);

public:
	SchlussstaffelComputer(const SchwimmerList&);

	void compute();
};

#endif /* SCHLUSSSTAFFELCOMUTER_H_ */
