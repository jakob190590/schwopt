//============================================================================
// Name        : schwopt.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Schwimmer.h"
#include "Zeit.h"
#include "computer/OptComputer.h"
#include "computer/LagenstaffelComputer1.h"
#include "computer/LagenstaffelComputer2.h"

using namespace std;


int main(int argc, char* argv[]) {

	if (argc == 1)
	{
		cout << "usage: schwopt <datafilename>" << endl;
		return EXIT_FAILURE;
	}

	cout << "first param: " << argv[1] << endl;

	SchwimmerVector schwimmer;
	ifstream ifs(argv[1]);
//	istream& inputstream = ifs || cin
	while (!ifs.eof())
	{
		Schwimmer* schw = new Schwimmer();
		ifs >> schw;
		cout << schw;
		schwimmer.push_back(schw);
	}
	ifs.close();

	// So, ab hier kann mit dem vector schwimmer gearbeitet werden
	cout << schwimmer << endl;

//	OptComputer optComputer(schwimmer);
	LagenstaffelComputer1 lagenstaffelComputer1(schwimmer);
	lagenstaffelComputer1.compute();
	lagenstaffelComputer1.outputResult(cout);

	LagenstaffelComputer2 lagenstaffelComputer2(schwimmer);
	lagenstaffelComputer2.compute();
	lagenstaffelComputer2.outputResult(cout);


	// Schwimmer im vector freigeben
	for (SchwimmerVector::const_iterator it = schwimmer.begin();
			it != schwimmer.end(); ++it)
		delete *it;

	return EXIT_SUCCESS;
}
