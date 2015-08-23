/*****< HARDWARE.c >**********************************************************/
/*      Copyright 2010 - 2014 Stonestreet One.                               */
/*      All Rights Reserved.                                                 */
/*                                                                           */
/*  HARDWARE - Hardware API for MSP430 Experimentor Board                    */
/*                                                                           */
/*  Author:  Tim Cook                                                        */
/*                                                                           */
/*** MODIFICATION HISTORY ****************************************************/
/*                                                                           */
/*   mm/dd/yy  F. Lastname    Description of Modification                    */
/*   --------  -----------    -----------------------------------------------*/
/*   07/07/10  Tim Cook       Initial creation.                              */
/*****************************************************************************/
#include <msp430.h>
#include "inc/hw_regaccess.h"
#include <string.h>
#include "HAL.h"                 /* MSP430 Hardware Abstraction API.         */
#include "HRDWCFG.h"             /* SS1 MSP430 Hardware Configuration Header.*/
#include "BTPSKRNL.h"
#include "SubsystemClock.h"
#include "AuxiliaryClock.h"

static const unsigned long BTPS_MSP430_DEFAULT_BAUD = 115200;

   /* Macro to stop the OS Scheduler.                                   */
#define STOP_SCHEDULER()   (TA1CTL &= (~(MC_3)))

   /* Instruction to start the Scheduler Tick ISR.                      */
#define START_SCHEDULER()  (TA1CTL |= MC_1)

   /* External functions called by this module.  These are neccessary   */
   /* for UART operation and reside in HCITRANS.c                       */

   /* Called upon reception of a CTS Interrupt. Must toggle Interrupt   */
   /* Edge Polarity and flag Tx Flow Enabled State.                     */
extern int CtsInterrupt(void);

   /* * NOTE * The following are the allowed flags for the flags        */
   /*          argument.                                                */
   /*  1.  UART_CONFIG_WLEN_8, UART_CONFIG_WLEN_7                       */
   /*  2.  UART_CONFIG_PAR_NONE,UART_CONFIG_PAR_ODD,UART_CONFIG_PAR_EVEN*/
   /*  3.  UART_CONFIG_STOP_ONE,UART_CONFIG_STOP_TWO                    */
   /*          The flags is a bitfield which may include one flag from  */
   /*          each of the three rows above                             */
void HAL_CommConfigure(unsigned char *UartBase, unsigned long BaudRate, unsigned char Flags)
{
   unsigned long Frequency;
   unsigned int  Divider;

   /* Since we allow access to register clear any invalid flags.        */
   Flags &= (UART_CONFIG_PAR_EVEN | UART_CONFIG_WLEN_7 | UART_CONFIG_STOP_TWO);

   /* set UCSWRST bit to hold UART module in reset while we configure   */
   /* it.                                                               */
   HWREG8(UartBase + MSP430_UART_CTL1_OFFSET) = MSP430_UART_CTL1_SWRST;

   /* Configure control register 0 by clearing and then setting the     */
   /* allowed user options we also ensure that UCSYNC = Asynchronous    */
   /* Mode, UCMODE = UART, UCMSB = LSB first and also ensure that the   */
   /* default 8N1 configuration is used if the flags argument is 0.     */
   HWREG8(UartBase + MSP430_UART_CTL0_OFFSET) = Flags;

   /* UART peripheral erroneous characters cause interrupts break       */
   /* characters cause interrupts on reception                          */
   HWREG8(UartBase + MSP430_UART_CTL1_OFFSET) |= (MSP430_UART_CTL1_RXIE | MSP430_UART_CTL1_BRKIE);

   /* clear UCA status register                                         */
   HWREG8(UartBase + MSP430_UART_STAT_OFFSET)  = 0x00;

   /* clear interrupt flags                                             */
   HWREG8(UartBase + MSP430_UART_IFG_OFFSET)  &= ~(MSP430_UART_TXIFG_mask | MSP430_UART_RXIFG_mask);

   /* If the baud rate is not valid, use the default.                   */
   if(!BaudRate)
      BaudRate = BTPS_MSP430_DEFAULT_BAUD;

   /* Use ACLK for Baud rates less than 9600 to allow us to still       */
   /* receive characters while in LPM3.                                 */
   if(BaudRate <= 9600)
   {
      HWREG8(UartBase + MSP430_UART_CTL1_OFFSET) |= MSP430_UART_CTL1_UCSSEL_ACLK_mask;
      Frequency = AuxiliaryClock::instance().frequency();
   }
   else
   {
      HWREG8(UartBase + MSP430_UART_CTL1_OFFSET) |= MSP430_UART_CTL1_UCSSEL_SMCLK_mask;
      Frequency = SubsystemClock::instance().frequency();
   }

   /* Calculate the initial baud rate divisor. This value can be used   */
   /* for correct rounding for either the low frequency or oversampling */
   /* calculations.                                                     */
   Divider = (unsigned int)((Frequency * 16) / BaudRate);

   /* If the divider will be less than 32, use the low frequency        */
   /* calculation, otherwise use oversampling. Note that the current    */
   /* divider value is 16 times the actual divider.                     */
   if(Divider < (32 * 16))
   {
      /* For low frequency baud rates, first calculate the divider * 8, */
      /* rounded to the nearest integer. The lower 3 bits of the result */
      /* will be the fraction portion of the divider and the rest will  */
      /* be the integer portion of the divider.                         */
      Divider     = (Divider + 1) / 2;
      HWREG16(UartBase + MSP430_UART_BRW_OFFSET) = Divider / 8;
      HWREG8(UartBase + MSP430_UART_MCTL_OFFSET) = (Divider & 0x07) << MSP430_UART_MCTL_BRS_bit;
   }
   else
   {
      /* For oversamping mode, first calculate the divider * 16, rounded*/
      /* to the nearest integer. The lower 4 bits of the result will be */
      /* the fraction portion of the divider and the rest will be the   */
      /* integer portion of the divider.                                */
      Divider   = (Divider + 8) / 16;
      HWREG16(UartBase + MSP430_UART_BRW_OFFSET) = Divider / 16;
      HWREG8(UartBase + MSP430_UART_MCTL_OFFSET) = ((Divider & 0x0F) << MSP430_UART_MCTL_BRF_bit) | MSP430_UART_MCTL_UCOS16_mask;
   }

   /* now clear the UCA2 Software Reset bit                             */
   HWREG8(UartBase + MSP430_UART_CTL1_OFFSET) &= (~(MSP430_UART_CTL1_SWRST));
}

   /* CTS Pin Interrupt. CtsInterrupt routine must change the polarity  */
   /* of the Cts Interrupt.                                             */
#pragma vector=BT_UART_CTS_IV
__interrupt void CTS_ISR(void)
{
   switch(BT_UART_CTS_IVR)
   {
      case BT_UART_CTS_INT_NUM:
         if(CtsInterrupt())
         {
            /* Exit LPM3 on interrupt exit (RETI).                      */
            LPM3_EXIT;
         }
         break;
   }
}

