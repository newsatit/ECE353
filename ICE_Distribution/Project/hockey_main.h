#ifndef __HOCKEY_MAIN_H__
#define __HOCKEY_MAIN_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#include "TM4C123.h"
#include "serial_debug.h"
#include "launchpad_io.h"

#include "eeprom.h"
#include "ft6x06.h"
#include "IO_Expander.h"
#include "accel.h"

#define ADDR_START    256
#define NUM_BYTES      20

extern volatile uint8_t push_buttons;
extern volatile bool button_pushed;
extern volatile uint16_t x_data;

void hockey_main();

#endif