#include "MARGAsyncSPI.h"
#include "LSM9DS0.h"
#include "usci_b_spi.h"
#include "gpio.h"
#include <string.h>

MARGAsyncSPI::MARGAsyncSPI(const MARGPins& pins)
:	pins_(pins)
{
	reset();
}

void MARGAsyncSPI::start(Readout::MARG* readout)
{
	memset(readout->a.values, 0, sizeof(readout->a.values));
	memset(readout->m.values, 0, sizeof(readout->m.values));
	readout_ = readout;
	is_running_ = true;
	USCI_B_SPI_enableInterrupt(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT);
	current_sensor_ = M;
	startReading(pins_.cs_am, LSM9DS0_OUT_X_L_M);
}

void MARGAsyncSPI::startReading(const Pin& cs,
				               const uint8_t register_address)
{
    const uint8_t address = (register_address & 0x3F) | 0xC0; // 10XX XXXX = read, auto increment
    GPIO_setOutputLowOnPin(cs.port, cs.pin);
	USCI_B_SPI_transmitData(pins_.usci, address);
}

// 0GGGGGG0AAAAAAMMMMMMTT
void MARGAsyncSPI::progress()
{
	const uint8_t value = USCI_B_SPI_receiveData(pins_.usci);
	bytes_read_++;

	switch (current_sensor_) {
	case M: {
		if (bytes_read_ >= 2) { // Discard first byte of previous request
			((uint8_t*)readout_->m.values)[bytes_read_ - 2] = value;
		}
		if (bytes_read_ == 6) {
			startReading(pins_.cs_am, LSM9DS0_OUT_X_L_A);
		}
		else if (bytes_read_ == 7) {
			current_sensor_ = A;
			bytes_read_ = 0;
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
		else {
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	case A: {
		((uint8_t*)readout_->a.values)[bytes_read_ - 1] = value;
		if (bytes_read_ == 5) {
			startReading(pins_.cs_am, LSM9DS0_OUT_TEMP_L_XM);
		}
		else if (bytes_read_ == 6) {
			current_sensor_ = T;
			bytes_read_ = 0;
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
		else {
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	case T: {
		((uint8_t*)&readout_->t)[bytes_read_ - 1] = value;
		if (bytes_read_ == 2) {
			current_sensor_ = G;
			bytes_read_ = 0;
		    GPIO_setOutputHighOnPin(pins_.cs_am.port, pins_.cs_am.pin);
			startReading(pins_.cs_g, LSM9DS0_OUT_X_L_G);
		}
		else {
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	case G: {
		if (bytes_read_ >= 2) { // Discard first byte of previous request
			((uint8_t*)readout_->g.values)[bytes_read_ - 2] = value;
		}
		if (bytes_read_ == 7) {
			reset();
		}
		else {
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	default: break;
	}
}

void MARGAsyncSPI::reset()
{
    GPIO_setOutputHighOnPin(pins_.cs_g.port, pins_.cs_g.pin);
    GPIO_setOutputHighOnPin(pins_.cs_am.port, pins_.cs_am.pin);
	USCI_B_SPI_disableInterrupt(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT);
	current_sensor_ = M;
	bytes_read_ = 0;
	is_running_ = false;
}

bool MARGAsyncSPI::isRunning() const
{
	return is_running_;
}
