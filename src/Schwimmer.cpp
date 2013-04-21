/*
 * Schwimmer.cpp
 *
 *  Created on: 14.04.2013
 *      Author: jakob190590
 */

#include <iostream>

#include "Schwimmer.h"
#include "Zeit.h"

using namespace std;

Schwimmer::Schwimmer(const string& nachname, const string& vorname, const string& kuerzel) :
		nachname(nachname), vorname(vorname), kuerzel(kuerzel)
{
	for (int i = 0; i < ANZAHL_DISZIPLINEN; i++)
		zeiten[i] = 0;
}

Schwimmer::Schwimmer(const string& nachname, const string& vorname, const string& kuerzel,
		unsigned brust50, unsigned brust100,
		unsigned rueck50, unsigned rueck100,
		unsigned schm50,  unsigned schm100,
		unsigned frei50,  unsigned frei100) :

		nachname(nachname), vorname(vorname), kuerzel(kuerzel)
		//zeiten(brust50, brust100, rueck50, rueck100,
		//		schm50, schm100, schm50, schm100)
{
	zeiten[BRUST_50]  = brust50;
	zeiten[BRUST_100] = brust100;
	zeiten[RUECK_50]  = rueck50;
	zeiten[RUECK_100] = rueck100;
	zeiten[SCHM_50]   = schm50;
	zeiten[SCHM_100]  = schm100;
	zeiten[FREI_50]   = frei50;
	zeiten[FREI_100]  = frei100;
	zeitLower<0>((const Schwimmer*) this, (const Schwimmer*) this);
}

ostream& operator <<(ostream& os, const Schwimmer& schw)
{
	os << schw.nachname << '\t' << schw.vorname << '\t'  << schw.kuerzel << '\t';
	os << Zeit::convertToString(schw.zeiten[BRUST_50]) << '\t';
	os << Zeit::convertToString(schw.zeiten[BRUST_100]) << '\t';
	os << Zeit::convertToString(schw.zeiten[RUECK_50]) << '\t';
	os << Zeit::convertToString(schw.zeiten[RUECK_100]) << '\t';
	os << Zeit::convertToString(schw.zeiten[SCHM_50]) << '\t';
	os << Zeit::convertToString(schw.zeiten[SCHM_100]) << '\t';
	os << Zeit::convertToString(schw.zeiten[FREI_50]) << '\t';
	os << Zeit::convertToString(schw.zeiten[FREI_100]);
	os << endl;
	return os;
}

istream& operator >>(istream& is, Schwimmer& schw)
{
	is >> schw.nachname >> schw.vorname >> schw.kuerzel;
	is >> schw.zeiten[BRUST_50];
	is >> schw.zeiten[BRUST_100];
	is >> schw.zeiten[RUECK_50];
	is >> schw.zeiten[RUECK_100];
	is >> schw.zeiten[SCHM_50];
	is >> schw.zeiten[SCHM_100];
	is >> schw.zeiten[FREI_50];
	is >> schw.zeiten[FREI_100];

	char rest[200];
	is.getline(rest, 200);
	clog << rest << endl;
	if (*rest && !is.eof()) cerr << "letzte zeile (die mit kommentar) weggelesen (inkl. \\n) ABER kein EOF in sicht!" << endl;
	return is;
}

ostream& operator <<(ostream& os, Schwimmer const * const schw)
{
	return operator <<(os, *schw);
}

istream& operator >>(istream& is, Schwimmer * const schw)
{
	return operator >>(is, *schw);
}

ostream& operator <<(ostream& os, const SchwimmerVector& schwVec)
{
	for (SchwimmerVector::const_iterator it = schwVec.begin();
			it != schwVec.end(); ++it)
		os << *it;

	return os;
}

