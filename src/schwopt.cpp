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
#include "computer/KraulstaffelComputer.h"
#include "computer/LagenstaffelComputer2.h"
#include "computer/schwoptalgo/LagenstaffelComputer.h"
#include "computer/schwoptalgo/EinzelstartsComputer.h"
#include "computer/schwoptalgo/GesamtComputer.h"

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
//		cout << schw;
		schwimmer.push_back(schw);
	}
	ifs.close();

	// So, ab hier kann mit dem vector schwimmer gearbeitet werden
	cout << endl << "Nachname       Vorname Kurzl brust50 brust100 rueck50 rueck100 schm50 schm100 frei50 frei100";
	cout << endl << "--------------------------------------------------------------------------------------------" << endl;
	cout << schwimmer << endl;

	cout << "// [Exakt] KraulstaffelComputer (Exakte Loesung)" << endl;
	KraulstaffelComputer kraulstaffelComputer(schwimmer);
	kraulstaffelComputer.compute();
	kraulstaffelComputer.outputResult(cout);

	cout << "// [Exakt] LagenstaffelComputer2 (Exakte Loesung, Durchprobieren)" << endl;
	LagenstaffelComputer2 lagenstaffelComputer2(schwimmer);
	lagenstaffelComputer2.compute();
	lagenstaffelComputer2.outputResult(cout);

//	cout << "// [SchwoptAlgo] LagenstaffelComputer" << endl;
//	LagenstaffelComputer lagenstaffelComputer(schwimmer);
//	lagenstaffelComputer.compute();
//	lagenstaffelComputer.outputResult(cout);

	cout << "// [SchwoptAlgo] EinzelstartsComputer" << endl;
	EinzelstartsComputer einzelstartsComputer(schwimmer);
	einzelstartsComputer.compute();
	einzelstartsComputer.outputResult(cout);

//	cout << "// [SchwoptAlgo] GesamtComputer" << endl;
//	GesamtComputer gesamtComputer(schwimmer);
//	gesamtComputer.compute();
//	gesamtComputer.outputResult(cout);


	// Schwimmer im vector freigeben
	for (SchwimmerVector::const_iterator it = schwimmer.begin();
			it != schwimmer.end(); ++it)
		delete *it;

	return EXIT_SUCCESS;
}
