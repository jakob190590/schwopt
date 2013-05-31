/*
 * EinzelstartsComputer.h
 *
 *  Created on: 25.04.2013
 *      Author: jakob190590
 */

#ifndef EINZELSTARTSCOMPUTER_H_
#define EINZELSTARTSCOMPUTER_H_

#include "SchwoptAlgoComputer.h"

class EinzelstartsComputer: public SchwoptAlgoComputer
{
public:
	EinzelstartsComputer(const SchwimmerVector&);

	void compute();
	void outputResult(ostream&) const;
};

#endif /* EINZELSTARTSCOMPUTER_H_ */
