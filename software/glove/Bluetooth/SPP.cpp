#include "Bluetooth\SPP.h"
#include "Bluetooth\Stack.h"
#include "Bluetooth\Bond.h"
#include "Bluetooth\GAP.h"
#include "Bluetooth\SerialPort.h"
#include "Platform.h"
#include "HCIAPI.h"
#include "Error.h"
#include "GAPAPI.h"
#include "StopReadoutCommand.h"
#include "SendCalibrationCommand.h"
#include "MARG.h"
#include "System.h"
#include <stdlib.h>
using namespace Bluetooth;

SPP& SPP::instance(Context* context)
{
    static SPP instance(context);
    return instance;
}

SPP::SPP(Context* context)
:   id_(invalid_id),
    sdp_handle_(invalid_sdp_handle),
    connection_handle_(invalid_connection_handle),
	context_(context),
	is_in_sniff_mode_(false)
{
	int status = 0;

#ifdef SYSTEM_SPP_INCREASE_TRANSMIT_BUFFER
	// Update SPP configuration to increase throughput
	SPP_Configuration_Params_t spp_configuration = {
		.MaximumFrameSize   = 256,
		.TransmitBufferSize = 1024,
		.ReceiveBufferSize  = 256
	};
	status = SPP_Set_Configuration_Parameters(Stack::instance().id(), &spp_configuration);
	if (status != 0) {
        Error::instance().set(Error::TYPE_BT_SPP_CONFIG, status);
	}
#endif

    // Open serial port
    status = SPP_Open_Server_Port(Stack::instance().id(),
                                      SPP_PORT_NUMBER_MINIMUM,
                                      sppEventCallback,
                                      0);
    if (status > 0) {
        char service_name[64];
        id_ = status;

        /* Manual accept to check for bonded device and
         * prevent multiple connections.
         */
        status = SPP_Set_Server_Connection_Mode(Stack::instance().id(),
                                                id_,
                                                smManualAccept);

        if (status == 0) {
            // Register named serial port via SDP for remote devices to see
            BTPS_SprintF(service_name, "SoundGlove", id_, SPP_PORT_NUMBER_MINIMUM);
            status = SPP_Register_SDP_Record(Stack::instance().id(),
                                             id_,
                                             (SPP_SDP_Service_Record_t*) NULL,
                                             service_name,
                                             &sdp_handle_);
            if (status < 0) {
                Error::instance().set(Error::TYPE_BT_SPP_SDP_REG, status);
            }
        }
        else {
            Error::instance().set(Error::TYPE_BT_SPP_CONN_MODE, status);
        }
    }
    else {
        Error::instance().set(Error::TYPE_BT_SPP_OPEN, status);
    }
#ifdef READOUT_100HZ_SNIFF
		enterSniffMode();
#endif
}

SPP::~SPP()
{
#ifdef READOUT_100HZ_SNIFF
	exitSniffMode();
#endif
    if (sdp_handle_ != invalid_sdp_handle) {
        SPP_Un_Register_SDP_Record(Stack::instance().id(),
                                   id_,
                                   sdp_handle_);
        sdp_handle_ = invalid_sdp_handle;
    }
    if (id_ != invalid_id) {
        SPP_Close_Server_Port(Stack::instance().id(), id_);
        id_ = invalid_id;
    }
    connection_handle_ = invalid_connection_handle;
}

int SPP::id() const
{
    return id_;
}

bool SPP::isOpen() const
{
    return id_ != invalid_id && sdp_handle_ != invalid_sdp_handle;
}

bool SPP::isConnected() const
{
    return connection_handle_ != invalid_connection_handle;
}

void SPP::enterSniffMode()
{
	if (!is_in_sniff_mode_) {
		Byte_t status_byte;
		const int status = HCI_Sniff_Mode(Stack::instance().id(),
					                      connection_handle_,
										  sniff_max_interval,
										  sniff_min_interval,
										  sniff_attempt,
										  sniff_timeout,
					                      &status_byte);
		if (status == 0) {
			is_in_sniff_mode_ = true;
		}
		else {
			Error::instance().set(Error::TYPE_BT_SPP_SNIFF_ENTER, status);
		}
	}
}

void SPP::exitSniffMode()
{
	if (is_in_sniff_mode_) {
		Byte_t status_byte;
		const int status = HCI_Exit_Sniff_Mode(Stack::instance().id(), connection_handle_, &status_byte);
		if (status == 0) {
			is_in_sniff_mode_ = false;
		}
		else {
			Error::instance().set(Error::TYPE_BT_SPP_SNIFF_EXIT, status);
		}
	}
}

void SPP::handle(const SPP_Event_Data_t& event)
{
    switch (event.Event_Data_Type) {
    case etPort_Open_Request_Indication:
        if (event.Event_Data.SPP_Open_Port_Indication_Data != NULL
        &&	isOpen()
        &&	!isConnected())
        {
            tryConnect(*event.Event_Data.SPP_Open_Port_Indication_Data);
        }
        break;
    case etPort_Open_Indication:
        if (event.Event_Data.SPP_Open_Port_Indication_Data != NULL) {
            connected(*event.Event_Data.SPP_Open_Port_Indication_Data);
        }
        break;
    case etPort_Close_Port_Indication:
        disconnected();
        break;
    case etPort_Data_Indication:
        SerialPort::instance().receiveData();
        break;
    case etPort_Transmit_Buffer_Empty_Indication:
        SerialPort::instance().signalTransmitReady();
        break;
    case etPort_Send_Port_Information_Indication:
        if (event.Event_Data.SPP_Send_Port_Information_Indication_Data != NULL) {
            sendPortInfo(*event.Event_Data.SPP_Send_Port_Information_Indication_Data);
        }
        break;
    default:
        break;
    }
}

void SPP::tryConnect(const SPP_Open_Port_Indication_Data_t& event)
{
    /* Only accept connections from our bonded device to this serial port.
     * NOTE: If bonding is desired in the future, check:
     * http://e2e.ti.com/support/wireless_connectivity/f/660/p/214749/760375#760375
     */
    const Boolean_t response = !isConnected()
                            && event.SerialPortID == id_
                            && Bond::instance().hasRemoteAddress(event.BD_ADDR);
    const int status = SPP_Open_Port_Request_Response(Stack::instance().id(),
                                                      id_,
                                                      response);
    if (status < 0) {
        Error::instance().set(Error::TYPE_BT_SPP_OPEN_REQ, status);
    }
}

void SPP::connected(const SPP_Open_Port_Indication_Data_t& event)
{
    if (event.SerialPortID == id_
    &&  Bond::instance().hasRemoteAddress(event.BD_ADDR))
    {
        const int status = GAP_Query_Connection_Handle(Stack::instance().id(),
                                                       event.BD_ADDR,
                                                       &connection_handle_);
        if (status == 0) {
            GAP::instance().setAllowsPairing(false);
        }
        else {
            Error::instance().set(Error::TYPE_BT_SPP_CONN_HANDLE, status);
        }

        SendCalibrationCommand calibration_command;
		calibration_command.calibration[0] = MARG::hand().calibration();
		calibration_command.calibration[1] = MARG::forearm().calibration();
		calibration_command.calibration[2] = MARG::upperArm().calibration();
		calibration_command.send(*context_);
    }
}

void SPP::disconnected()
{
	StopReadoutCommand stop;
    connection_handle_ = invalid_connection_handle;
    stop.execute(*context_);
    GAP::instance().setAllowsPairing(true);
}

void SPP::sendPortInfo(const SPP_Send_Port_Information_Indication_Data_t& event)
{
    /* According to demo:
     * "Simply Respond with the information that was sent to us."
     * (Why?)
     */
    SPP_Port_Information_t info = event.SPPPortInformation;
    const int status = SPP_Respond_Port_Information(Stack::instance().id(),
                                                    event.SerialPortID,
                                                    &info);
    if (status < 0) {
        Error::instance().set(Error::TYPE_BT_SPP_INFO, status);
    }
}

void BTPSAPI SPP::sppEventCallback(unsigned int stack_id,
                                   SPP_Event_Data_t* spp_event_data,
                                   unsigned long callback_parameter)
{
    if (stack_id == Stack::instance().id() && spp_event_data != NULL) {
        SPP::instance().handle(*spp_event_data);
    }
}
