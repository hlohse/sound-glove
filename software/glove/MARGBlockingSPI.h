#ifndef MARGBLOCKINGSPI_H_
#define MARGBLOCKINGSPI_H_

#include "MARGPins.h"

class MARGBlockingSPI {
public:
	MARGBlockingSPI(const MARGPins& pins);

	void updateRegister(const Pin& cs,
	                    const uint8_t register_address,
	                    const uint8_t value,
	                    const uint8_t lsb_index,
	                    const uint8_t length) const;

	uint8_t readRegister(const Pin& cs,
			             const uint8_t register_address) const;
	void readRegisters(const Pin& cs,
	                   const uint8_t register_address_start,
	                   uint8_t* buffer,
	                   const int num_bytes) const;
	void writeRegister(const Pin& cs,
			           const uint8_t register_address,
					   const uint8_t value) const;

private:
	const MARGPins pins_;
};

#endif /* MARGBLOCKINGSPI_H_ */
