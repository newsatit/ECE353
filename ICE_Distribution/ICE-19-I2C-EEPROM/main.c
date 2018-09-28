// Copyright (c) 2014, Joe Krachey
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
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>


#include "main.h"


//*****************************************************************************
// 
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
// 
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}


//*****************************************************************************
//*****************************************************************************
void initializeBoard(void)
{
  DisableInterrupts();
  init_serial_debug(true, true);
  eeprom_init();
  EnableInterrupts();
}


//*****************************************************************************
//*****************************************************************************
void eeprom_test(void)
{
  uint16_t addr;
  uint8_t values[20];
  uint8_t read_val;
  bool status = true;
  
  for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
  {
      values[ addr - ADDR_START] = rand();
      printf("Writing %i\n\r",values[addr-ADDR_START]);
      eeprom_byte_write(I2C1_BASE,addr, values[addr-ADDR_START]);
  }
  
  for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
  {
      eeprom_byte_read(I2C1_BASE,addr, &read_val);
      if( read_val != values[addr-ADDR_START])
      {
        printf("ERROR: addr: %i write: %i read %i\n\r",addr,values[addr-ADDR_START], read_val);
        status = false;
      }
  }
  
  if(status)
  {
    printf("EEPROM Test passed\n\r");
  }
  else
  {
    printf("EEPROM Test failed\n\r");
  }
  
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{

  
  initializeBoard();

  if( !validate_ice(ICE_I2C_EEPROM) )
  {
     while(1){};
  }
  
  printf("\n\r");
  printf("**************************************\n\r");
  printf("* In Class - I2C\n\r");
  printf("**************************************\n\r");
  
  eeprom_test();
  
  while(1){
  
  };

}

