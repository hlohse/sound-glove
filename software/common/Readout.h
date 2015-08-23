#ifndef READOUT_H_
#define READOUT_H_

#include "Export.h"
#include "Platform.h"
#include "Vector.h"
#include <stdint.h>

class Readout {
public:
	/* I'd love to put this as some nested Value class into MARG, but since the
	 * latter is not available on any desktop platform, the aggregated values
	 * class Readout::MARG is defined here. A little confusing.
	 */
    struct MARG {
        Vector<int16_t> g;
        Vector<int16_t> a;
        Vector<int16_t> m;
        int16_t t;
	};

	MARG margs[3];
	uint16_t flex[5];
    uint8_t is_touched;

    SOUNDGLOVE_API Readout();

    MARG handImu() const;
    MARG forearmImu() const;
    MARG upperArmImu() const;

#ifdef MSP430
    void capture();
#endif
};

#endif /* READOUT_H_ */
