/*
 * Zeit.h
 *
 *  Created on: 16.04.2013
 *      Author: jakob190590
 */

#ifndef ZEIT_H_
#define ZEIT_H_

#include <string>

using namespace std;

class Zeit {
public:
	static const int MAX_STRING_LENGTH_PLUS_ONE;

	static unsigned convertToUnsigned(const string& zeit);
	static string convertToString(unsigned zeit);
};

#endif /* ZEIT_H_ */
