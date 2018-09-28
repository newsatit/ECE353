//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"

extern int32_t addMultiply(int8_t *a0, int8_t *a1, int8_t *a2, int8_t *a3, int32_t size);

int8_t array1[] = { 0, 1, 2, 3, 4, 5, 6, 7};
int8_t array2[] = { 1, 2, 3, 4, 5, 6, 7, 8};
int8_t array3[] = { 2, 2, 2, 2, 2, 2, 2, 2};
int8_t ans[8];
  
//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  int32_t returnVal;
  int8_t i;
  

  // Call the EABI compliant subroutine
  returnVal = addMultiply(array1, array2, array3, ans, sizeof(array1));
  
  // Verify that the paramters were all valid
  if (returnVal != 0)
  {
     // If you reached this point, you did not correctly set the return
     // value to 0
     while(1){};
  }
  
  // Check to see if the results calculated match the problem description.
  for(i = 0; i < 8; i++)
  {
    if( ans[i] != ((array1[i] + array2[i]) * array3[i]))
    {
      // If you reach this point, there is something wrong with your assembly function
      while(1){};
    }
  }

  // If you reach this point, your impementation of addMultiply is correct
  while(1){};
}
