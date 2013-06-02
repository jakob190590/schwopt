
#ifndef CLI_H_
#define CLI_H_

// Wettkampfbestimmungen (2)
enum Class {
	// Kinder
	MINI_MIXED = 0,
	// Jugend
	JUGEND_W,
	JUGEND_M,
	JUGEND_MIXED,
	// Offene Klassen
	DAMEN,
	HERREN,
	MIXED, // default
	// Anzahl?
	ANZAHL_CLASSES
};

Class defaultClass = MIXED;

char* classNameTable[ANZAHL_CLASSES] = {
	"mini-mixed",
	"jugend-w",
	"jugend-m",
	"jugend-mixed",
	"damen",
	"herren",
	"mixed"
};

enum Block {
	GESAMT = 0, // default
	LAGENSTAFFEL,
	SCHLUSSSTAFFEL,
	EINZELSTARTS,
	// bei MINI_MIXED ist das anders...
	// Anzahl?
	ANZAHL_BLOCKS
};

Block defaultBlock = GESAMT;

char* blockNameTable[ANZAHL_BLOCKS] = {
	"gesamt",
	"lagenstaffel",
	"schlussstaffel",
	"einzelstarts"
};

#endif /* CLI_H_ */
