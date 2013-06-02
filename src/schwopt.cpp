
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>

#include "Schwimmer.h"

#include "compute/KraulstaffelComputer.h"
#include "compute/LagenstaffelComputer2.h"
#include "compute/LagenstaffelComputer.h"
//#include "compute/EinzelstartsComputer.h"
#include "compute/GesamtComputer.h"
#include "compute/ManuellerGesamtComputer.h"

using namespace std;

void coutSchwimmer(Schwimmer* s)
{
	cout << s;
}

void deleteSchwimmer(Schwimmer* s)
{
	delete s;
}

int main(int argc, char* argv[])
{
	if (argc == 1) // keine argumente
	{
		cerr << "usage: schwopt <datafilename>" << endl;
		return EXIT_FAILURE;
	}

	ifstream ifs(argv[1]);
	if (!ifs.is_open())
	{
		cerr << "schwopt: cannot open data file" << endl;
		return EXIT_FAILURE;
	}
	SchwimmerList schwimmer;
	while (!ifs.eof())
	{
		Schwimmer* schw = new Schwimmer();
		ifs >> schw;
		schwimmer.push_back(schw);
	}
	ifs.close();

	// lookupSchwimmer Test
	assert(lookupSchwimmer(schwimmer, "LH") == *schwimmer.begin());
	assert(lookupSchwimmer(schwimmer, "lh") == NULL);

	// So, ab hier kann mit dem vector schwimmer gearbeitet werden
	cout << endl << "Nachname       Vorname   Kurzl brust50 brust100 rueck50 rueck100 schm50 schm100 frei50 frei100";
	cout << endl << "--------------------------------------------------------------------------------------------" << endl;
	for_each(schwimmer.begin(), schwimmer.end(), coutSchwimmer);

	cout << "// [Exakt] KraulstaffelComputer (Exakte Loesung)" << endl;
	KraulstaffelComputer kraulstaffelComputer(schwimmer);
	kraulstaffelComputer.compute();
	kraulstaffelComputer.outputResult(cout);

	cout << "// [Exakt] LagenstaffelComputer2 (Exakte Loesung, Durchprobieren)" << endl;
	LagenstaffelComputer2 lagenstaffelComputer2(schwimmer);
	lagenstaffelComputer2.compute();
	lagenstaffelComputer2.outputResult(cout);

	cout << "// [SchwoptAlgo] LagenstaffelComputer" << endl;
	LagenstaffelComputer lagenstaffelComputer(schwimmer);
	lagenstaffelComputer.compute();
	lagenstaffelComputer.outputResult(cout);

//	cout << "// [SchwoptAlgo] Einzelstarts" << endl;
//	EinzelstartsComputer einzelstartsComputer(schwimmer);
//	einzelstartsComputer.compute();
//	einzelstartsComputer.outputResult(cout);

	cout << "// [SchwoptAlgo] GesamtComputer" << endl;
	GesamtComputer gesamtComputer(schwimmer);
	gesamtComputer.compute();
	gesamtComputer.outputResult(cout);

	cout << "// [Eigene Belegung] GesamtComputer" << endl
	     << "Manuelle Eingabe von Schwimmerkuerzeln" << endl
	     << "Eingabe beenden mit <Enter> <Strg + Z>" << endl
	     << "bzw. unter Unix mit <Enter> <Strg + D>" << endl;
	SchwimmerList eingesetzteSchwimmer;
	string input;
	while (cin >> input)
		eingesetzteSchwimmer.push_back(lookupSchwimmer(schwimmer, input));
	ManuellerGesamtComputer manuellerGesamtComputer(schwimmer, eingesetzteSchwimmer);
	manuellerGesamtComputer.compute();
	manuellerGesamtComputer.outputResult(cout);


	// Schwimmer in list freigeben
	for_each(schwimmer.begin(), schwimmer.end(), deleteSchwimmer);

	return EXIT_SUCCESS;
}
