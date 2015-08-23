#ifndef PINS_H_
#define PINS_H_

#include <msp430.h>
#include "gpio.h"
#include <stdint.h>

struct Pin {
    Pin(const uint8_t port, const uint16_t pin);
    uint8_t port;
    uint16_t pin;
};

#define PIN_XTAL32K_IN_PORT   GPIO_PORT_P7
#define PIN_XTAL32K_IN_PIN    GPIO_PIN0
#define PIN_XTAL32K_OUT_PORT  GPIO_PORT_P7
#define PIN_XTAL32K_OUT_PIN   GPIO_PIN1

#define PIN_BT_UART           UCA2CTLW0
#define PIN_BT_UART_IV		  USCI_A2_VECTOR
#define PIN_BT_UART_IVR       UCA2IV
#define PIN_BT_UART_PORT      P9IN
#define PIN_BT_UART_TX_PIN    BIT4
#define PIN_BT_UART_RX_PIN    BIT5
#define PIN_BT_RTS_PORT       P1IN
#define PIN_BT_RTS_PIN        BIT4
#define PIN_BT_CTS_PORT       P1IN
#define PIN_BT_CTS_PIN        BIT3
#define PIN_BT_CTS_IN         P1IV_P1IFG3
#define PIN_BT_CTS_IV		  PORT1_VECTOR
#define PIN_BT_CTS_IVR        P1IV
#define PIN_BT_NSHUTDOWN_PORT P8IN
#define PIN_BT_NSHUTDOWN_PIN  BIT2
#define PIN_BT_SLOW_CLK_PORT  P11IN
#define PIN_BT_SLOW_CLK_PIN   BIT0
#define PIN_BT_OE_PORT        GPIO_PORT_P10
#define PIN_BT_OE_PIN         GPIO_PIN7

#define PIN_MARG_HAND_USCI       USCI_B3_BASE
#define PIN_MARG_HAND_CLK_PORT   GPIO_PORT_P10
#define PIN_MARG_HAND_CLK_PIN    GPIO_PIN3
#define PIN_MARG_HAND_SOMI_PORT  GPIO_PORT_P10
#define PIN_MARG_HAND_SOMI_PIN   GPIO_PIN2
#define PIN_MARG_HAND_SIMO_PORT  GPIO_PORT_P10
#define PIN_MARG_HAND_SIMO_PIN   GPIO_PIN1
#define PIN_MARG_HAND_CS_AM_PORT GPIO_PORT_P11
#define PIN_MARG_HAND_CS_AM_PIN  GPIO_PIN1
#define PIN_MARG_HAND_CS_G_PORT  GPIO_PORT_P11
#define PIN_MARG_HAND_CS_G_PIN   GPIO_PIN2

#define PIN_MARG_LOWER_ARM_USCI       USCI_B1_BASE
#define PIN_MARG_LOWER_ARM_CLK_PORT   GPIO_PORT_P5
#define PIN_MARG_LOWER_ARM_CLK_PIN    GPIO_PIN5
#define PIN_MARG_LOWER_ARM_SOMI_PORT  GPIO_PORT_P5
#define PIN_MARG_LOWER_ARM_SOMI_PIN   GPIO_PIN4
#define PIN_MARG_LOWER_ARM_SIMO_PORT  GPIO_PORT_P3
#define PIN_MARG_LOWER_ARM_SIMO_PIN   GPIO_PIN7
#define PIN_MARG_LOWER_ARM_CS_AM_PORT GPIO_PORT_P4
#define PIN_MARG_LOWER_ARM_CS_AM_PIN  GPIO_PIN3
#define PIN_MARG_LOWER_ARM_CS_G_PORT  GPIO_PORT_P4
#define PIN_MARG_LOWER_ARM_CS_G_PIN   GPIO_PIN2

#define PIN_MARG_UPPER_ARM_USCI       USCI_B0_BASE
#define PIN_MARG_UPPER_ARM_CLK_PORT   GPIO_PORT_P3
#define PIN_MARG_UPPER_ARM_CLK_PIN    GPIO_PIN3
#define PIN_MARG_UPPER_ARM_SOMI_PORT  GPIO_PORT_P3
#define PIN_MARG_UPPER_ARM_SOMI_PIN   GPIO_PIN2
#define PIN_MARG_UPPER_ARM_SIMO_PORT  GPIO_PORT_P3
#define PIN_MARG_UPPER_ARM_SIMO_PIN   GPIO_PIN1
#define PIN_MARG_UPPER_ARM_CS_AM_PORT GPIO_PORT_P4
#define PIN_MARG_UPPER_ARM_CS_AM_PIN  GPIO_PIN1
#define PIN_MARG_UPPER_ARM_CS_G_PORT  GPIO_PORT_P4
#define PIN_MARG_UPPER_ARM_CS_G_PIN   GPIO_PIN0

#define PIN_LED_LEFT_PORT         GPIO_PORT_P8
#define PIN_LED_LEFT_PIN          GPIO_PIN3
#define PIN_LED_MIDDLE_LEFT_PORT  GPIO_PORT_P8
#define PIN_LED_MIDDLE_LEFT_PIN   GPIO_PIN1
#define PIN_LED_MIDDLE_RIGHT_PORT GPIO_PORT_P8
#define PIN_LED_MIDDLE_RIGHT_PIN  GPIO_PIN0
#define PIN_LED_RIGHT_PORT        GPIO_PORT_P7
#define PIN_LED_RIGHT_PIN         GPIO_PIN3

#define PIN_FLEX_PORT     GPIO_PORT_P6
#define PIN_FLEX_PIN      GPIO_PIN4
#define PIN_MOTOR_PORT    GPIO_PORT_P6
#define PIN_MOTOR_PIN     GPIO_PIN3
#define PIN_CAPACITY_PORT GPIO_PORT_P2
#define PIN_CAPACITY_PIN  GPIO_PIN1

#define PIN_USB_CONNECTED_PORT GPIO_PORT_P1
#define PIN_USB_CONNECTED_PIN  GPIO_PIN5

#endif /* PINS_H_ */