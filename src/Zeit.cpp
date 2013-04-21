/*
 * Zeit.cpp
 *
 *  Created on: 16.04.2013
 *      Author: jakob190590
 */

#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "Zeit.h"

using namespace std;

const int Zeit::MAX_STRING_LENGTH_PLUS_ONE = 8; // 00:00,0  also auf Zehntelsekunden genau

unsigned Zeit::convertToUnsigned(const string& zeit)
{
	// zeit ::= [ ZAHL ] COLON [ ZAHL [ COMMA [ ZAHL ] ] ]
	//           minute         second         zehntel

	unsigned minutes, seconds, zehntel;
	stringstream stream(zeit);

	string word;
	getline(stream, word, ':');
	minutes = atoi(word.c_str());

	getline(stream, word, ',');
	seconds = atoi(word.c_str());

	word = "0"; // Neuinitialisierung
	getline(stream, word); // ... falls stream aus ist
	zehntel = atoi(word.substr(0, 1).c_str()); // substr, falls mehrere stellen angegeben sind

	return (minutes * 60 + seconds) * 1000 + zehntel * 100;
}

string Zeit::convertToString(unsigned zeit)
{
	char result[MAX_STRING_LENGTH_PLUS_ONE];
	unsigned millis = zeit % 1000;
	unsigned seconds = (zeit / 1000) % 60;
	unsigned minutes = zeit / 1000 / 60;
	snprintf(result, MAX_STRING_LENGTH_PLUS_ONE, "%02i:%02i,%01i",
			minutes, seconds, millis / 100);
	return result;
}
