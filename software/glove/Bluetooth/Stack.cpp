#include "Bluetooth\Stack.h"
#include "Error.h"
#include "AuxiliaryClock.h"
#include "System.h"
#include "BTPSKRNL.h"
#include "BSCAPI.h"
#include "BTPSVEND.h"
#include "BTVSAPI.h"
#include "Pins.h"
#include "gpio.h"
#include <msp430.h>
#include <stdlib.h>
using namespace std;
using namespace Bluetooth;

volatile unsigned long Stack::tick_count = 0;

Stack& Stack::instance()
{
    static Stack instance;
    return instance;
}

Stack::Stack()
:   id_(uninitialized_id),
    callback_id_(uninitialized_callback_id),
    hci_version_(hvSpecification_1_0B)
{
    int status = 0;
    Byte_t hci_policy_status = 0;
    HCI_DriverInformation_t hci_driver_information;
    BTPS_Initialization_t bpts_initialization = {
    	getTickCountCallback,
		messageOutputCallback
    };
	L2CA_Link_Connect_Params_t  L2CA_Link_Connect_Params = {
		cqAllowRoleSwitch,
		csMaintainCurrentRole
	};

    __enable_interrupt();

    enableLogicLevelShifter();
    configureTickCountTimer();

    /* Initialize driver with default baud rate of 115200 at COM port 1 using
     * the HCI low level communication interface with TI RTS and CTS signalling */
    HCI_DRIVER_SET_COMM_INFORMATION(&hci_driver_information, 1, 115200, cpHCILL_RTS_CTS);
    hci_driver_information.DriverInformation.COMMDriverInformation.InitializationDelay = 100;

    // Initialize stack
    BTPS_Init(&bpts_initialization);

    // Initialize stack for device
    status = BSC_Initialize(&hci_driver_information, 0);
    if (status <= 0) {
        Error::instance().set(Error::TYPE_BT_STACK_BSC_INIT, status);
    }

	id_ = status;

	// Allow master/slave role switch for new L2CAP links
	status = L2CA_Set_Link_Connection_Configuration(id(), &L2CA_Link_Connect_Params);
	if (status != 0) {
		 Error::instance().set(Error::TYPE_BT_STACK_L2CAP_ROLE, status);
	}

	// Apply changed L2CAP setting to HCI level
	status = HCI_Write_Default_Link_Policy_Settings(
		id(),
		(HCI_LINK_POLICY_SETTINGS_ENABLE_MASTER_SLAVE_SWITCH | HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE),
		&hci_policy_status);
	if (status != 0) {
		 Error::instance().set(Error::TYPE_BT_STACK_HCI_POLICY, hci_policy_status);
	}

	// Register HCI event callback
	status = HCI_Register_Event_Callback(id(), hciEventCallback, 0);
	if (status <= 0) {
		Error::instance().set(Error::TYPE_BT_STACK_HCI_REG, status);
	}

	callback_id_ = status;

	// Verify that our device supports expected HCI features
	if (HCI_Version_Supported(id(), &hci_version_) != 0) {
		Error::instance().set(Error::TYPE_BT_STACK_HCI_VER, Error::no_info);
	}

	// Update baud rate to maximum
#ifdef SYSTEM_CLOCK_8_MHZ
	status = VS_Update_UART_Baud_Rate(id(), 230400);
#else
	status = VS_Update_UART_Baud_Rate(id(), 921600);
#endif
	if (status != 0) {
		Error::instance().set(Error::TYPE_BT_STACK_MBAUD, status);
	}

#ifdef SYSTEM_BLUETOOTH_MAX_OUTPUT_POWER
	// Maximize output power
	status = VS_Set_Max_Output_Power(id(), 12);
	if (status != 0) {
		Error::instance().set(Error::TYPE_BT_STACK_OUT_POWER, status);
	}
#endif
}

Stack::~Stack()
{
    // Ensure the timer is stopped
    TA1CTL = 0;

    // Deinitialize stack
    HCI_Un_Register_Callback(id(), callback_id_);
    if (isReady()) {
        BSC_Shutdown(id());
    }
    BTPS_DeInit();
}

bool Stack::isReady() const
{
    return id() != uninitialized_id
        && callback_id_ != uninitialized_callback_id;
}

unsigned int Stack::id() const
{
    return id_;
}

HCI_Version_t Stack::hciVersion() const
{
    return hci_version_;
}

char* Stack::debugMessage()
{
    return debug_message_;
}

void Stack::enableLogicLevelShifter() const
{
	const Pin enable(PIN_BT_OE_PORT, PIN_BT_OE_PIN);
	GPIO_setAsOutputPin(enable.port, enable.pin);
	GPIO_setOutputHighOnPin(enable.port, enable.pin);
    __delay_cycles(25e6); // 1us delay by shifter... better take 1s :)
}

void Stack::configureTickCountTimer() const
{
    TA1CTL = 0;                       // Ensure the timer is stopped
    TA1CTL = TASSEL_1 | ID_0;         // Run the timer off of the ACLK
    TA1CTL |= TACLR;                  // Clear everything to start with
    // Set the compare match value according to the tick rate we want
    TA1CCR0 = (AuxiliaryClock::instance().frequency() / (uint32_t) Stack::tick_rate_hz) + 1;
    TA1CCTL0 = CCIE;                  // Enable the interrupts
    TA1CTL |= TACLR;                  // Start up clean
    TA1CTL |= TASSEL_1 | MC_1 | ID_0; // Up mode
}

unsigned long Stack::getTickCountCallback()
{
    return tick_count;
}

void Stack::messageOutputCallback(char c)
{
    static int index = 0;
    instance().debug_message_[index] = c;
    index = index == (debug_message_size-1) ? 0 : index + 1;
}

void Stack::handleHciEvent(const HCI_Event_Data_t& hci_event_data)
{
}

void BTPSAPI Stack::hciEventCallback(unsigned int stack_id,
                                     HCI_Event_Data_t* hci_event_data,
                                     unsigned long callback_parameter)
{
    if (stack_id == Stack::instance().id() && hci_event_data != NULL) {
        Stack::instance().handleHciEvent(*hci_event_data);
    }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1Interrupt()
{
   ++Stack::tick_count;
   LPM3_EXIT;
}
