#ifndef __IO_EXPANDER_IO__
#define __IO_EXPANDER_IO__

#include <stdint.h>
#include <stdbool.h>

#include "TM4C123.h"
#include "driver_defines.h"
#include "gpio_port.h"
#include "i2c.h"

#define 	IO_EXPANDER_DEV_ID                  0x27

//*****************************************************************************
// Fill out the #defines below to configure which pins are connected to
// the I2C Bus
//*****************************************************************************
#define   IO_EXPANDER_GPIO_BASE         GPIOA_BASE
#define   IO_EXPANDER_I2C_BASE          I2C1_BASE
#define   IO_EXPANDER_I2C_SCL_PIN       PA6
#define   IO_EXPANDER_I2C_SDA_PIN       PA7
#define 	IO_EXPANDER_I2C_SCL_PCTL_M		 GPIO_PCTL_PA6_M
#define   IO_EXPANDER_I2C_SCL_PIN_PCTL  GPIO_PCTL_PA6_I2C1SCL
#define 	IO_EXPANDER_I2C_SDA_PCTL_M		 GPIO_PCTL_PA7_M
#define   IO_EXPANDER_I2C_SDA_PIN_PCTL  GPIO_PCTL_PA7_I2C1SDA

#define   IO_EXPANDE_IRQ_GPIO_BASE     GPIOF_BASE
#define   IO_EXPANDE_IRQ_PIN_NUM       PF0

#define IO_EXPANDER_IODIRA						0x00
#define IO_EXPANDER_IODIRB						0x01
#define IO_EXPANDER_GPINTENB					0x05
#define IO_EXPANDER_INTCONB						0x09
#define IO_EXPANDER_GPPUB							0x0D
#define IO_EXPANDER_GPIOA							0x12
#define IO_EXPANDER_GPIOB							0x13
#define IO_EXPANDER_PUSH_BUTTONS			IO_EXPANDER_GPIOB
#define IO_EXPANDER_RIGHT_M						PIN_3
#define IO_EXPANDER_LEFT_M						PIN_2
#define IO_EXPANDER_DOWN_M						PIN_1
#define IO_EXPANDER_UP_M							PIN_0

#define SW2_IO_EXPANDER_INT						PF0
#define IO_EXPANDER_BUTTONS_M					0xFF

// ADD CODE 01
#define   RED_BIT       1
#define   BLUE_BIT      2
#define   GREEN_BIT     3
#define   SW1_BIT       4
#define   SW2_BIT       0

#define   RED_M     (1 << RED_BIT)
#define   BLUE_M    (1 << BLUE_BIT)
#define   GREEN_M   (1 << GREEN_BIT)

#define   SW1_M     (1 << SW1_BIT)
#define   SW2_M     (1 << SW2_BIT)

//*****************************************************************************
// Read the status of push buttons
//*****************************************************************************
uint8_t MCP23017_read_push_buttons(void);

//*****************************************************************************
// Write to the LEDs
// 
// Paramters
//    pins:   a mask of led pins
//
//*****************************************************************************
void MCP23017_write_leds(uint8_t pins);

// Config MCP23017 
void MCP23017_init(void);



#endif
