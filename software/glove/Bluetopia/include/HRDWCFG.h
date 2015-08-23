/*****< HRDWCFG.h >************************************************************/
/*      Copyright 2010 - 2014 Stonestreet One.                                */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*  HRDWCFG - Hardware Configuration header for MSP430 5438 Experimentor      */
/*            Board.                                                          */
/*                                                                            */
/*  Author:  Tim Cook                                                         */
/*                                                                            */
/*** MODIFICATION HISTORY *****************************************************/
/*                                                                            */
/*   mm/dd/yy  F. Lastname    Description of Modification                     */
/*   --------  -----------    ------------------------------------------------*/
/*   08/31/10  Tim Cook       Initial creation.                               */
/******************************************************************************/
#ifndef _HRDWCFG_H_
#define _HRDWCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <msp430.h>        /* Compiler specific Chip header.                  */
#include "MSP430Utils.h"   /* MSP430 defines and Processor Specific Utilities */
#include "Pins.h"
#include "System.h"

   /* The following typedef represents all of the allowable clock speeds   */
   /* that may be specified in the BT_CPU_FREQ macro.                      */
typedef enum
{
   cf8MHZ_t  =  8000000,
   cf16MHZ_t = 16000000,
   cf20MHZ_t = 20000000,
   cf25MHZ_t = 25000000
} Cpu_Frequency_t;

/******************************************************************************/
/** The following defines control the Bluetooth Slow Clock Line.             **/
/******************************************************************************/

   /* The following is the base address of the GPIO Port where the BT   */
   /* Slow Clock line is located.                                       */
#define BT_SLOW_CLOCK_PORT_BASE        ((unsigned char *)&PIN_BT_SLOW_CLK_PORT)

   /* The following is the pin number on BT_SLOW_CLOCK_PORT_BASE which  */
   /* for which the controller's slow clock line is located.            */
#define BT_SLOW_CLOCK_PORT_PIN         (PIN_BT_SLOW_CLK_PIN)

/******************************************************************************/
/** The following defines control the Bluetooth HCI UART Reset Line.         **/
/******************************************************************************/

   /* The following is the base address of the GPIO Port where the BT   */
   /* controlller's reset line is located.  (Default if Port 8)         */
#define BT_DEVICE_RESET_PORT_BASE      ((unsigned char *)&PIN_BT_NSHUTDOWN_PORT)

   /* The following is the pin number on BT_DEVICE_RESET_PORT which for */
   /* which the controller's reset line is located.                     */
#define BT_DEVICE_RESET_PORT_PIN       (PIN_BT_NSHUTDOWN_PIN)

/******************************************************************************/
/** The following defines control the Bluetooth HCI UART RTS/CTS Lines.      **/
/******************************************************************************/

   /* The base address of the Processor Port on which the RTS Pin is    */
   /* located.  (Default=0x200 is address of P1IN).                     */
#define BT_UART_FLOW_RTS_PIN_BASE      ((unsigned char *)&PIN_BT_RTS_PORT)

   /* The base address of the Processor Port on which the CTS Pin is    */
   /* located.  (Default=0x200 is address of P1IN).                     */
#define BT_UART_FLOW_CTS_PIN_BASE      ((unsigned char *)&PIN_BT_CTS_PORT)

   /* The following is the interrupt vector offset of the RTS/CTS Port. */
#define BT_UART_CTS_IV                 (PIN_BT_CTS_IV)

   /* The following is the UART RTS/CTS Interrupt Vector Register.      */
#define BT_UART_CTS_IVR                (PIN_BT_CTS_IVR)

   /* The Bluetooth RTS Output Port Pin Number.                         */
#define BT_UART_RTS_PIN                (PIN_BT_RTS_PIN)

   /* The Bluetooth CTS Input Port Pin Number.                          */
#define BT_UART_CTS_PIN                (PIN_BT_CTS_PIN)

   /* The Bluetooth CTS Interrupt Number.  This should be in the form   */
   /* PXIV_PXIFGY where X is the Port Number (1 in this case) and Y is  */
   /* the Pin Number on the corresponding port (4 in this case).        */
#define BT_UART_CTS_INT_NUM            (PIN_BT_CTS_IN)

/******************************************************************************/
/** The following group of defines control the UART module used to talk to   **/
/** the Bluetooth Controller chip.                                           **/
/******************************************************************************/

   /* The Base Address of the used UART Module.  On the MSP430x5xx      */
   /* family this is set to the address of the Module's Control Word 0  */
   /* Register.  (All other registers accessed are accessed based on    */
   /* their offset from this Address Base).                             */
#define BT_UART_MODULE_BASE            ((unsigned char *)&PIN_BT_UART)

   /* The Bluetooth UART Interrupt Vector Offset.                       */
#define BT_UART_IV                     (PIN_BT_UART_IV)

   /* The Bluetooth UART Interrupt Vector Register.                     */
#define BT_UART_IVR                    (PIN_BT_UART_IVR)

   /* The following is the Base Address of the UART Module's Tx/Rx pin's*/
   /* Port base.  This will be used to configure the Module's Tx/Rx to  */
   /* be used as peripheral pins.                                       */
#define BT_UART_PIN_PORT_BASE          ((unsigned char *)&PIN_BT_UART_PORT)

   /* The UART Module's Tx Pin Mask.                                    */
#define BT_UART_PIN_TX                 (PIN_BT_UART_TX_PIN)

   /* The UART Module's Rx Pin Mask.                                    */
#define BT_UART_PIN_RX                 (PIN_BT_UART_RX_PIN)

/******************************************************************************/
/** The following control the frequency of the processor.                    **/
/******************************************************************************/

   /* The following define controls the speed that the system is        */
   /* configured to run at.                                             */
#ifdef SYSTEM_CLOCK_8_MHZ
#define BT_CPU_FREQ                    (cf8MHZ_t)
#endif
#ifdef SYSTEM_CLOCK_20_10_MHZ
#define BT_CPU_FREQ                    (cf20MHZ_t)
#endif
#ifdef SYSTEM_CLOCK_25_6_MHZ
#define BT_CPU_FREQ                    (cf25MHZ_t)
#endif

/******************************************************************************/
/** The following define the Tick Rate for the Bluetopia NoOS Stack.         **/
/******************************************************************************/
#define MSP430_TICK_RATE_HZ            ((unsigned int)1000)
#define MSP430_TICK_RATE_MS            ((unsigned int)1000 / MSP430_TICK_RATE_HZ)

/*************************NON CONFIGURABLE SECTION*****************************/
/*************************NON CONFIGURABLE SECTION*****************************/
/*************************NON CONFIGURABLE SECTION*****************************/
/**                                                                          **/
/** The following sections consist of MACROs that exist to aid in            **/
/** configuring the Bluetooth Chip that should never need to be changed      **/
/** however for completeness they are included in this file.                 **/
/**                                                                          **/
/*************************NON CONFIGURABLE SECTION*****************************/
/*************************NON CONFIGURABLE SECTION*****************************/
/*************************NON CONFIGURABLE SECTION*****************************/

   /* The following MACRO is used to configure the Slow Clock Line.     */
#define BT_CONFIG_SLOW_CLOCK()         GPIOPinTypePeripheralOutput(BT_SLOW_CLOCK_PORT_BASE, BT_SLOW_CLOCK_PORT_PIN)

   /* The following macro is used to configure the Reset line.          */
#define BT_CONFIG_RESET()              GPIOPinTypeReset(BT_DEVICE_RESET_PORT_BASE, BT_DEVICE_RESET_PORT_PIN)

   /* The following is used to Reset the Bluetooth Controller.          */
#define BT_DEVICE_RESET()              HWREG8((BT_DEVICE_RESET_PORT_BASE) + MSP430F5438_GPIO_OUTPUT_OFFSET) &= (~(BT_DEVICE_RESET_PORT_PIN))

   /* The following is used to bring the Bluetooth Controller out of    */
   /* reset.                                                            */
#define BT_DEVICE_UNRESET()            HWREG8((BT_DEVICE_RESET_PORT_BASE) + MSP430F5438_GPIO_OUTPUT_OFFSET) |= (BT_DEVICE_RESET_PORT_PIN)

   /* The following macros are used to configure the RTS/CTS Flow       */
   /* Control Pins for Hardware Flow Control Emulation.                 */
#define BT_CONFIG_RTS_PIN()            GPIOPinTypeRtsFlow(BT_UART_FLOW_RTS_PIN_BASE, BT_UART_RTS_PIN)
#define BT_CONFIG_CTS_PIN()            GPIOPinTypeCtsFlow(BT_UART_FLOW_CTS_PIN_BASE, BT_UART_CTS_PIN)

   /* The following is used to Disable Flow Control with RTS.           */
#define BT_DISABLE_FLOW()              HWREG8((BT_UART_FLOW_RTS_PIN_BASE) + MSP430F5438_GPIO_OUTPUT_OFFSET) |= (BT_UART_RTS_PIN)

   /* The following is used to Re-enable Flow Control with RTS.         */
#define BT_ENABLE_FLOW()               HWREG8((BT_UART_FLOW_RTS_PIN_BASE) + MSP430F5438_GPIO_OUTPUT_OFFSET) &= (~(BT_UART_RTS_PIN))

   /* The following macro is used to make CTS Interrupt Negative Edge   */
   /* Active.                                                           */
#define BT_CTS_INT_NEG_EDGE()          GPIOIntNegEdge((BT_UART_FLOW_CTS_PIN_BASE), (BT_UART_CTS_PIN))

   /* The following Macro returns Non-Zero if CTS Interrupts are        */
   /* currently Negative Edge Active.                                   */
#define BT_CTS_INT_IS_NEG_EDGE()       GPIOIntEdgeIsNeg((BT_UART_FLOW_CTS_PIN_BASE), (BT_UART_CTS_PIN))

   /* The following macro is used to make CTS Interrupt Positive Edge   */
   /* Active.                                                           */
#define BT_CTS_INT_POS_EDGE()          GPIOIntPosEdge((BT_UART_FLOW_CTS_PIN_BASE), (BT_UART_CTS_PIN))

   /* The following macro is used to read the CTS Pin.                  */
#define BT_CTS_READ()                  (GPIORead(BT_UART_FLOW_CTS_PIN_BASE) & (BT_UART_CTS_PIN))

   /* The following macro is used to configure the UART Module's Pins   */
   /* for use by the module.  (Rather than GPIO).                       */
#define BT_CONFIG_UART_PINS()          GPIOPinTypeUART(BT_UART_PIN_PORT_BASE, BT_UART_PIN_TX,BT_UART_PIN_RX)

#ifdef __cplusplus
}
#endif

#endif
