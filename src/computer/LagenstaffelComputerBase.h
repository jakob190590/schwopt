/*
 * LagenstaffelComputerBase.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMUTERBASE_H_
#define LAGENSTAFFELCOMUTERBASE_H_

#include "OptComputer.h"

class LagenstaffelComputerBase: public OptComputer
{
public:
	static const int ANZAHL_POSITIONEN_IN_STAFFEL = 4;
	static const int DISZIPLINEN_IN_STAFFEL[];

	LagenstaffelComputerBase(const SchwimmerVector&);

	ostream& outputResult(ostream&) const;
};

#endif /* LAGENSTAFFELCOMUTERBASE_H_ */
