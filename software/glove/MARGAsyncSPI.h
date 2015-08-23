#ifndef MARGASYNCSPI_H_
#define MARGASYNCSPI_H_

#include "MARGPins.h"
#include "Readout.h"
#include "Vector.h"

class MARGAsyncSPI {
public:
	MARGAsyncSPI(const MARGPins& pins);

	virtual void start(Readout::MARG* readout);
	virtual void progress();
	bool isRunning() const;

protected:
	enum Sensor {
		G,
		A,
		M,
		T
	};

	const MARGPins pins_;
	volatile Readout::MARG* readout_;
	volatile Sensor current_sensor_;
	volatile int bytes_read_;
	volatile bool is_running_;

	virtual void startReading(const Pin& cs,
					          const uint8_t register_address);
	virtual void reset();
};

#endif /* MARGASYNCSPI_H_ */
