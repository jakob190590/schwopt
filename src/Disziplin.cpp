/*
 * Disziplin.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include <string>
#include <sstream>

#include "Disziplin.h"

using namespace std;

int Disziplin::getStrecke(int disziplin)
{
	return (disziplin % 2 == 0) ? 50 : 100; // geht nur hier bei diesen Disziplinen!
}

string Disziplin::convertToString(int disziplin, bool lageAusgeben,
		bool streckeAusgeben, string meterAusgabe)
{
	string lage;
	// simple IntToString (fu C++) -- in C++11 gibt's std::to_string
	string strecke = (dynamic_cast<ostringstream&>(ostringstream() << getStrecke(disziplin))).str();
	switch (disziplin)
	{
	case BRUST_50:
	case BRUST_100:
		lage = "Brust";
		break;
	case RUECK_50:
	case RUECK_100:
		lage = "Ruecken";
		break;
	case SCHM_50:
	case SCHM_100:
		lage = "Schmetterling";
		break;
	case FREI_50:
	case FREI_100:
		lage = "Freistil";
		break;
	default:
		return "undefiniert";
	}

	string result;
	if (streckeAusgeben)
		result = strecke + " " + meterAusgabe + " ";
	if (lageAusgeben)
		result += lage;

	return result;
}
