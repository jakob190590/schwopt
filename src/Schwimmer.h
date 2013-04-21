/*
 * Schwimmer.h
 *
 *  Created on: 14.04.2013
 *      Author: jakob190590
 */

#ifndef SCHWIMMER_H_
#define SCHWIMMER_H_

#include <vector>
#include <string>

using namespace std;

// Konstanten fuer Lage/Strecke (Indizes)
#define BRUST_50    0
#define BRUST_100   1
#define RUECK_50    2
#define RUECK_100   3
#define SCHM_50     4
#define SCHM_100    5
#define FREI_50     6
#define FREI_100    7

#define ANZAHL_DISZIPLINEN 8

class Schwimmer; // forward declaration

typedef vector<Schwimmer*> SchwimmerVector;

class Schwimmer {
public:

	// Daten zum Schwimmer
	string nachname;
	string vorname;
	string kuerzel;

	// Bestzeiten fuer Lage/Strecke

	unsigned zeiten[8];

	Schwimmer(const string& nachname = "", const string& vorname = "", const string& kuerzel = "");
	Schwimmer(const string& nachname, const string& vorname, const string& kuerzel,
			unsigned brust50, unsigned brust100,
			unsigned rueck50, unsigned rueck100,
			unsigned schm50,  unsigned schm100,
			unsigned frei50,  unsigned frei100);

	friend ostream& operator <<(ostream&, const Schwimmer&);
	friend istream& operator >>(istream&, Schwimmer&);

};

ostream& operator <<(ostream&, Schwimmer const * const);
istream& operator >>(istream&, Schwimmer * const);

ostream& operator <<(ostream&, const SchwimmerVector&);

// template (non-specialized) function's impl must be visible to a translation unit that uses it. otherwiese: "undefinied reference to <template function>"
template<int disziplin> bool zeitLower(const Schwimmer* schw1, const Schwimmer* schw2)
{
	return schw1->zeiten[disziplin] < schw2->zeiten[disziplin];
}

#endif /* SCHWIMMER_H_ */
