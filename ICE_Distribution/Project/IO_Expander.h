#ifndef __IO_EXPANDER_IO__
#define __IO_EXPANDER_IO__

#include <stdint.h>
#include <stdbool.h>

#include "TM4C123.h"
#include "driver_defines.h"
#include "gpio_port.h"
#include "i2c.h"

#define 	IO_EXPANDER_DEV_ID                  0x27
#define FT6X06_DEV_ID                  0x38

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

#define   FT6X06_IRQ_GPIO_BASE     GPIOF_BASE
#define   FT6X06_IRQ_PIN_NUM       PF4

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

#define   SERIAL_DBG_RX_PCTL_M    GPIO_PCTL_PF0_M
#define   SERIAL_DBG_RX_PCTL      GPIO_PCTL_PA0_U0RX
#define   SERIAL_DBG_TX_PCTL      GPIO_PCTL_PA1_U0TX


#define FT6X06_DEV_MODE_R             0x00
#define FT6X06_GET_ID_R               0x01
#define FT6X06_TD_STATUS_R            0x02
#define FT6X06_P1_XH_R                0x03
#define FT6X06_P1_XL_R                0x04
#define FT6X06_P1_YH_R                0x05
#define FT6X06_P1_YL_R                0x06
#define FT6X06_P1_WEIGHT_R            0x07
#define FT6X06_P1_MISC_R              0x08
#define FT6X06_P2_XH_R                0x09
#define FT6X06_P2_XL_R                0x0A
#define FT6X06_P2_YH_R                0x0B
#define FT6X06_P2_YL_R                0x0C
#define FT6X06_P2_WEIGHT_R            0x0D
#define FT6X06_P2_MISC_R              0x0E
#define FT6X06_TH_GROUP_R             0x80
#define FT6X06_TH_DIFF_R              0x85
#define FT6X06_CTRL_R                 0x86
#define FT6X06_TIMEENTERMONITOR_R     0x87
#define FT6X06_PERIODACTIVITY_R       0x88
#define FT6X06_PERIODMONITOR_R        0x89
#define FT6X06_RADIAN_VALUE_R         0x91
#define FT6X06_OFFSET_LEFT_RIGHT_R    0x92
#define FT6X06_OFFSET_UP_DOWN_R       0x93
#define FT6X06_DISTANCE_LEFT_RIGHT_R  0x94
#define FT6X06_DISTANCE_UP_DOWN_R     0x95
#define FT6X06_DISTANCE_ZOOM_R        0x96
#define FT6X06_LIB_VER_H_R            0xA1
#define FT6X06_LIB_VER_L_R            0xA2
#define FT6X06_CIPHER_R               0xA3
#define FT6X06_G_MODE_R               0xA4
#define FT6X06_POWER_MODE_R           0xA5
#define FT6X06_FIRMID_R               0xA6
#define FT6X06_FOCALTECH_ID_R         0xA8
#define FT6X06_REALEASE_CODE_ID_R     0xAF
#define FT6X06_STATE_R                0xBC

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
void MCP23017_config(void);



#endif
