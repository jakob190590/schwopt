/*
 * Disziplin.cpp
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#include "Disziplin.h"

string Disziplin::convertToString(int disziplin)
{
	switch (disziplin)
	{
	case BRUST_50:
		return "50 Meter Brust";
	case BRUST_100:
		return "100 Meter Brust";
	case RUECK_50:
		return "50 Meter Ruecken";
	case RUECK_100:
		return "100 Meter Ruecken";
	case SCHM_50:
		return "50 Meter Schmetterling";
	case SCHM_100:
		return "100 Meter Schmetterling";
	case FREI_50:
		return "50 Meter Freistil";
	case FREI_100:
		return "100 Meter Freistil";
	default:
		return "undefiniert";
	}
}
