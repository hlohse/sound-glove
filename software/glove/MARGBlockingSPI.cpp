#include "MARGBlockingSPI.h"
#include "Error.h"
#include "usci_b_spi.h"
#include "gpio.h"

MARGBlockingSPI::MARGBlockingSPI(const MARGPins& pins)
:	pins_(pins)
{
}

void MARGBlockingSPI::updateRegister(const Pin& cs,
                                    const uint8_t register_address,
                                    const uint8_t value,
                                    const uint8_t lsb_index,
                                    const uint8_t length) const
{
    uint8_t temp = readRegister(cs, register_address);
    temp &= 0xFF^(((1 << length) - 1) << lsb_index);  // Mask out current value bits
    temp |= value << lsb_index; // Shift in new value bits
    writeRegister(cs, register_address, temp);
}

uint8_t MARGBlockingSPI::readRegister(const Pin& cs,
							         const uint8_t register_address) const
{
    uint8_t value = 0;
    const uint8_t address = (register_address & 0x3F) | 0x80; // 10XX XXXX = read, no increment
    GPIO_setOutputLowOnPin(cs.port, cs.pin);

    // Write twice to receive answered data during second write
    for (int i = 0; i < 2; ++i) {
		while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT));
		USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT);
		USCI_B_SPI_transmitData(pins_.usci, address);
    }

    while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT));
    USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT);
    value = USCI_B_SPI_receiveData(pins_.usci);

    GPIO_setOutputHighOnPin(cs.port, cs.pin);
    return value;
}

void MARGBlockingSPI::readRegisters(const Pin& cs,
						           const uint8_t register_address_start,
						           uint8_t* buffer,
						           const int num_bytes) const
{
    const uint8_t address = (register_address_start & 0x3F) | 0xC0; // 11XX XXXX = read, increment
    GPIO_setOutputLowOnPin(cs.port, cs.pin);

    while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT));
    USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT);
    USCI_B_SPI_transmitData(pins_.usci, address);

    for (int i = 0; i < num_bytes; ++i) {
        // Write dummy data to receive slave value
        while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT));
        USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT);
        USCI_B_SPI_transmitData(pins_.usci, 0);

        while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT));
        USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT);
        buffer[i] = USCI_B_SPI_receiveData(pins_.usci);
    }

    GPIO_setOutputHighOnPin(cs.port, cs.pin);
}

void MARGBlockingSPI::writeRegister(const Pin& cs,
						           const uint8_t register_address,
						           const uint8_t value) const
{
    const uint8_t address = register_address & 0x3F; // 00XX XXXX = write, no increment
    GPIO_setOutputLowOnPin(cs.port, cs.pin);

    while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT));
    USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT);
    USCI_B_SPI_transmitData(pins_.usci, address);

    while (!USCI_B_SPI_getInterruptStatus(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT));
    USCI_B_SPI_clearInterruptFlag(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT);
    USCI_B_SPI_transmitData(pins_.usci, value);

    GPIO_setOutputHighOnPin(cs.port, cs.pin);
}
