/*
 * Schwimmer.h
 *
 *  Created on: 14.04.2013
 *      Author: jakob190590
 */

#ifndef SCHWIMMER_H_
#define SCHWIMMER_H_

#include <vector>
#include <list>
#include <set>
#include <string>
#include <algorithm>

#include "Disziplin.h"

using namespace std;

class Schwimmer; // forward declaration

typedef vector<Schwimmer*> SchwimmerVector;
typedef list<Schwimmer*> SchwimmerList;
typedef set<Schwimmer*> SchwimmerSet;

class Schwimmer {
public:
	enum Geschlecht { MAENNLICH = 0, WEIBLICH = 1 };
	class GeschlechtPredicate
	{
		const Schwimmer::Geschlecht& geschlecht;
	public:
		GeschlechtPredicate(const Schwimmer::Geschlecht& g) : geschlecht(g) { };
		bool operator ()(const Schwimmer* s) { return s->geschlecht == geschlecht; };
	};
	class KuerzelPredicate
	{
		const string kuerzel;
	public:
		KuerzelPredicate(const string& k) : kuerzel(k) { };
		bool operator ()(const Schwimmer* s) { return s->kuerzel == kuerzel; };

	};

	// Daten zum Schwimmer
	string nachname;
	string vorname;
	string kuerzel;
	Geschlecht geschlecht;

	// Bestzeiten fuer Lage/Strecke

	unsigned zeiten[8];

	Schwimmer(const Geschlecht& = MAENNLICH, const string& nachname = "", const string& vorname = "", const string& kuerzel = "");
	Schwimmer(const Geschlecht&, const string& nachname, const string& vorname, const string& kuerzel,
			unsigned brust50, unsigned brust100,
			unsigned rueck50, unsigned rueck100,
			unsigned schm50,  unsigned schm100,
			unsigned frei50,  unsigned frei100);

	friend ostream& operator <<(ostream&, const Schwimmer&);
	friend istream& operator >>(istream&, Schwimmer&);

};

template<typename C> Schwimmer* lookupSchwimmer(const C& container, const string& kuerzel)
{
	typename C::const_iterator it = find_if(container.begin(), container.end(),
			Schwimmer::KuerzelPredicate(kuerzel));
	if (it != container.end())
		return *it;
	return NULL;
}
//Schwimmer* lookupSchwimmer<SchwimmerVector>(const SchwimmerVector&, const string&);

ostream& operator <<(ostream&, Schwimmer const * const);
istream& operator >>(istream&, Schwimmer * const);

// template (non-specialized) function's impl must be visible to a translation unit that uses it.
// otherwiese: "undefinied reference to <template function>"
template<int disziplin> bool zeitLess(const Schwimmer* schw1, const Schwimmer* schw2)
{
	return schw1->zeiten[disziplin] < schw2->zeiten[disziplin];
}

#endif /* SCHWIMMER_H_ */
