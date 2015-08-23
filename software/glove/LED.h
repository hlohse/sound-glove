#ifndef LED_H_
#define LED_H_

#include "Pins.h"
#include <stdint.h>

class LED {
public:
    LED(const Pin& pin, const bool is_on);
    LED();
    ~LED();

    bool isOn() const;

    void set(const bool is_on);

    void setOn();
    void setOff();

    void toggle();

private:
    Pin pin_;
    bool is_on_;
};

#endif /* LED_H_ */
