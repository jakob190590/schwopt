
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <sstream>
#include <getopt.h>
#include <iomanip>

#include "Schwimmer.h"
#include "CLI.h"

#include "compute/KraulstaffelComputer.h"
#include "compute/LagenstaffelExaktComputer.h"
#include "compute/LagenstaffelComputer.h"
//#include "compute/EinzelstartsComputer.h"
#include "compute/GesamtComputer.h"
#include "compute/GesamtNotComputer.h"

using namespace std;

static const string usage = "usage: schwopt [--help] [or other options] <filename>";

static void exitWithError(const string& errmsg)
{
	cerr << "schwopt: " << errmsg << endl;
	exit(EXIT_FAILURE);
}

static void parseArguments(int argc, char* argv[], int& flag_no_compute, int& flag_dry, int& flag_verbose, Class& val_class, Block& val_block, string& val_param)
{
	int c;
	while (1)
	{
		static struct option long_options[] = {
			/* These options set a flag. */
			//{ "dry", no_argument, &flag_dry, 1 }, // wird jetzt unten behandelt, zusammen mit abkuerzung -d
			/* These options don't set a flag.
			 We distinguish them by their indices. */
			// Grossbuchstaben heissen bei mir, es gibt KEINE Kurzform
			{ "help",        no_argument, 0, 'H' },
			{ "version",     no_argument, 0, 'V' },
			{ "dry",         no_argument, 0, 'd' },
			{ "no-compute",  no_argument, 0, 'n' },
			{ "verbose",     no_argument, 0, 'v' },
			{ "class", required_argument, 0, 'C' },
			{ "block", required_argument, 0, 'B' },
			{ 0, 0, 0, 0 }
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "dnv", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			break;

		case 'H':
			{
				const int OPTION_COL_WIDTH = 18;
				cout << usage << endl << setiosflags(ios::left)
					 << setw(OPTION_COL_WIDTH) << " --help"           << endl
					 << setw(OPTION_COL_WIDTH) << " --version"        << endl
					 << setw(OPTION_COL_WIDTH) << " --class=CLASS"    << "mini-mixed, jugend-w, jugend-m, jugend-mixed," << endl
					 << setw(OPTION_COL_WIDTH) << ""                  << "damen, herren, mixed (default)" << endl
					 << setw(OPTION_COL_WIDTH) << " --block=BLOCK"    << "lagenstaffel, schlussstaffel," << endl
					 << setw(OPTION_COL_WIDTH) << ""                  << "einzelstarts, gesamt (default)" << endl
					 << setw(OPTION_COL_WIDTH) << " -d, --dry"        << "Nicht berechnen, nur leeres Ergebnis anzeigen" << endl
					 << setw(OPTION_COL_WIDTH) << " -n, --no-compute" << "Nicht berechnen, Belegung der Positionen selbst eingeben" << endl
					 << setw(OPTION_COL_WIDTH) << " -v, --verbose"    << endl;
				exit(EXIT_SUCCESS);
			}
			break;
		case 'V':
			// no version info yet
			cout << "schwopt version 1.0.0" << endl;
			exit(EXIT_SUCCESS);
			break;
		case 'd':
			flag_dry = 1;
			break;
		case 'n':
			flag_no_compute = 1;
			break;
		case 'v':
			flag_verbose = 1;
			break;
		case 'C':
			{
				int cl; // class
				for (cl = 0; cl < ANZAHL_CLASSES; cl++)
					if (strcmp(optarg, classNameTable[cl]) == 0)
					{
						val_class = (Class) cl;
						break;
					}
				if (cl == ANZAHL_CLASSES) // class not found
					exitWithError("invalid value `" + string(optarg) + "' for option -- class");
			}
			break;
		case 'B':
			{
				int bl; // block
				for (bl = 0; bl < ANZAHL_BLOCKS; bl++)
					if (strcmp(optarg, blockNameTable[bl]) == 0)
					{
						val_block = (Block) bl;
						break;
					}
				if (bl == ANZAHL_BLOCKS) // block not found
					exitWithError("invalid value `" + string(optarg) + "' for option -- block");
			}
			break;

		case '?':
			/* getopt_long already printed an error message. */
			break;

		default:
			// getopt failed oder eher: vergessen, einen case zu behandeln
			abort();
		}
	}

	if (optind < argc)
		val_param = argv[optind];
	else
	{
		cerr << usage << endl;
		exit(EXIT_FAILURE);
	}
}

static void readFile(const string& filename, SchwimmerList& schwimmer)
{
	// Datei oeffnen
	ifstream ifs(filename.c_str());
	if (!ifs.is_open())
		exitWithError("cannot open data file `" + filename + "'");

	// Schwimmer aus Datei in list einlesen
	while (ifs)
	{
		string line;
		getline(ifs, line);
		if (line.length() == 0 || line[0] == '#')
		{
			// Leerzeile oder Kommentarzeile
			continue;
		}

		istringstream iss(line);
		string nname, vname, kuerzel;
		char c;
		iss >> nname >> vname >> c >> kuerzel;
		Schwimmer::Geschlecht geschl = (c == 'w') ? Schwimmer::WEIBLICH : Schwimmer::MAENNLICH;
		Schwimmer* schw = new Schwimmer(geschl, nname, vname, kuerzel);

		for (int i = 0; iss && i < Disziplin::ANZAHL; i++)
		{
			string zeitInput;
			iss >> zeitInput;
			schw->zeiten[i] = Zeit::convertToUnsigned(zeitInput);
		}

		schwimmer.push_back(schw);
	}
	ifs.close();
}

static void coutSchwimmer(Schwimmer* s)
{
	cout << s;
}

static void deleteSchwimmer(Schwimmer* s)
{
	delete s;
}

int main(int argc, char* argv[])
{
	int flagNoCompute = 0;
	int flagDry = 0;
	int flagVerbose = 0;
	Class valClass = MIXED;
	Block valBlock = GESAMT;
	string valParam; // empty

	parseArguments(argc, argv,
			flagNoCompute, flagDry, flagVerbose,
			valClass, valBlock, valParam);

	SchwimmerList schwimmer;
	readFile(valParam, schwimmer);

	// So, ab hier kann mit der list schwimmer gearbeitet werden
	if (flagVerbose)
	{
		cout << endl << "Nachname       Vorname   Kurzl brust50 brust100 rueck50 rueck100 schm50 schm100 frei50 frei100";
		cout << endl << "--------------------------------------------------------------------------------------------" << endl;
		for_each(schwimmer.begin(), schwimmer.end(), coutSchwimmer);
	}

	switch (valClass) {
	case MIXED:
		switch (valBlock) {
		case LAGENSTAFFEL:
			{
				if (flagVerbose) cout << "// [Exakt] LagenstaffelExaktComputer (Exakte Loesung, Durchprobieren)" << endl;
				LagenstaffelExaktComputer lagenstaffelComputer2(schwimmer);
				lagenstaffelComputer2.compute();
				lagenstaffelComputer2.outputResult(cout);

				if (flagVerbose)
				{
					cout << "// [SchwoptAlgo] LagenstaffelComputer" << endl;
					LagenstaffelComputer lagenstaffelComputer(schwimmer);
					lagenstaffelComputer.compute();
					lagenstaffelComputer.outputResult(cout);
				}
			}
			break;

		case SCHLUSSSTAFFEL:
			{
				if (flagVerbose) cout << "// [Exakt] KraulstaffelComputer (Exakte Loesung)" << endl;
				KraulstaffelComputer kraulstaffelComputer(schwimmer);
				kraulstaffelComputer.compute();
				kraulstaffelComputer.outputResult(cout);
			}
			break;

		case EINZELSTARTS:
			{
//				if (flagVerbose) cout << "// [SchwoptAlgo] Einzelstarts" << endl;
//				EinzelstartsComputer einzelstartsComputer(schwimmer);
//				einzelstartsComputer.compute();
//				einzelstartsComputer.outputResult(cout);
			}
			break;

		case GESAMT:
			if (!flagDry && !flagNoCompute)
			{
				// Normale Berechnung und Ausgabe
				if (flagVerbose) cout << "// [SchwoptAlgo] GesamtComputer" << endl;
				GesamtComputer gesamtComputer(schwimmer);
				gesamtComputer.compute();
				gesamtComputer.outputResult(cout);
			}
			else
			{
				SchwimmerList eingesetzteSchwimmer;
				if (flagNoCompute)
				{
					if (flagVerbose)
						cout << "// [Eigene Belegung] GesamtComputer"    << endl
							 << "Manuelle Eingabe von Schwimmerkuerzeln" << endl
							 << "durch Leerzeichen getrennt!"            << endl
							 << "Eingabe beenden mit <Enter> <Strg + Z>" << endl
							 << "bzw. unter Unix mit <Enter> <Strg + D>" << endl;

					string input;
					while (cin >> input)
						eingesetzteSchwimmer.push_back(lookupSchwimmer(schwimmer, input));
				}

				GesamtNotComputer manuellerGesamtComputer(schwimmer, eingesetzteSchwimmer);
				manuellerGesamtComputer.compute();
				manuellerGesamtComputer.outputResult(cout);
			}
			break;

		default:
			exitWithError("block `" + string(blockNameTable[valBlock]) + "' not supported yet");
			break;
		}
		break;

	default:
		exitWithError("class `" + string(classNameTable[valClass]) + "' not supported yet");
		break;
	}

	// Schwimmer in list freigeben
	for_each(schwimmer.begin(), schwimmer.end(), deleteSchwimmer);

	return EXIT_SUCCESS;
}
