/*
 * Zeit.h
 *
 *  Created on: 16.04.2013
 *      Author: jakob190590
 */

#ifndef ZEIT_H_
#define ZEIT_H_

#include <string>
#include <climits>

using namespace std;

class Zeit {
public:
	static const unsigned MAX_UNSIGNED_VALUE = UINT_MAX;
	static const int MAX_STRING_LENGTH = 7; // 00:00,0  also auf Zehntelsekunden genau

	static unsigned convertToUnsigned(const string& zeit);
	static string convertToString(unsigned zeit);
};

#endif /* ZEIT_H_ */
