#include "MARGAsyncSPIDMA.h"
#include "LSM9DS0.h"
#include "usci_b_spi.h"
#include "gpio.h"
#include "dma.h"
#include <string.h>

MARGAsyncSPIDMA::MARGAsyncSPIDMA(const MARGPins pins,
			                   const uint8_t dma_channel,
				               const uint8_t dma_trigger_source,
				               const uint32_t dma_source_address)
:	MARGAsyncSPI(pins),
	dma_channel_(dma_channel),
	dma_trigger_source_(dma_trigger_source),
	address_(0)
{
	DMA_initializeParam dma_parameters = {
		dma_channel_,
		DMA_TRANSFER_REPEATED_SINGLE,
		1,
		dma_trigger_source_,
		DMA_SIZE_SRCBYTE_DSTBYTE,
		DMA_TRIGGER_HIGH
	};
	DMA_initialize(&dma_parameters);
	DMA_setSrcAddress(dma_channel_, dma_source_address, DMA_DIRECTION_UNCHANGED);
}

void MARGAsyncSPIDMA::start(Readout::MARG* readout)
{
	memset(readout->a.values, 0, sizeof(readout->a.values));
	memset(readout->m.values, 0, sizeof(readout->m.values));
	readout_ = readout;
	is_running_ = true;
	current_sensor_ = M;
	address_ = (uint32_t)readout_->m.values;
	DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
	DMA_enableInterrupt(dma_channel_);
	DMA_enableTransfers(dma_channel_);
	startReading(pins_.cs_am, LSM9DS0_OUT_X_L_M);
}

void MARGAsyncSPIDMA::startReading(const Pin& cs,
				                  const uint8_t register_address)
{
    const uint8_t address = (register_address & 0x3F) | 0xC0; // 10XX XXXX = read, auto increment
    GPIO_setOutputLowOnPin(cs.port, cs.pin);
	USCI_B_SPI_transmitData(pins_.usci, address);
}

// 0GGGGGG0AAAAAAMMMMMMTT
void MARGAsyncSPIDMA::progress()
{
	bytes_read_++;
	address_++;

	switch (current_sensor_) {
	case M: {
		if (bytes_read_ == 1) { // Discard first byte of previous request
			address_ = (uint32_t)readout_->m.values;
		}
		if (bytes_read_ == 6) {
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
			startReading(pins_.cs_am, LSM9DS0_OUT_X_L_A);
		}
		else if (bytes_read_ == 7) {
			current_sensor_ = A;
			bytes_read_ = 0;
			address_ = (uint32_t)readout_->a.values;
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_INCREMENT);
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
		else {
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	case A: {
		if (bytes_read_ == 5) {
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
			startReading(pins_.cs_am, LSM9DS0_OUT_TEMP_L_XM);
		}
		else if (bytes_read_ == 6) {
			current_sensor_ = T;
			bytes_read_ = 0;
			address_ = (uint32_t)&(readout_->t);
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_INCREMENT);
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
		else {
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	case T: {
		if (bytes_read_ == 2) {
			current_sensor_ = G;
			bytes_read_ = 0;
			address_ = (uint32_t)readout_->g.values;
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
		    GPIO_setOutputHighOnPin(pins_.cs_am.port, pins_.cs_am.pin);
			startReading(pins_.cs_g, LSM9DS0_OUT_X_L_G);
		}
		else {
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	case G: {
		if (bytes_read_ == 1) { // Discard first byte of previous request
			address_ = (uint32_t)readout_->g.values;
		}
		if (bytes_read_ == 7) {
			reset();
		}
		else {
			DMA_setDstAddress(dma_channel_, address_, DMA_DIRECTION_UNCHANGED);
			USCI_B_SPI_transmitData(pins_.usci, 0);
		}
	} break;
	default: break;
	}
}

void MARGAsyncSPIDMA::reset()
{
    GPIO_setOutputHighOnPin(pins_.cs_g.port, pins_.cs_g.pin);
    GPIO_setOutputHighOnPin(pins_.cs_am.port, pins_.cs_am.pin);
	DMA_disableInterrupt(dma_channel_);
	DMA_disableTransfers(dma_channel_);
	current_sensor_ = M;
	bytes_read_ = 0;
	is_running_ = false;

	for (int i = 0; i < 3; ++i) {
		readout_->m.values[i] = __swap_bytes(readout_->m.values[i]);
		readout_->a.values[i] = __swap_bytes(readout_->a.values[i]);
		readout_->g.values[i] = __swap_bytes(readout_->g.values[i]);
	}
	readout_->t = __swap_bytes(readout_->t);
}
