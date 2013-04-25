/*
 * EinzelstartsComputer.h
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#ifndef EINZELSTARTSCOMPUTER_H_
#define EINZELSTARTSCOMPUTER_H_

#include "OptComputer.h"

class EinzelstartsComputer: public OptComputer
{
public:
	EinzelstartsComputer(const SchwimmerVector&);

	void compute();
	unsigned getTime();
	SchwimmerVector getResult();
	ostream& outputResult(ostream&);
};

#endif /* EINZELSTARTSCOMPUTER_H_ */
