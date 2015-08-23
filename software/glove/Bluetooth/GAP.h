#ifndef BLUETOOTH_GAP_H_
#define BLUETOOTH_GAP_H_

#include "Error.h"
#include "GAPAPI.h"

namespace Bluetooth {

class GAP {
public:
    static GAP& instance();
    ~GAP();

    void setAllowsPairing(const bool state);

    void handle(const GAP_Event_Data_t& event);

private:
    bool allows_pairing_;

    GAP();

    void handleAuthentication(const GAP_Authentication_Event_Data_t& event);
    void sendIoCapabilities(const GAP_Authentication_Event_Data_t& event) const;
    void sendLinkKey(const GAP_Authentication_Event_Data_t& event);
    void createLinkKey(const GAP_Authentication_Event_Data_t& event);
    void confirmPairing(const GAP_Authentication_Event_Data_t& event);
    //void finishSecureSimplePairing(const GAP_Authentication_Event_Data_t& event);
    void sendAuthenticationResponse(GAP_Authentication_Information_t& response,
                                    const GAP_Authentication_Event_Data_t& event,
                                    const Error::Type error_type) const;

    static void BTPSAPI gapEventCallback(unsigned int stack_id,
                                         GAP_Event_Data_t* gap_event_data,
                                         unsigned long callback_parameter);

    // Disallow copies; (do not implement!)
    GAP(const GAP&);
    void operator=(const GAP&);
};

}

#endif /* BLUETOOTH_GAP_H_ */
