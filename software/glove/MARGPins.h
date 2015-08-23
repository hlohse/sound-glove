#ifndef MARGPINS_H_
#define MARGPINS_H_

#include "Pins.h"

struct MARGPins {
	uint16_t usci;
	Pin clk;
	Pin somi;
	Pin simo;
	Pin cs_g;
	Pin cs_am;
};

#endif /* MARGPINS_H_ */
