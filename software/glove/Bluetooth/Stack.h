#ifndef BLUETOOTH_STACK_H_
#define BLUETOOTH_STACK_H_

#include "System.h"
#include "HCIAPI.h"

namespace Bluetooth {

// Uses TIMER1_A0 for ACLK tick count
class Stack {
public:
    static const unsigned int uninitialized_id = 0;
    static const int uninitialized_callback_id = 0;
    static const int debug_message_size = 64;
#ifdef SYSTEM_BENCHMARK_BLUETOOTH
    static const int tick_rate_hz = 0xFFFF; // => 32kHz
#else
    static const int tick_rate_hz = 1000;   // => ~1kHz
#endif

    static volatile unsigned long tick_count;

    static Stack& instance();
    ~Stack();

    bool isReady() const;

    unsigned int id() const;
    HCI_Version_t hciVersion() const;
    char* debugMessage();

private:
    unsigned int id_;
    int callback_id_;
    HCI_Version_t hci_version_;
    char debug_message_[debug_message_size];

    // Sets Error
    Stack();

    void enableLogicLevelShifter() const;
    void configureTickCountTimer() const;
    static unsigned long getTickCountCallback();
    static void messageOutputCallback(char c);

    void handleHciEvent(const HCI_Event_Data_t& hci_event_data);
    static void BTPSAPI hciEventCallback(unsigned int stack_id,
                                         HCI_Event_Data_t* hci_event_data,
                                         unsigned long callback_parameter);

    // Disallow copies; (do not implement!)
    Stack(const Stack&);
    void operator=(const Stack&);
};

}

#endif /* BLUETOOTH_STACK_H_ */
