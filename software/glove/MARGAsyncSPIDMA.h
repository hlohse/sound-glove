#ifndef MARGASYNCSPIDMA_H_
#define MARGASYNCSPIDMA_H_

#include "MARGAsyncSPI.h"

class MARGAsyncSPIDMA : public MARGAsyncSPI {
public:
	MARGAsyncSPIDMA(const MARGPins pins,
			       const uint8_t dma_channel,
				   const uint8_t dma_trigger_source,
				   const uint32_t dma_source_address);

	virtual void start(Readout::MARG* readout);
	virtual void progress();

protected:
	virtual void startReading(const Pin& cs,
					          const uint8_t register_address);
	virtual void reset();

private:
	const uint8_t dma_channel_;
	const uint8_t dma_trigger_source_;
	uint32_t address_;
};

#endif /* MARGASYNCSPIDMA_H_ */
