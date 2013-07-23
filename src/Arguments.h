
#ifndef ARGUMENTS_H_
#define ARGUMENTS_H_

#include <string>
#include "CLI.h"

using namespace std;

struct Arguments
{
	int flagDry;
	int flagInput;
	int flagPlain;
	int flagVerbose;
	Class valClass;
	Block valBlock;
	string valInput;
	string valParam;
	Arguments() :
		flagDry(0), flagInput(0), flagPlain(0), flagVerbose(0),
		valClass(DEFAULT_CLASS), valBlock(DEFAULT_BLOCK) { }
};

#endif /* ARGUMENTS_H_ */
