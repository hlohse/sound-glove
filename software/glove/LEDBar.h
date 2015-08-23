#ifndef LEDBAR_H_
#define LEDBAR_H_

#include "LED.h"

template <uint8_t leds>
class LEDBar {
public:
    static const int max_display_decimal = (2 << (leds - 1)) - 1;

LEDBar(const Pin pins[leds])
{
    for (uint8_t i = 0; i < leds; ++i) {
        leds_[i] = LED(pins[i], false);
    }
}

~LEDBar()
{
}

void displayBinary(const uint8_t decimal)
{
    const uint8_t capped_decimal = decimal > max_display_decimal ?
        max_display_decimal : decimal;
    for (int i = 0; i < leds; ++i) {
        leds_[i].set((capped_decimal & (1 << i)) != 0);
    }
}

void displayLEDs(const bool is_on[leds])
{
    for (uint8_t i = 0; i < leds; ++i) {
        leds_[i].set(is_on[i]);
    }
}

private:
    LED leds_[leds];
};

#endif /* LEDBAR_H_ */
