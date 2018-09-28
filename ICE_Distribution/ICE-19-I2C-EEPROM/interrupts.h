#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "driver_defines.h"
#include "TM4C123.h"
#include "pc_buffer.h"
#include "uart_irqs.h"
#include "validate.h"

extern PC_Buffer UART0_Rx_Buffer;
extern PC_Buffer UART0_Tx_Buffer;
////extern volatile bool  AlertTx;
////extern volatile bool  AlertTimerStart;
//extern volatile bool  AlertTimerStop;
extern volatile bool AlertOneSec;
extern volatile bool AlertButtons;
//extern volatile uint32_t Miliseconds;


#endif
