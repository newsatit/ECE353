// Copyright (c) 2015-18, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "main.h"

/******************************************************************************
 * Global Variables
 *****************************************************************************/
 
volatile uint8_t PUSH_BUTTONS = 0;
volatile bool BUTTON_PUSHED = false;
 
typedef enum 
{
  DEBOUNCE_ONE,
  DEBOUNCE_1ST_ZERO,
  DEBOUNCE_2ND_ZERO,
  DEBOUNCE_PRESSED
} DEBOUNCE_STATES;

typedef enum {
	ALL_OFF,
	RED_ON,
	BLUE_ON,
	GREEN_ON
} LED_STATES;
static LED_STATES state = ALL_OFF;

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
bool sw1_debounce_fsm(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  bool pin_logic_level;
  
  pin_logic_level = lp_io_read_pin(SW1_BIT);
  
  switch (state)
  {
    case DEBOUNCE_ONE:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_1ST_ZERO;
      }
      break;
    }
    case DEBOUNCE_1ST_ZERO:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_2ND_ZERO;
      }
      break;
    }
    case DEBOUNCE_2ND_ZERO:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_PRESSED;
      }
      break;
    }
    case DEBOUNCE_PRESSED:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_PRESSED;
      }
      break;
    }
    default:
    {
      while(1){};
    }
  }
  
  if(state == DEBOUNCE_2ND_ZERO )
  {
    return true;
  }
  else
  {
    return false;
  }
}
void debounce_wait(void) 
{
  int i = 10000;
  // Delay
  while(i > 0)
  {
    i--;
  }
}
void initializeBoard(void)
{
  DisableInterrupts();
  init_serial_debug(true, true);
  eeprom_init();
	ft6x06_init();
	accel_initialize();
  EnableInterrupts();
}
//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	uint16_t addr;
  uint8_t values[20];
  uint8_t read_val;
	uint8_t push_val;
	uint8_t touch_event;
	int16_t accel;
	bool pressed = false;
 
  lp_io_init();
	initializeBoard();
	MCP23017_config();
//push_val = MCP23017_read_push_buttons();
	//printf("%d\n", push_val)
	printf("hello world\n");
	push_val = MCP23017_read_push_buttons();
	printf("%d\n", push_val);
	
	MCP23017_write_leds(0xAA);
	
	push_val = MCP23017_read_push_buttons();
	printf("%d\n", push_val);
	while(1){
      // Delay before entering the code to determine which FSM state to 
      // transistion to.
      debounce_wait();
			pressed = sw1_debounce_fsm();		
			if(pressed){
					for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
					{
							values[ addr - ADDR_START] = rand();
							printf("Writing %i\n\r",values[addr-ADDR_START]);
							eeprom_byte_write(I2C1_BASE,addr, values[addr-ADDR_START]);
					}
					for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
					{
							eeprom_byte_read(I2C1_BASE,addr, &read_val);
							printf("Reading %i\n\r",read_val);
					}
			}
			
			// touch_event = ft6x06_read_td_status();
			// printf("touch event: %d\n", touch_event);
			
			//push_val = MCP23017_read_push_buttons();
			//printf("%d\n", push_val);
			
			accel = accel_read_x();
			printf("%d\n", accel);
			
			if(BUTTON_PUSHED) {
					BUTTON_PUSHED = false;
					printf("%d\n", PUSH_BUTTONS);
					EnableInterrupts();
			}

	}	
  // Reach infinite loop after the game is over.
  while(1){};
}
