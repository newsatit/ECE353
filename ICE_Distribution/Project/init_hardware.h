#ifndef __INIT_HARDWARE_H__
#define __INIT_HARDWARE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#include "TM4C123.h"
#include "serial_debug.h"
#include "launchpad_io.h"
#include "gpio_port.h"
#include "i2c.h"
#include "spi.h"
#include "timers.h"
#include "uart.h"

#include "eeprom.h"
#include "accel.h"
#include "lcd.h"
#include "ft6x06.h"
#include "IO_Expander.h"

#include "main.h"

#define ADDR_START    256
#define NUM_BYTES      20

#endif


void initializeBoard(void);
/******************************************************************************
 * Timer2 is initialized to be a 32 bit count down timer
 * It generates an interrupt every 1 ms
 *****************************************************************************/
void init_timer2(void);