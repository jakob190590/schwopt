/*
 * Disziplin.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include "Disziplin.h"

string Disziplin::convertToString(int disziplin, bool lageAusgeben,
		bool streckeAusgeben, string meterAusgabe)
{
	string lage;
	string strecke = (disziplin % 2 == 0) ? "50" : "100"; // geht nur hier bei diesen Disziplinen!
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
