/*
 * LagenstaffelComputer2.h
 *
 *  Created on: 21.04.2013
 *      Author: jakob190590
 */

#ifndef LAGENSTAFFELCOMUTER2_H_
#define LAGENSTAFFELCOMUTER2_H_

#include <map>

#include "LagenstaffelComputerBase.h"

class LagenstaffelComputer2: public LagenstaffelComputerBase
{
public:
	LagenstaffelComputer2(const SchwimmerVector&);

	void compute();
};

#endif /* LAGENSTAFFELCOMUTER_H_ */
