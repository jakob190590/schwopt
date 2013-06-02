
#ifndef GESAMT_H_
#define GESAMT_H_

#include "SchwoptComputer.h"

#include "Lagenstaffel.h"
#include "Kraulstaffel.h"
#include "Einzelstarts.h"

class Gesamt: public SchwoptComputer
{
protected:
	typedef vector<int> DisziplinenAufPositionen;

	DisziplinenAufPositionen disziplinenAufPositionen;
public:
	static const int ANZAHL_POSITIONEN = Lagenstaffel::ANZAHL_POSITIONEN + Kraulstaffel::ANZAHL_POSITIONEN + Einzelstarts::ANZAHL_POSITIONEN;

	enum Block {
		LAGENSTAFFEL = 0,  // Lagenstaffel
		KRAULSTAFFEL,      // Kraulstaffel
		EINZELSTARTS_50,   // Einzelstarts (50 m)
		EINZELSTARTS_100,  // Einzelstarts (100 m)
		ANZAHL_BLOCKE };

	Gesamt(const SchwimmerList& schwimmer);

	void outputResult(ostream&) const;

	int getBlock(int position);
};

#endif /* GESAMT_H_ */
