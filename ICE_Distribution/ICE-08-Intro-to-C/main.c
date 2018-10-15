//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "driver_defines.h"
#include "validate.h"

/******************************************************************************
 * Includes
 *****************************************************************************/
// ADD CODE 02
#include "ws2812b.h"
/******************************************************************************
 * MACROS
 *****************************************************************************/
// ADD CODE 03
#define NUM_LEDS 8


/******************************************************************************
 * Global Variables
 *****************************************************************************/
// ADD CODE 04
WS2812B_t LEDs[NUM_LEDS];




//*****************************************************************************
// External Functions
//*****************************************************************************
extern void WS2812B_write(uint32_t port_base_addr, uint8_t *led_array_base_addr, uint16_t num_leds);

//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  int i;
  
	if( validate_ice(ICE_INTRO_TO_C) == false)
	{
		// Error in configuing the board, so enter infinite loop.
		DisableInterrupts();
		while(1){};
	}
  put_string("\n\r\n\r");
  put_string("**************************************\n\r");
  put_string("* ECE353: ICE-08-Intro-To-C\n\r");
  put_string("**************************************\n\r");

  // ADD CODE 05
	LEDs[0].blue = 0x80;
	
	LEDs[1].red = 0x80;
	
	LEDs[2].red = 0x80;
	LEDs[2].blue = 0x80;
	
	LEDs[3].green = 0x80;
	
	LEDs[4].green = 0x80;
	LEDs[4].blue = 0x80;
	
	LEDs[5].green = 0x80;
	LEDs[5].red = 0x80;
	
	LEDs[6].red = 0x80;
	LEDs[6].green = 0x80;
	LEDs[6].blue = 0x80;
	
	LEDs[7].green = 0x40;
	LEDs[7].red = 0x20;
	LEDs[7].blue = 0x80;
 
  // ADD CODE 06
	WS2812B_write(WS2812B_GPIO_ADDR, (uint8_t *)LEDs, NUM_LEDS);
  
  // Infinite Loop
  while(1) {

    
    // ADD CODE 07
		ws2812b_rotate(LEDs, NUM_LEDS); 

    // ADD CODE 08
		for(i = 0; i < 500000; i++) {}

    // ADD CODE 08
		WS2812B_write(WS2812B_GPIO_ADDR, (uint8_t *)LEDs, NUM_LEDS);
    
  };
}
