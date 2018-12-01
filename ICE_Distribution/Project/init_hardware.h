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

#define ADDR_START    256
#define NUM_BYTES      20

#endif


void initializeBoard(void);

void init_timer1(void);