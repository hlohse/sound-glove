#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Platform.h"
#include "ISerialPort.h"
#include "SharedPointer.h"
#include "Readout.h"
#ifdef PLATFORM_DESKTOP
#include "../desktop/soundglove/Glove.h"
#include <memory>
#endif

struct Context {
#ifdef MSP430
	ISerialPort* serial_port;
#else
	std::shared_ptr<SerialPort> serial_port;
	std::shared_ptr<Glove> glove;
#endif
	SharedPointer<Readout> readout_diff_ref;
};

#endif /* CONTEXT_H_ */
