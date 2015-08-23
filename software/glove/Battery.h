#ifndef BATTERY_H_
#define BATTERY_H_

#include <stdint.h>

class Battery {
public:
	static Battery& instance();

	uint16_t charge() const;
	bool isUsed() const;

private:
	Battery();

    // Disallow copies; (do not implement!)
	Battery(const Battery&);
    void operator=(const Battery&);

};

#endif /* BATTERY_H_ */
