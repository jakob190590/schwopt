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
	const int BUFFER_SIZE = 20;
	char result[BUFFER_SIZE];
	unsigned millis = zeit % 1000;
	unsigned seconds = (zeit / 1000) % 60;
	unsigned minutes = zeit / 1000 / 60;
	snprintf(result, BUFFER_SIZE, "%02i:%02i,%01i",
			minutes, seconds, millis / 100);
	return result;
}
