#include "Bluetooth\GAP.h"
#include "Bluetooth\Stack.h"
#include "Bluetooth\Bond.h"
#include "Configuration.h"
#include <string.h>
using namespace Bluetooth;

GAP& GAP::instance()
{
    static GAP instance;
    return instance;
}

GAP::GAP()
:   allows_pairing_(true)
{
    int status = 0;

    status = GAP_Register_Remote_Authentication(Stack::instance().id(), gapEventCallback, 0);
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_GAP_REG_AUTH, status);
    }

    status = GAP_Set_Local_Device_Name(Stack::instance().id(),
    								   Configuration::instance().name());
    if (status != 0) {
        Error::instance().set(Error::TYPE_BT_GAP_REG_NAME, status);
    }
}

GAP::~GAP()
{
    GAP_Un_Register_Remote_Authentication(Stack::instance().id());
}

void GAP::setAllowsPairing(const bool state)
{
    allows_pairing_ = state;
}

void GAP::handle(const GAP_Event_Data_t& event)
{
    switch (event.Event_Data_Type) {
    case etAuthentication:
        handleAuthentication(*event.Event_Data.GAP_Authentication_Event_Data);
        break;
    default:
        break;
    }
}

void GAP::handleAuthentication(const GAP_Authentication_Event_Data_t& event)
{
    switch (event.GAP_Authentication_Event_Type) {
    case atIOCapabilityRequest:         sendIoCapabilities(event);        break;
    case atLinkKeyRequest:              sendLinkKey(event);               break;
    case atLinkKeyCreation:             createLinkKey(event);             break;
    case atUserConfirmationRequest:     confirmPairing(event);            break;
    //case atSecureSimplePairingComplete: finishSecureSimplePairing(event); break;
    default: break;
    }
}

void GAP::sendIoCapabilities(const GAP_Authentication_Event_Data_t& event) const
{
    GAP_Authentication_Information_t response;

    response.GAP_Authentication_Type = atIOCapabilities;
    response.Authentication_Data_Length = (Byte_t) sizeof(GAP_IO_Capabilities_t);
    response.Authentication_Data.IO_Capabilities.IO_Capability = icNoInputNoOutput;
    response.Authentication_Data.IO_Capabilities.MITM_Protection_Required = FALSE;
    response.Authentication_Data.IO_Capabilities.OOB_Data_Present = FALSE;
    // Requires "Add device..."-like pairing prior to usage
    response.Authentication_Data.IO_Capabilities.Bonding_Type = ibDedicatedBonding;

    sendAuthenticationResponse(response, event, Error::TYPE_BT_GAP_IO_CAP);
}

void GAP::sendLinkKey(const GAP_Authentication_Event_Data_t& event)
{
    GAP_Authentication_Information_t response;
    response.GAP_Authentication_Type = atLinkKey;

    // Use Bond's link key if available; null link key otherwise
    if (Bond::instance().hasRemoteAddress(event.Remote_Device)) {
        response.Authentication_Data_Length = (Byte_t) sizeof(Bond::instance().linkKey());
        response.Authentication_Data.Link_Key = Bond::instance().linkKey();
    }
    else {
        response.Authentication_Data_Length = 0;
        memset(&response.Authentication_Data.Link_Key, 0, sizeof(response.Authentication_Data.Link_Key));
    }

    sendAuthenticationResponse(response, event, Error::TYPE_BT_GAP_LINK_SEND);
}

void GAP::createLinkKey(const GAP_Authentication_Event_Data_t& event)
{
    Bond::instance().setRemoteAddress(event.Remote_Device);
    Bond::instance().setLinkKey(event.Authentication_Event_Data.Link_Key_Info.Link_Key);
}

void GAP::confirmPairing(const GAP_Authentication_Event_Data_t& event)
{
    GAP_Authentication_Information_t response;
    response.GAP_Authentication_Type = atUserConfirmation;
    response.Authentication_Data_Length = (Byte_t) sizeof(Byte_t);
    response.Authentication_Data.Confirmation = allows_pairing_ ? TRUE : FALSE;
    sendAuthenticationResponse(response, event, Error::TYPE_BT_GAP_USER_CONF);
}

/*
void GAP::finishSecureSimplePairing(const GAP_Authentication_Event_Data_t& event)
{

}
*/

void GAP::sendAuthenticationResponse(GAP_Authentication_Information_t& response,
                                     const GAP_Authentication_Event_Data_t& event,
                                     const Error::Type error_type) const
{
    const int status = GAP_Authentication_Response(Stack::instance().id(),
                                                   event.Remote_Device,
                                                   &response);
    if (status != 0) {
        Error::instance().set(error_type, status);
    }
}

void BTPSAPI GAP::gapEventCallback(unsigned int stack_id,
                                   GAP_Event_Data_t* gap_event_data,
                                   unsigned long callback_parameter)
{
    if (stack_id == Stack::instance().id() && gap_event_data != NULL) {
        GAP::instance().handle(*gap_event_data);
    }
}
