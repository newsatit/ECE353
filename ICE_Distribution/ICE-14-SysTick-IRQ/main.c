// Copyright (c) 2016, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
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

// ADD CODE

/*******************************************************************************
* Function Name: configure_gpio_pins
********************************************************************************
* Summary: Configures the GPIO pins connected to the PS2 Joystick and the BLUE
*          LED on the launchpad
*
* Return:
*  None
*******************************************************************************/
void configure_hardware(void)
{
  lp_io_init();
  ps2_initialize();
}

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary: SysTick Interrupt Service Routine
*
*
* Return:
*  
*
*******************************************************************************/
// ADD CODE

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary: Controls the GREEN LED on the Tiva Launchpad using the PS2 Joystick to
*          set the duty cycle of the LED.
*
*
* Return:
*  
*
*******************************************************************************/
int 
main(void)
{
  initialize_serial_debug();
  
  put_string("\n\r");
  put_string("********************************************\n\r");
  put_string("ECE353 - SysTick Interrupt In Class Exercise\n\r");
  put_string("*********************************************\n\r");
  
  configure_hardware();
    
  SysTick_Config(SYSTICK_LOAD_VAL);
 
  if( !validate_ice(ICE_SYSTICK) )
  {
     while(1){};
  }
  
  while(1)
  {
    // ADD CODE
    
  }
}
