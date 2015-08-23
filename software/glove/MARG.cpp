#include "MARG.h"
#include "System.h"
#include "LSM9DS0.h"
#include "SubsystemClock.h"
#include "Configuration.h"
#include "MARGAsyncSPIDMA.h"
#include "usci_b_spi.h"
#include "gpio.h"

MARGConfiguration& MARG::defaultConfiguration()
{
    static MARGConfiguration instance;
    return instance;
}

MARG& MARG::hand()
{
    static MARGPins pins_configuration = {
        PIN_MARG_HAND_USCI,
		Pin(PIN_MARG_HAND_CLK_PORT,   PIN_MARG_HAND_CLK_PIN),
		Pin(PIN_MARG_HAND_SOMI_PORT,  PIN_MARG_HAND_SOMI_PIN),
		Pin(PIN_MARG_HAND_SIMO_PORT,  PIN_MARG_HAND_SIMO_PIN),
        Pin(PIN_MARG_HAND_CS_G_PORT,  PIN_MARG_HAND_CS_G_PIN),
        Pin(PIN_MARG_HAND_CS_AM_PORT, PIN_MARG_HAND_CS_AM_PIN)
    };
    static DMAConfiguration dma_configuration = {
    	false,
		dma_channel_hand,
		dma_trigger_source_hand
    };
    static MARG instance(Hand, pins_configuration, MARG::defaultConfiguration(), dma_configuration);
    return instance;
}

MARG& MARG::forearm()
{
    static MARGPins pins_configuration = {
        PIN_MARG_LOWER_ARM_USCI,
		Pin(PIN_MARG_LOWER_ARM_CLK_PORT,   PIN_MARG_LOWER_ARM_CLK_PIN),
		Pin(PIN_MARG_LOWER_ARM_SOMI_PORT,  PIN_MARG_LOWER_ARM_SOMI_PIN),
		Pin(PIN_MARG_LOWER_ARM_SIMO_PORT,  PIN_MARG_LOWER_ARM_SIMO_PIN),
        Pin(PIN_MARG_LOWER_ARM_CS_G_PORT,  PIN_MARG_LOWER_ARM_CS_G_PIN),
        Pin(PIN_MARG_LOWER_ARM_CS_AM_PORT, PIN_MARG_LOWER_ARM_CS_AM_PIN)
    };
    static DMAConfiguration dma_configuration = {
    	true,
		dma_channel_forearm,
		dma_trigger_source_forearm
    };
    static MARG instance(Forearm, pins_configuration, MARG::defaultConfiguration(), dma_configuration);
    return instance;
}

MARG& MARG::upperArm()
{
    static MARGPins pins_configuration = {
        PIN_MARG_UPPER_ARM_USCI,
		Pin(PIN_MARG_UPPER_ARM_CLK_PORT,   PIN_MARG_UPPER_ARM_CLK_PIN),
		Pin(PIN_MARG_UPPER_ARM_SOMI_PORT,  PIN_MARG_UPPER_ARM_SOMI_PIN),
		Pin(PIN_MARG_UPPER_ARM_SIMO_PORT,  PIN_MARG_UPPER_ARM_SIMO_PIN),
        Pin(PIN_MARG_UPPER_ARM_CS_G_PORT,  PIN_MARG_UPPER_ARM_CS_G_PIN),
        Pin(PIN_MARG_UPPER_ARM_CS_AM_PORT, PIN_MARG_UPPER_ARM_CS_AM_PIN)
    };
    static DMAConfiguration dma_configuration = {
    	true,
		dma_channel_upper_arm,
		dma_trigger_source_upper_arm
    };
    static MARG instance(UpperArm, pins_configuration, MARG::defaultConfiguration(), dma_configuration);
    return instance;
}

MARG::MARG(const Placement placement,
		 const MARGPins& pins,
		 const MARGConfiguration& marg_configuration,
		 const DMAConfiguration& dma_configuration)
:   spi_blocking_(MARGBlockingSPI(pins)),
	placement_(placement),
	pins_(pins)
{
#ifdef SYSTEM_SPI_ASYNC_WITH_DMA
	if (dma_configuration.use) {
		spi_async_ = new MARGAsyncSPIDMA(pins,
				                        dma_configuration.channel,
										dma_configuration.trigger_source,
										USCI_B_SPI_getReceiveBufferAddressForDMA(pins.usci));
	}
	else {
		spi_async_ = new MARGAsyncSPI(pins);
	}
#else
	spi_async_ = new MARGAsyncSPI(pins);
#endif

	// Deactivate chip selects
    GPIO_setAsOutputPin(pins_.cs_g.port, pins_.cs_g.pin);
    GPIO_setAsOutputPin(pins_.cs_am.port, pins_.cs_am.pin);
    GPIO_setOutputHighOnPin(pins_.cs_g.port, pins_.cs_g.pin);
    GPIO_setOutputHighOnPin(pins_.cs_am.port, pins_.cs_am.pin);
    // Activate USCI pin functionality
    GPIO_setAsPeripheralModuleFunctionOutputPin(pins_.clk.port, pins_.clk.pin);
    GPIO_setAsPeripheralModuleFunctionOutputPin(pins_.simo.port, pins_.simo.pin);
    GPIO_setAsInputPinWithPullDownResistor(pins_.somi.port, pins_.somi.pin);
    GPIO_setAsPeripheralModuleFunctionInputPin(pins_.somi.port, pins_.somi.pin);

	USCI_B_SPI_initMasterParam parameters = {
		USCI_B_SPI_CLOCKSOURCE_SMCLK,
		SubsystemClock::instance().frequency(),
		SubsystemClock::instance().frequency(),
        USCI_B_SPI_MSB_FIRST,
		USCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,
		USCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW
    };

    if (USCI_B_SPI_initMaster(pins_.usci, &parameters) != STATUS_SUCCESS) {
    	Error::instance().set(Error::TYPE_MARG_INIT, pins_.usci);
    }

    USCI_B_SPI_enable(pins_.usci);
    USCI_B_SPI_disableInterrupt(pins_.usci, USCI_B_SPI_TRANSMIT_INTERRUPT);
    USCI_B_SPI_disableInterrupt(pins_.usci, USCI_B_SPI_RECEIVE_INTERRUPT);

    setImuConfiguration(marg_configuration);
}

MARG::~MARG()
{
	delete spi_async_;
}

void MARG::setImuConfiguration(const MARGConfiguration& marg_configuration)
{
	if (spi_async_->isRunning()) {
		Error::instance().set(Error::TYPE_MARG_BLOCK_ASYNC, Error::no_info);
	}

	if (isReady()) {
		disableFifoG();
		disableFifoAM();

		powerUpG();
		powerUpM();
		powerUpT();

		setGScale(marg_configuration.g_scale);
		setAScale(marg_configuration.a_scale);
		setMScale(marg_configuration.m_scale);
		setGODRCutoff(marg_configuration.g_odr_cutoff);
		setAODR(marg_configuration.a_odr);
		setAABF(marg_configuration.a_abf);
		setMODR(marg_configuration.m_odr);
		setMRES(marg_configuration.m_res);
	}
}

bool MARG::isReady() const
{
	const int who_am_i_g  = spi_blocking_.readRegister(pins_.cs_g,  LSM9DS0_WHO_AM_I_G);
	const int who_am_i_am = spi_blocking_.readRegister(pins_.cs_am, LSM9DS0_WHO_AM_I_XM);
	if (who_am_i_g != LSM9DS0_WHO_AM_I_G_VAL) {
		Error::instance().set(Error::TYPE_MARG_WHO_AM_I_G, who_am_i_g);
		return false;
	}
	if (who_am_i_am != LSM9DS0_WHO_AM_I_XM_VAL) {
		Error::instance().set(Error::TYPE_MARG_WHO_AM_I_AM, who_am_i_am);
		return false;
	}
	return true;
}

void MARG::powerUpG()
{
	// Set normal mode
	spi_blocking_.updateRegister(pins_.cs_g, LSM9DS0_CTRL_REG1_G, 1, 3, 1);
}

void MARG::powerUpM()
{
	// Set continuous-conversion mode
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG7_XM, 0, 0, 2);
}

void MARG::powerUpT()
{
	// Enable
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG5_XM, 1, 7, 1);
}

void MARG::setGScale(const uint8_t g_scale)
{
	spi_blocking_.updateRegister(pins_.cs_g, LSM9DS0_CTRL_REG4_G, g_scale, 4, 2);
}

void MARG::setAScale(const uint8_t a_scale)
{
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG2_XM, a_scale, 3, 3);
}

void MARG::setMScale(const uint8_t m_scale)
{
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG6_XM, m_scale, 5, 2);
}

void MARG::setGODRCutoff(const uint8_t g_odr_cutoff)
{
	spi_blocking_.updateRegister(pins_.cs_g, LSM9DS0_CTRL_REG1_G, g_odr_cutoff, 4, 4);
}

void MARG::setAODR(const uint8_t a_odr)
{
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG1_XM, a_odr, 4, 4);
}

void MARG::setAABF(const uint8_t a_abf)
{
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG2_XM, a_abf, 6, 2);
}

void MARG::setMODR(const uint8_t m_odr)
{
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG5_XM, m_odr, 2, 3);
}

void MARG::setMRES(const uint8_t m_res)
{
	spi_blocking_.updateRegister(pins_.cs_am, LSM9DS0_CTRL_REG5_XM, m_res, 5, 2);
}

Calibration MARG::calibration() const
{
	return Configuration::instance().margCalibration(placement_);
}

void MARG::setCalibration(Calibration& calibration)
{
	Configuration::instance().setImuCalibration(placement_, calibration);
}

Vector<int16_t> MARG::determineMeanG(const int32_t fifos)
{
	return determineMean(pins_.cs_g, LSM9DS0_OUT_X_L_G, fifos);
}

Vector<int16_t> MARG::determineMeanA(const int32_t fifos)
{
	return determineMean(pins_.cs_am, LSM9DS0_OUT_X_L_A, fifos);
}

Vector<int16_t> MARG::determineMeanM(const int32_t fifos)
{
	return determineMean(pins_.cs_am, LSM9DS0_OUT_X_L_M, fifos);
}

Vector<int16_t> MARG::determineMean(const Pin& cs, const int register_address, const uint32_t fifos)
{
	Vector<int16_t> mean(0, 0, 0);
	const int64_t samples = (int64_t)(fifos * fifo_size);

	if (spi_async_->isRunning()) {
		Error::instance().set(Error::TYPE_MARG_BLOCK_ASYNC, Error::no_info);
	}

	if (&cs == &pins_.cs_g) {
		enableFifoG();
	}
	else {
		enableFifoAM();
	}

	if (isReady()) {
		int16_t temp_readout[3] = {0, 0, 0};
		int64_t readout_sum[3] = {0, 0, 0};

		// Skip first FIFO. Might contain garbage values. (Why?)
		while (fifoSamples(cs) < fifo_size);
		for (int i = 0; i < fifo_size; ++i) {
			spi_blocking_.readRegisters(cs, register_address, (uint8_t*) temp_readout, 6);
		}

		for (int fifo = 0; fifo < fifos; ++fifo) {
			while (fifoSamples(cs) < fifo_size);
			for (int i = 0; i < fifo_size; ++i) {
				spi_blocking_.readRegisters(cs, register_address, (uint8_t*) temp_readout, 6);
				readout_sum[0] += temp_readout[0];
				readout_sum[1] += temp_readout[1];
				readout_sum[2] += temp_readout[2];
			}
		}
		mean.setX(readout_sum[0] / samples);
		mean.setY(readout_sum[1] / samples);
		mean.setZ(readout_sum[2] / samples);
	}
	else {
		Error::instance().set(Error::TYPE_MARG_MEAN, Error::no_info);
	}

	if (&cs == &pins_.cs_g) {
		disableFifoG();
	}
	else {
		disableFifoAM();
	}

	return mean;
}

void MARG::enableFifoG()
{
	setFifoSamples(pins_.cs_g, LSM9DS0_CTRL_REG5_G);
}

void MARG::enableFifoAM()
{
	setFifoSamples(pins_.cs_am, LSM9DS0_CTRL_REG0_XM);
}

void MARG::disableFifoG()
{
	setFifoBypass(pins_.cs_g, LSM9DS0_CTRL_REG5_G);
}

void MARG::disableFifoAM()
{
	setFifoBypass(pins_.cs_am, LSM9DS0_CTRL_REG0_XM);
}

void MARG::setFifoSamples(const Pin& cs, const uint8_t register_address)
{
	const uint8_t enable = 0x20; // 00[1]0 0000
	const uint8_t value = enable | fifo_size;
	spi_blocking_.updateRegister(cs, register_address, 1, 6, 1); // FIFO_EN
	spi_blocking_.updateRegister(cs, LSM9DS0_FIFO_CTRL_REG, value, 0, 8); // FIFO mode
}

void MARG::setFifoBypass(const Pin& cs, const uint8_t register_address)
{
	spi_blocking_.updateRegister(cs, register_address, 0, 6, 1); // !FIFO_EN
	spi_blocking_.updateRegister(cs, LSM9DS0_FIFO_CTRL_REG, 0, 0, 8); // !FIFO mode
}

uint8_t MARG::fifoSamples(const Pin& cs)
{
	return spi_blocking_.readRegister(cs, LSM9DS0_FIFO_SRC_REG);
}

void MARG::startReadingOut(Readout::MARG* readout)
{
#if defined(SYSTEM_SPI_ASYNC) || defined(SYSTEM_SPI_ASYNC_WITH_DMA)
	spi_async_->start(readout);
#else
	spi_blocking_.readRegisters(pins_.cs_g,  LSM9DS0_OUT_X_L_G, (uint8_t*)readout->g.values, 6);
	spi_blocking_.readRegisters(pins_.cs_am, LSM9DS0_OUT_X_L_A, (uint8_t*)readout->a.values, 6);
	spi_blocking_.readRegisters(pins_.cs_am, LSM9DS0_OUT_X_L_M, (uint8_t*)readout->m.values, 6);
	spi_blocking_.readRegisters(pins_.cs_am, LSM9DS0_OUT_TEMP_L_XM, (uint8_t*)&readout->t, 2);
#endif
}

void MARG::progressReadingOut()
{
#if defined(SYSTEM_SPI_ASYNC) || defined(SYSTEM_SPI_ASYNC_WITH_DMA)
	spi_async_->progress();
#endif
}

bool MARG::isDoneReadingOut() const
{
#if defined(SYSTEM_SPI_ASYNC) || defined(SYSTEM_SPI_ASYNC_WITH_DMA)
	return !spi_async_->isRunning();
#else
	return true;
#endif
}

bool MARG::hasNewG() const
{
	const uint8_t status = spi_blocking_.readRegister(pins_.cs_g, LSM9DS0_STATUS_REG_G);
	const int has_new_xyz_mask = 0x08; // 0000 1000
	return status & has_new_xyz_mask != 0;
}

Vector<int16_t> MARG::getG() const
{
	Vector<int16_t> readout;
	spi_blocking_.readRegisters(pins_.cs_g,
			                    LSM9DS0_OUT_X_L_G,
								(uint8_t*)readout.values,
								sizeof(readout.values));
	return readout;
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR()
{
	USCI_B_SPI_clearInterruptFlag(USCI_B0_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT | USCI_B_SPI_RECEIVE_INTERRUPT);
	__enable_interrupt();
    MARG::upperArm().progressReadingOut();
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR()
{
	USCI_B_SPI_clearInterruptFlag(USCI_B1_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT | USCI_B_SPI_RECEIVE_INTERRUPT);
	__enable_interrupt();
    MARG::forearm().progressReadingOut();
}

#pragma vector=USCI_B3_VECTOR
__interrupt void USCI_B3_ISR()
{
	USCI_B_SPI_clearInterruptFlag(USCI_B3_BASE, USCI_B_SPI_TRANSMIT_INTERRUPT | USCI_B_SPI_RECEIVE_INTERRUPT);
	__enable_interrupt();
    MARG::hand().progressReadingOut();
}

#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR()
{
	if (DMA_getInterruptStatus(MARG::dma_channel_upper_arm) == DMA_INT_ACTIVE) {
		DMA_clearInterrupt(MARG::dma_channel_upper_arm);
		__enable_interrupt();
	    MARG::upperArm().progressReadingOut();
	}
	else if (DMA_getInterruptStatus(MARG::dma_channel_forearm) == DMA_INT_ACTIVE) {
		DMA_clearInterrupt(MARG::dma_channel_forearm);
		__enable_interrupt();
	    MARG::forearm().progressReadingOut();
	}
}
