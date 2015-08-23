#ifndef BLUETOOTH_SPP_H_
#define BLUETOOTH_SPP_H_

#include "SPPAPI.h"
#include "Context.h"
#include <stdlib.h>

namespace Bluetooth {

// http://e2e.ti.com/support/wireless_connectivity/f/660/p/137571/513247#513247

class SPP {
public:
    static SPP& instance(Context* context = ((Context*) NULL));
    ~SPP();

    int id() const;

    bool isOpen() const;
    bool isConnected() const;

    void enterSniffMode();
    void exitSniffMode();

private:
    static const int invalid_id = 0;
    static const Word_t invalid_connection_handle = 0;
    static const unsigned long invalid_sdp_handle = 0;
    static const int sniff_max_interval = 16; // 16 * 625us = 10ms
    static const int sniff_min_interval = 14; // 14 * 625us = 8.75ms
    static const int sniff_attempt      = 1;
    static const int sniff_timeout      = 2;

    int id_;
    unsigned long sdp_handle_;
    Word_t connection_handle_;
    Context* context_;
    bool is_in_sniff_mode_;

    SPP(Context* context);

    void handle(const SPP_Event_Data_t& event);
    void tryConnect(const SPP_Open_Port_Indication_Data_t& event);
    void connected(const SPP_Open_Port_Indication_Data_t& event);
    void disconnected();
    void sendPortInfo(const SPP_Send_Port_Information_Indication_Data_t& event);

    static void BTPSAPI sppEventCallback(unsigned int stack_id,
                                         SPP_Event_Data_t* spp_event_data,
                                         unsigned long callback_parameter);

    // Disallow copies; (do not implement!)
    SPP(const SPP&);
    void operator=(const SPP&);
};

}

#endif /* BLUETOOTH_SPP_H_ */
