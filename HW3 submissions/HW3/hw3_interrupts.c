#include "hw3_interrupts.h"

volatile PS2_DIR_t INVADER_DIR = PS2_DIR_CENTER;

//*****************************************************************************
// TIMER2 ISR is used to determine when to move the Invader
//*****************************************************************************
void TIMER2A_Handler(void)
{	 
	// Invader can be moved
	if(!contact_edge(
		INVADER_DIR, 
		INVADER_X_COORD, INVADER_Y_COORD, 
		invaderHeightPixels, invaderWidthPixels)) 
	{
		DisableInterrupts();
		move_image(INVADER_DIR, &INVADER_X_COORD, &INVADER_Y_COORD, invaderHeightPixels, invaderWidthPixels);
		MOVE_INVADER = true;
	}
	
	TIMER2->ICR |= TIMER_ICR_TATOCINT; // Clear Interrupt
}

//*****************************************************************************
// TIMER3 ISR is used to determine when to move the spaceship
//*****************************************************************************
void TIMER3A_Handler(void)
{
  static PS2_DIR_t current_dir = PS2_DIR_CENTER;
  static uint16_t move_count = 0;  
	
	// continue the move
	if(move_count > 0)
	{
		// if not contacting edge (can move)
		if(!contact_edge(
			current_dir, 
			SHIP_X_COORD, SHIP_Y_COORD, 
			space_shipHeightPixels, space_shipWidthPixels)) 
		{
			DisableInterrupts();
			move_count--;
			move_image(current_dir, &SHIP_X_COORD, &SHIP_Y_COORD, space_shipHeightPixels, space_shipWidthPixels);
			MOVE_SHIP = true;
		} 
		// if contact edge generate new move
		else 
		{
			// ensure that new direction does not contact edge
			do {
				current_dir = get_new_direction(current_dir);
			}while(contact_edge(current_dir, SHIP_X_COORD, SHIP_Y_COORD, space_shipHeightPixels, space_shipWidthPixels));
			move_count = get_new_move_count();
		}

	}
	// generate new move
	else
	{
			// ensure that new direction does not contact edge
			do {
				current_dir = get_new_direction(current_dir);
			}while(contact_edge(current_dir, SHIP_X_COORD, SHIP_Y_COORD, space_shipHeightPixels, space_shipWidthPixels));
			move_count = get_new_move_count();
	}
	
	TIMER3->ICR |= TIMER_ICR_TATOCINT; // Clear Interrupt
    
}

//*****************************************************************************
// TIMER4 ISR is used to trigger the ADC
//*****************************************************************************
void TIMER4A_Handler(void)
{	
	ADC0->PSSI =   ADC_PSSI_SS2;     // Start SS2 of ADC0
	
	TIMER4->ICR |= TIMER_ICR_TATOCINT; // Clear Interrupt
    
}

//*****************************************************************************
// ADC0 SS2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{
	uint32_t x, y;
	double x_read, y_read;
	
	// Read 12-bit data
  x = ADC0->SSFIFO2 & 0xFFF;   
	y = ADC0->SSFIFO2 & 0xFFF; 	
	
	// convert the reading of x and y between 0 and 3.3 V
	x_read = (((double)x/((double)(1<<12)-1))*3.3);
	y_read = (((double)y/((double)(1<<12)-1))*3.3);
	
	// determine the direction
	if (x_read > 2.40)
	{
		INVADER_DIR = PS2_DIR_LEFT;
	}
	else if (x_read < 0.85)
	{
		INVADER_DIR = PS2_DIR_RIGHT;
	}
	else if (y_read > 2.40)
	{
		INVADER_DIR = PS2_DIR_UP;
	}
	else if (y_read < 0.85)
	{
		INVADER_DIR = PS2_DIR_DOWN;
	} 
	else if(x_read > 0.85 && x_read < 2.40)
	{
		INVADER_DIR = PS2_DIR_CENTER;
	}
	
  ADC0->ISC  = ADC_ISC_IN2;          // Ack the conversion
}
