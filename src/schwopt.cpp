
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

#include "compute/mixed/SchlussstaffelComputer.h"
#include "compute/mixed/LagenstaffelExaktComputer.h"
#include "compute/mixed/LagenstaffelComputer.h"
#include "compute/mixed/GesamtComputer.h"
#include "compute/notmixed/GesamtComputer.h"
#include "compute/GesamtNotComputer.h"

using namespace std;

static const string usage   = "usage: schwopt [--help] [or other options] <filename>";
static const string version = "schwopt version 1.0.0";

static void exitWithError(const string& errmsg)
{
	cerr << "schwopt: " << errmsg << endl;
	exit(EXIT_FAILURE);
}

static void parseArguments(int argc, char* argv[], int& flag_dry, int& flag_input, int& flag_plain, int& flag_verbose, Class& val_class, Block& val_block, string& val_input, string& val_param)
{
	// Lesenwert: http://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html
	// Argument Syntax Conventions. Z. B. per Konvention aequivalent: -ifile.txt und -i file.txt
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
			{ "input", optional_argument, 0, 'i' },
			{ "plain",       no_argument, 0, 'p' },
			{ "verbose",     no_argument, 0, 'v' },
			{ "class", required_argument, 0, 'C' },
			{ "block", required_argument, 0, 'B' },
			{ 0, 0, 0, 0 }
		};

		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "di::pv", long_options, &option_index);

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
				const int OPTION_COL_WIDTH = 20;
				cout << usage << endl << setiosflags(ios::left)
					 << setw(OPTION_COL_WIDTH) << " --help"             << endl
					 << setw(OPTION_COL_WIDTH) << " --version"          << endl
					 << setw(OPTION_COL_WIDTH) << " --class=CLASS"      << "mini-mixed, jugend-w, jugend-m, jugend-mixed, damen, " << endl
					 << setw(OPTION_COL_WIDTH) << ""                    << "herren, mixed (default)" << endl
					 << setw(OPTION_COL_WIDTH) << " --block=BLOCK"      << "lagenstaffel, schlussstaffel, einzelstarts," << endl
					 << setw(OPTION_COL_WIDTH) << ""                    << "gesamt (default)" << endl
					 << setw(OPTION_COL_WIDTH) << " -d, --dry"          << "Nicht berechnen, nur leeres Ergebnis anzeigen" << endl
					 << setw(OPTION_COL_WIDTH) << " -i, --input[=FILE]" << "Nicht berechnen, Belegung der Positionen selbst eingeben" << endl
					 << setw(OPTION_COL_WIDTH) << " -p, --plain"        << "Nur die Schwimmerkuerzel als Ergebnis ausgeben" << endl
					 << setw(OPTION_COL_WIDTH) << " -v, --verbose"      << endl;
				exit(EXIT_SUCCESS);
			}
			break;
		case 'V':
			cout << version << endl;
			exit(EXIT_SUCCESS);
			break;
		case 'd':
			flag_dry = 1;
			break;
		case 'i':
			flag_input = 1;
			if (optarg)
				val_input = string(optarg);
			break;
		case 'p':
			flag_plain = 1;
			break;
		case 'v':
			flag_verbose = 1;
			break;
		case 'C':
			{
				int cl; // class
				for (cl = 0; cl < ANZAHL_CLASSES; cl++)
					if (strcmp(optarg, CLASS_NAME_TABLE[cl]) == 0)
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
					if (strcmp(optarg, BLOCK_NAME_TABLE[bl]) == 0)
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

static void readDataFile(const string& filename, SchwimmerList& schwimmer)
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

static void outputResult(ostream& os, const SchwoptComputer& computer, const bool& plain = false)
{
	if (!plain)
		computer.outputResult(os);
	else
	{
		SchwimmerVector vec = computer.getResult();
		for (SchwimmerVector::const_iterator it = vec.begin();
				it != vec.end(); ++it)
		{
			if (*it)
				os << (*it)->kuerzel;
			else
				os << "N/A";
			os << endl;
		}
	}
}

static void deleteSchwimmer(Schwimmer* s)
{
	delete s;
}

int main(int argc, char* argv[])
{
	int flagDry = 0;
	int flagInput = 0;
	int flagPlain = 0;
	int flagVerbose = 0;
	Class valClass = DEFAULT_CLASS;
	Block valBlock = DEFAULT_BLOCK;
	string valInput, valParam; // empty

	parseArguments(argc, argv,
			flagDry, flagInput, flagPlain, flagVerbose,
			valClass, valBlock, valInput, valParam);

	SchwimmerList schwimmer;
	readDataFile(valParam, schwimmer);

	// So, ab hier kann mit der list schwimmer gearbeitet werden
	if (flagVerbose)
	{
		cout << "Nachname       Vorname   Kurzl brust50 brust100 rueck50 rueck100 schm50 schm100 frei50 frei100" << endl
		     << "--------------------------------------------------------------------------------------------" << endl;
		for_each(schwimmer.begin(), schwimmer.end(), coutSchwimmer);
		cout << endl;
	}

	switch (valClass) {
	case MIXED:
	case JUGEND_MIXED:
		switch (valBlock) {
		case LAGENSTAFFEL:
			{
				if (flagVerbose) cout << "// [Exakt] LagenstaffelExaktComputer (Exakte Loesung, Durchprobieren)" << endl;
				Mixed::LagenstaffelExaktComputer lagenstaffelExaktComputer(schwimmer);
				lagenstaffelExaktComputer.compute();
				outputResult(cout, lagenstaffelExaktComputer, flagPlain);

				if (flagVerbose)
				{
					cout << "// [SchwoptAlgo] LagenstaffelComputer" << endl;
					Mixed::LagenstaffelComputer lagenstaffelComputer(schwimmer);
					lagenstaffelComputer.compute();
					outputResult(cout, lagenstaffelComputer, flagPlain);
				}
			}
			break;

		case SCHLUSSSTAFFEL:
			{
				if (flagVerbose) cout << "// [Exakt] KraulstaffelComputer (Exakte Loesung)" << endl;
				Mixed::SchlussstaffelComputer kraulstaffelComputer(schwimmer);
				kraulstaffelComputer.compute();
				outputResult(cout, kraulstaffelComputer, flagPlain);
			}
			break;

		case EINZELSTARTS:
			{
//				if (flagVerbose) cout << "// [SchwoptAlgo] Einzelstarts" << endl;
//				Mixed::EinzelstartsComputer einzelstartsComputer(schwimmer);
//				einzelstartsComputer.compute();
//				outputResult(cout, einzelstartsComputer, flagPlain);
			}
			break;

		case GESAMT:
			if (!flagDry && !flagInput)
			{
				// Normale Berechnung und Ausgabe
				if (flagVerbose) cout << "// [SchwoptAlgo] GesamtComputer" << endl;
				Mixed::GesamtComputer gesamtComputer(schwimmer);
				gesamtComputer.compute();
				outputResult(cout, gesamtComputer, flagPlain);
			}
			else
			{
				SchwimmerList eingesetzteSchwimmer;
				if (flagInput)
				{
					if (flagVerbose && valInput.empty()) // nur wenn verbose und input nicht aus Datei gelesen wird
						cout << "// [Eigene Belegung] GesamtComputer"    << endl
							 << "Manuelle Eingabe von Schwimmerkuerzeln" << endl
							 << "durch Leerzeichen getrennt!"            << endl
							 << "Eingabe beenden mit <Enter> <Strg + Z>" << endl
							 << "bzw. unter Unix mit <Enter> <Strg + D>" << endl;

					string input;
					if (valInput.empty())
						while (cin >> input)
							eingesetzteSchwimmer.push_back(lookupSchwimmer(schwimmer, input));
					else
					{
						ifstream ifs(valInput.c_str());
						if (!ifs.is_open())
							exitWithError("cannot open input file `" + valInput + "'");
						while (ifs >> input)
							eingesetzteSchwimmer.push_back(lookupSchwimmer(schwimmer, input));
					}
				}

				GesamtNotComputer gesamtNotComputer(schwimmer, eingesetzteSchwimmer);
				gesamtNotComputer.compute();
				outputResult(cout, gesamtNotComputer, flagPlain);
			}
			break;

		default:
			exitWithError("block `" + string(BLOCK_NAME_TABLE[valBlock]) + "' not supported yet");
			break;
		}
		break;

	case DAMEN:
	case HERREN:
	case JUGEND_W:
	case JUGEND_M:
		switch (valBlock) {
		case GESAMT:
			if (!flagDry && !flagInput)
			{
				// Normale Berechnung und Ausgabe
				if (flagVerbose) cout << "// [SchwoptAlgo] GesamtComputer" << endl;
				NotMixed::GesamtComputer gesamtComputer(schwimmer);
				gesamtComputer.compute();
				outputResult(cout, gesamtComputer, flagPlain);
			}
			else
			{
				SchwimmerList eingesetzteSchwimmer;
				if (flagInput)
				{
					if (flagVerbose && valInput.empty()) // nur wenn verbose und input nicht aus Datei gelesen wird
						cout << "// [Eigene Belegung] GesamtComputer"    << endl
							 << "Manuelle Eingabe von Schwimmerkuerzeln" << endl
							 << "durch Leerzeichen getrennt!"            << endl
							 << "Eingabe beenden mit <Enter> <Strg + Z>" << endl
							 << "bzw. unter Unix mit <Enter> <Strg + D>" << endl;

					string input;
					if (valInput.empty())
						while (cin >> input)
							eingesetzteSchwimmer.push_back(lookupSchwimmer(schwimmer, input));
					else
					{
						ifstream ifs(valInput.c_str());
						if (!ifs.is_open())
							exitWithError("cannot open input file `" + valInput + "'");
						while (ifs >> input)
							eingesetzteSchwimmer.push_back(lookupSchwimmer(schwimmer, input));
					}
				}

				GesamtNotComputer gesamtNotComputer(schwimmer, eingesetzteSchwimmer);
				gesamtNotComputer.compute();
				outputResult(cout, gesamtNotComputer, flagPlain);
			}
			break;

		default:
			exitWithError("block `" + string(BLOCK_NAME_TABLE[valBlock]) + "' not supported yet");
			break;
		}
		break;

	default:
		exitWithError("class `" + string(CLASS_NAME_TABLE[valClass]) + "' not supported yet");
		break;
	}

	// Schwimmer in list freigeben
	for_each(schwimmer.begin(), schwimmer.end(), deleteSchwimmer);

	return EXIT_SUCCESS;
}
