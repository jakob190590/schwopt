
#ifndef GESAMTNOTCOMPUTER_H_
#define GESAMTNOTCOMPUTER_H_

#include "../Gesamt.h"

// Berechnet nicht die optimale Besetzung der Positionen, sondern
// fuer eine gegebene Belegung die Zwischen- und Gesamtzeiten.
class GesamtNotComputer: public Gesamt
{
	const SchwimmerList& eingesetzteSchwimmer;
public:
	GesamtNotComputer(const SchwimmerList&, const SchwimmerList&);
	void compute();
};

#endif /* GESAMTNOTCOMPUTER_H_ */
