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

void print_ps2(void)
{
  uint16_t x_data, y_data;
  uint32_t i;
  char msg[80];
  while(1)
  {

    x_data = ps2_get_x();
    y_data = ps2_get_y();
    sprintf(msg,"X Dir value : 0x%03x        Y Dir value : 0x%03x\r",x_data, y_data);
    put_string(msg);
    for(i=0;i<100000; i++){}
    
  }
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  initialize_serial_debug();

  put_string("\n\r***************************************\n\r");
  put_string("ECE353 - Analog Input In Class Exercise\n\r");
  put_string("****************************************\n\r");
  
  ps2_initialize(); 
  
  // Run validation code
  if( validate_ice(ICE_ADC) == false)
  {
    while(1){
    };
  }
  
  // Get User Input
  print_ps2();
  
  // Reach infinite loop
  while(1){};
}
