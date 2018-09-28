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

//ADD CODE #2

// ADD CODE #3

/******************************************************************************
 * Global Variables
 *****************************************************************************/

// ADD CODE #4

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
// Function:      pc_buffer_test_0
// Description:   Adds then removes data from the PC buffer one at a time.
//*****************************************************************************
bool pc_buffer_test_0(uint16_t buffer_size)
{
  char input, output;
  uint8_t i;
  
  for(i = 0; i < 4*buffer_size; i++)
  {
    
    // If the PC buffer is full, return false
    if(pc_buffer_full(&Circular_Buffer))
    {
      return false;
    }
    
    // Get random data
    input = rand();
    
    // Add the data to the PC Buffer
    pc_buffer_add(&Circular_Buffer, input);
    
    // If the PC buffer is full, return false
    if(pc_buffer_full(&Circular_Buffer))
    {
      return false;
    }
    
    // If the PC buffer is empty, return false
    if(pc_buffer_empty(&Circular_Buffer))
    {
      return false;
    }
    
    // Remove the data
    pc_buffer_remove(&Circular_Buffer,&output);
    
    // If the pc_buffer is NOT empty, return false
    if(!pc_buffer_empty(&Circular_Buffer))
    {
      return false;
    }
    
    // If the input data does NOT match the output data, return false
    if ( input != output)
    {
      return false;
    }
  }
  
 // Validate that the PC Buffer is empty
  if( !pc_buffer_empty(&Circular_Buffer))
  {
    return false;
  }
  
  // Return true
  return true;
 
}

//*****************************************************************************
// Function:      pc_buffer_test_1
// Description:   Fills the PC buffer and then empties the buffer.
//*****************************************************************************
bool pc_buffer_test_1(uint16_t buffer_size)
{
  char *input;
  char output;
  uint8_t i;
  
  input = malloc(buffer_size);
  
  for(i = 0; i < buffer_size; i++)
  {
    // Get random data
    input[i] = rand();
  }
  
  // Fill the PC buffer with data
  for(i = 0; i < buffer_size; i++)
  {
    
    // If the PC buffer is full, return false
    if(pc_buffer_full(&Circular_Buffer))
    {
      return false;
    }
    
    // Add the data to the PC Buffer
    pc_buffer_add(&Circular_Buffer, input[i]);
    
    // If the PC buffer is empty, return false
    if(pc_buffer_empty(&Circular_Buffer))
    {
      return false;
    }
  }
  
 // Validate that the PC Buffer is full
  if( !pc_buffer_full(&Circular_Buffer))
  {
    return false;
  }
  
  // empty the circular buffer and verify the data removed matches what was
  // placed in the circular buffer.
  for(i = 0; i < buffer_size; i++)
  {
    
    // If the PC buffer is empty, return false
    if(pc_buffer_empty(&Circular_Buffer))
    {
      return false;
    }
    
    // Remove data from the PC Buffer
    pc_buffer_remove(&Circular_Buffer, &output);
    
    if( output != input[i])
    {
      return false;
    }
  }
  
  // If the PC buffer is NOT empty, return false
  if(!pc_buffer_empty(&Circular_Buffer))
  {
    return false;
  }
  
  free(input);
  
  // Return true
  return true;
}



//*****************************************************************************
//*****************************************************************************
int 
main(void)
{

	if( validate_ice(ICE_PC_BUFFER) == false)
	{
		// Error in configuing the board, so enter infinite loop.
		DisableInterrupts();
		while(1){};
	}
  put_string("\n\r\n\r");
  put_string("**************************************\n\r");
  put_string("* ECE353: ICE-PC-Buffer\n\r");
  put_string("**************************************\n\r");
  
  // ADD CODE 10
  
  if (pc_buffer_test_0(BUFFER_SIZE) )
  {
    put_string("\tPASSED pc_buffer_test_0\n\r");
  }
  else
  {
    put_string("\tFAILED pc_buffer_test_0\n\r");
  }
  
  
  if (pc_buffer_test_1(BUFFER_SIZE) )
  {
    put_string("\tPASSED pc_buffer_test_1\n\r");
  }
  else
  {
    put_string("\tFAILED pc_buffer_test_1\n\r");
  }
  
  // Infinite Loop
  while(1){
  };
}
