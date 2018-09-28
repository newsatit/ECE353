// Copyright (c) 2014-16, Joe Krachey
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
//*****************************************************************************
//*****************************************************************************

#include "main.h"

// ADD CODE
// Create a constant called SEC_ONE that is equal to the number
// of clock cycles it will take for 1 second to pass.  
// 
// The clock frequency of the Tiva Launchpad has been configured for 
// 50HMz.
#define SEC_ONE     50000000


// ADD CODE
// Use the macro in driver_defines.h that sets a general purpose in one shot mode
#define ONE_SHOT    TIMER_TAMR_TAMR_1_SHOT



//*****************************************************************************
//*****************************************************************************
int 
  
main(void)
{
  int i = 1;
  char msg[80];
  bool status = false;
  
  initialize_serial_debug();


  put_string("\n\r");
  put_string("************************************\n\r");
  put_string("ECE353 - Timer0 In Class Exercise\n\r");
  put_string("************************************\n\r");
  
  
  // Initialize the TIMER0 to be a 
  //      32-bit
  //      one-shot
  //      count down
  //      no interrupts
  gp_timer_config_32(TIMER0_BASE, ONE_SHOT, false, false);
  
  status = validate_ice(ICE_TIMER0);
  
  // Infinite loop if the configuration of the timer is not correct.
  while(!status)
  {
    // Wait forever
  };
  
  while(1)
  {
    gp_timer_wait(TIMER0_BASE, SEC_ONE);
    sprintf(msg,"SEC :%d\n\r", i);
    put_string(msg);
    i++;
  };
}
