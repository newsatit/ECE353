#include "ws2812b.h"

/*******************************************************************************
* Function Name: rotate_ws2812bs
********************************************************************************
* Summary:
*
*   Rotates the WS2812B structs are rotates in the array by one position.  This
*   can be done by coping each WS2812B stuct to the next highest index in the 
*   array.  The last WS2812B struct should be moved to base[0]
*
*
* Parameters:
*   base         Base address of the WS2812B_t array
*   num_leds     Size number WS2812B_t structs in the array
*
* Return:
*  void
*
*******************************************************************************/
void ws2812b_rotate( 
  WS2812B_t *base,
  uint8_t num_leds
) {
	WS2812B_t temp = base[num_leds - 1];
	int i;
	for(i = num_leds - 1; i > 0; i--) {
		base[i] = base[i - 1];
	}
	base[0] = temp;
}

