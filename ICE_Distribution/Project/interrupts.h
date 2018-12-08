#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "TM4C123.h"
#include "gpio_port.h"
#include "ps2.h"
#include "lcd.h"
#include "timers.h"
#include "IO_Expander.h"

#include "main.h"

//*****************************************************************************
//*****************************************************************************
extern void DisableInterrupts(void);

//*****************************************************************************
//*****************************************************************************
extern void EnableInterrupts(void);


#endif