/*
 * Debugging.h
 *
 *  Created on: 29.04.2013
 *      Author: jakob190590
 */

#ifndef DEBUGGING_H_
#define DEBUGGING_H_

#include <iostream>
#include <iterator>

#include "Zeit.h"
#include "Schwimmer.h"
#include "Disziplin.h"

using namespace std;

template <typename Iterator> ostream& outputSchwimmerZeiten(ostream& os,
		const Iterator& begin, const Iterator& end, int disziplin)
{
	os << "-----------------------------------------" << endl;
	os << "Schwimmer/Zeiten, Sortiert nach Zeit, Disziplin: " << Disziplin::convertToString(disziplin) << endl;
	for (Iterator it = begin; it != end; ++it)
	{
		Schwimmer& schw = **it;
		os << setiosflags(ios::left);
		os << setw(16) << schw.nachname << setw(10) << schw.vorname << setw(3) << schw.kuerzel;
		os << setiosflags(ios::right);
		os << setw(14) << Zeit::convertToString(schw.zeiten[disziplin]) << endl;
		os << resetiosflags(ios::right);
	}
	return os;
}

#endif /* DEBUGGING_H_ */
