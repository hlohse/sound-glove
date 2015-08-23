/*****< HAL.h >****************************************************************/
/*      Copyright 2010 - 2014 Stonestreet One.                                */
/*      All Rights Reserved.                                                  */
/*                                                                            */
/*  HAL - Hardware Abstraction functions for Stellaris LM3S9B96 Board         */
/*                                                                            */
/*  Author:  Tim Thomas                                                       */
/*                                                                            */
/*** MODIFICATION HISTORY *****************************************************/
/*                                                                            */
/*   mm/dd/yy  F. Lastname    Description of Modification                     */
/*   --------  -----------    ------------------------------------------------*/
/*   07/05/11  Tim Thomas     Initial creation.                               */
/******************************************************************************/
#ifndef __HAL_H__
#define __HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

   /* * NOTE * The following are the allowed flags for the flags        */
   /*          argument.                                                */
   /*  1.  UART_CONFIG_WLEN_8, UART_CONFIG_WLEN_7                       */
   /*  2.  UART_CONFIG_PAR_NONE,UART_CONFIG_PAR_ODD,UART_CONFIG_PAR_EVEN*/
   /*  3.  UART_CONFIG_STOP_ONE,UART_CONFIG_STOP_TWO                    */
   /*          The flags is a bitfield which may include one flag from  */
   /*          each of the three rows above                             */
void HAL_CommConfigure(unsigned char *UartBase, unsigned long BaudRate, unsigned char Flags);

#ifdef __cplusplus
}
#endif

#endif

