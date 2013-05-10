/*
 * Disziplin.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef DISZIPLIN_H_
#define DISZIPLIN_H_

#include <string>

using namespace std;

class Disziplin
{
public:
	enum Strecke { METER_50 = 0, METER_100 = 1 };

	// Konstanten fuer Disziplinen (Lage/Strecke) -> Indizes
	static const int BRUST_50  = 0;
	static const int BRUST_100 = 1;
	static const int RUECK_50  = 2;
	static const int RUECK_100 = 3;
	static const int SCHM_50   = 4;
	static const int SCHM_100  = 5;
	static const int FREI_50   = 6;
	static const int FREI_100  = 7;

	// Anzahl der oben definierten Disziplinen
	static const int ANZAHL = 8;

	static Strecke getStrecke(int disziplin);
	static int convertStreckeToInt(Strecke);
	static string convertToString(int disziplin, bool lageAusgeben = true,
			bool streckeAusgeben = true, string meterAusgabe = "Meter");
};

#endif /* DISZIPLIN_H_ */
