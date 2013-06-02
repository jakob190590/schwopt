
#ifndef SCHWOPTALGO_H_
#define SCHWOPTALGO_H_

#include <map>
#include <cassert>

#include "../Schwimmer.h"
#include "../Zeit.h"

// Zuordnung: Position -> Disziplin
typedef vector<int> PositionDisziplinTable;

// Schwimmer, der auf einer bestimmten Position eingesetzt ist
typedef pair<int, Schwimmer*> PositionSchwimmerPair;
typedef list<PositionSchwimmerPair> PositionSchwimmerPairList;

// Map: Schwimmer -> int, z. B. verwendet in ensureMax3Bedingung (jeder Schwimmer maximal drei mal)
typedef map<Schwimmer*, int> SchwimmerIntMap;

// Abstaende von Schwimmern zum jeweils Naechstschlechteren in einer Disziplin
typedef map<Schwimmer*, unsigned> SchwimmerAbstandMap;
typedef vector<SchwimmerAbstandMap> SchwimmerAbstandMapVector;

// vector mit Eintrag fuer jede Disziplin: set: Schwimmer -> Abstand zum Naechstschlechteren
SchwimmerAbstandMapVector createAbstandsMap(const SchwimmerListVector& schwimmerSortiert);

#endif /* SCHWOPTALGO_H_ */
