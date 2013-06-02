
#ifndef GESAMT_H_
#define GESAMT_H_

#include "SchwoptComputer.h"
#include "SchwoptAlgo.h"
#include "Lagenstaffel.h"
#include "Kraulstaffel.h"
#include "Einzelstarts.h"

class Gesamt: public SchwoptComputer
{
protected:
	PositionDisziplinTable positionDisziplinTable;
	void outputZwischenergebnis(ostream& os, const unsigned& zeit, const string& divider) const;
public:
	static const int ANZAHL_POSITIONEN = Lagenstaffel::ANZAHL_POSITIONEN + Kraulstaffel::ANZAHL_POSITIONEN + Einzelstarts::ANZAHL_POSITIONEN;

	enum Block {
		LAGENSTAFFEL = 0,  // Lagenstaffel
		KRAULSTAFFEL,      // Kraulstaffel
		EINZELSTARTS_50,   // Einzelstarts (50 m)
		EINZELSTARTS_100,  // Einzelstarts (100 m)
		ANZAHL_BLOCKE };

	Block getBlock(int position);

	Gesamt(const SchwimmerList& schwimmer);

	void outputResult(ostream&) const;
};

#endif /* GESAMT_H_ */
