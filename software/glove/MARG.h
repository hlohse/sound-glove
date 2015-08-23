#ifndef MARG_H_
#define MARG_H_

#include "MARGBlockingSPI.h"
#include "MARGAsyncSPI.h"
#include "MARGConfiguration.h"
#include "Readout.h"
#include "Vector.h"
#include "Calibration.h"
#include "dma.h"
#include <stdint.h>

class MARG {
public:
	enum Placement {
    	Hand,
		Forearm,
		UpperArm
    };

    static const uint8_t dma_channel_hand      = 0xFF;
    static const uint8_t dma_channel_forearm = DMA_CHANNEL_1;
    static const uint8_t dma_channel_upper_arm = DMA_CHANNEL_0;
    static const uint8_t dma_trigger_source_hand      = 0;
    static const uint8_t dma_trigger_source_forearm = 22; // UCB1RXIFG @ channel 1
    static const uint8_t dma_trigger_source_upper_arm = 18; // UCB0RXIFG @ channel 0

    static MARGConfiguration& defaultConfiguration();
    static MARG& hand();
    static MARG& forearm();
    static MARG& upperArm();

    ~MARG();

    Calibration calibration() const;
    void setCalibration(Calibration& calibration);
    Vector<int16_t> determineMeanG(const int32_t fifos);
    Vector<int16_t> determineMeanA(const int32_t fifos);
    Vector<int16_t> determineMeanM(const int32_t fifos);

    bool isReady() const;

    void startReadingOut(Readout::MARG* readout);
    void progressReadingOut();
    bool isDoneReadingOut() const;

    bool hasNewG() const;
    Vector<int16_t> getG() const;

private:
    struct DMAConfiguration {
    	bool use;
    	uint8_t channel;
    	uint8_t trigger_source;
    };

    static const int32_t fifo_size = 32;

    const MARGBlockingSPI spi_blocking_;
    MARGAsyncSPI* spi_async_;
    const Placement placement_;
    const MARGPins pins_;

    MARG(const Placement placement,
    	const MARGPins& pins,
		const MARGConfiguration& marg_configuration,
		const DMAConfiguration& dma_configuration);
    void setImuConfiguration(const MARGConfiguration& marg_configuration);

    void powerUpG();
    void powerUpM();
    void powerUpT();

    void setGScale(const uint8_t g_scale);
    void setAScale(const uint8_t a_scale);
    void setMScale(const uint8_t m_scale);
    void setGODRCutoff(const uint8_t g_odr_cutoff);
    void setAODR(const uint8_t a_odr);
    void setAABF(const uint8_t a_abf);
    void setMODR(const uint8_t m_odr);
    void setMRES(const uint8_t m_res);

    Vector<int16_t> determineMean(const Pin& cs, const int register_address, const uint32_t fifos);
    void enableFifoG();
    void enableFifoAM();
    void disableFifoG();
    void disableFifoAM();
    void setFifoSamples(const Pin& cs, const uint8_t register_address);
    void setFifoBypass(const Pin& cs, const uint8_t register_address);
    uint8_t fifoSamples(const Pin& cs);

    // Disallow copies; (do not implement!)
    MARG(const MARG&);
    void operator=(const MARG&);
};

#endif /* MARG_H_ */
